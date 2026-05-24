#include "WavefunctionRenderer.hpp"
#include <iostream>

WavefunctionRenderer::WavefunctionRenderer()
{
	_W = 100;
	_H = 100;
	_grid = 64;
	_scale = 0.1;
	cam.theta = M_PI / 2.0;
	cam.phi = 0;
	raycast = &traceRayDensity;
	_iso = 0.1f;
}

WavefunctionRenderer::~WavefunctionRenderer()
{
	delete _window;
}

void	WavefunctionRenderer::setResolution(int W, int H)
{
	_W = W;
	_H = H;
}

void	WavefunctionRenderer::setTitle(std::string title)
{
	_title = title;
}

void	WavefunctionRenderer::setWaveFunction(const std::function<std::complex<float>(float, float, float)>& psi)
{
	_psi = psi;
}

void	WavefunctionRenderer::setGrid(unsigned int x)
{
	_grid = x;
	_volume = Volume(x);
}

void	WavefunctionRenderer::setScale(float scale)
{
	_scale = scale;
}

void	WavefunctionRenderer::setMode(Mode mode)
{
	switch (mode)
	{
		case Mode::Scattering:
			raycast = &traceRayScattering;
			break;
		case Mode::Surface:
			raycast = &traceRaySurface;
			break;
		default:
			raycast = &traceRayDensity;
	}
}

void	WavefunctionRenderer::setIsosurface(float iso)
{
	_iso = iso;
}

void WavefunctionRenderer::buildVolume()
{
	float	value = -1;
	float	max = -1;
	float	min = MAXFLOAT;
	for (int z = 0; z < _volume.voxels; z++)
	{
		for (int y = 0; y < _volume.voxels; y++)
		{
			for (int x = 0; x < _volume.voxels; x++)
			{
				float px = ((float)x / _volume.voxels - 0.5) * _scale;
				float py = ((float)y / _volume.voxels - 0.5) * _scale;
				float pz = ((float)z / _volume.voxels - 0.5) * _scale;

				value = std::norm(_psi(px, py, pz));
				if (value > max)
					max = value;
				if (value < min)
					min = value;
				_volume.at(x, y, z) = value;
			}
		}
	}
	_volume.max = max;
	_volume.min = min;
	_volume.iso = (_iso > _volume.max) ? _volume.max : _iso;
	_volume.iso = (_iso < _volume.min) ? _volume.min : _iso;
}

void	WavefunctionRenderer::paintScreen(Framebuffer& fb)
{
	Color	color;

	fb.clear({0, 0, 0, 255});
	for (int y = 0; y < fb.getHeight(); y++)
	{
		for (int x = 0; x < fb.getWidth(); x++)
		{
			Ray ray = generateRay(x, y, fb.getWidth(), fb.getHeight(), _scale, cam);

			color = raycast(ray, _volume, _scale);
			fb.setPixel(x, y, color);
		}
	}
}

void WavefunctionRenderer::handleCameraInput(Camera& cam, Framebuffer& fb)
{
	if (!_dragging)
		return;

	sf::Vector2i mouse = sf::Mouse::getPosition(*_window);

	sf::Vector2i delta = mouse - _lastMouse;
	_lastMouse = mouse;

	float sensitivity = 0.005f;

	cam.phi += delta.x * sensitivity;
	cam.theta += delta.y * sensitivity;


	this->paintScreen(fb);
}

void	WavefunctionRenderer::show()
{
	//check if everything is set up
	this->buildVolume();
	_window = new sf::RenderWindow(sf::VideoMode(_W, _H), _title);
	Framebuffer fb(_W, _H);

	sf::Texture texture;
	texture.create(_W, _H);

	sf::Sprite sprite(texture);
	sf::Clock clock;
	sf::Time time = clock.getElapsedTime();
	paintScreen(fb);
	_dt = (clock.getElapsedTime().asSeconds() - time.asSeconds());

	while (_window->isOpen())
	{
		sf::Event event;
		while (_window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				_window->close();
			else if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Left)
			{
				if (_zooming)
				{
					this->buildVolume();
					_zooming = false;
				}
				_dragging = true;
				_lastMouse = sf::Mouse::getPosition(*_window);
			}
			else if (event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::LControl)
				_ctrl = true;
			else if (event.type == sf::Event::KeyReleased &&
				event.key.code == sf::Keyboard::LControl)
				_ctrl = false;
			else if (event.type == sf::Event::MouseButtonReleased &&
				event.mouseButton.button == sf::Mouse::Left)
				_dragging = false;
			else if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (!_ctrl)
				{
					if (event.mouseWheelScroll.delta > 0)
						_scale *= 0.9;
					else if (event.mouseWheelScroll.delta < 0)
						_scale /= 0.9;
					int grid = _grid;
					setGrid(64);
					_zooming = true;
					this->buildVolume();
					this->paintScreen(fb);
					setGrid(grid);
				}
				else
				{
					if (_zooming)
					{
						this->buildVolume();
						_zooming = false;
					}
					if (event.mouseWheelScroll.delta > 0)
						_iso += 0.01 * (_volume.max - _volume.min);
					else if (event.mouseWheelScroll.delta < 0)
						_iso -= 0.01 * (_volume.max - _volume.min);
					_iso = (_iso > _volume.max) ? _volume.max : _iso;
					_iso = (_iso < _volume.min) ? _volume.min : _iso;
					_volume.iso = _iso;
					this->paintScreen(fb);
				}
  			}
		}

		handleCameraInput(cam, fb);

		texture.update(fb.raw());
		_window->clear();
		_window->draw(sprite);
		_window->display();
	}
}