#include "WavefunctionRenderer.hpp"

WavefunctionRenderer::WavefunctionRenderer()
{
	_W = 100;
	_H = 100;
	_grid = 64;
	_scale = 0.1;
	cam.theta = M_PI / 2.0;
	cam.fov = 0;
	cam.phi = 0;
	raycast = &traceRayDensity;
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

void WavefunctionRenderer::buildVolume()
{
	for (int z = 0; z < _volume.voxels; z++)
	{
		for (int y = 0; y < _volume.voxels; y++)
		{
			for (int x = 0; x < _volume.voxels; x++)
			{
				float px = ((float)x / _volume.voxels - 0.5) * _scale;
				float py = ((float)y / _volume.voxels - 0.5) * _scale;
				float pz = ((float)z / _volume.voxels - 0.5) * _scale;

				_volume.at(x, y, z) = std::norm(_psi(px, py, pz));
			}
		}
	}
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
			if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Left)
			{
				_dragging = true;
				_lastMouse = sf::Mouse::getPosition(*_window);
			}
			if (event.type == sf::Event::MouseButtonReleased &&
				event.mouseButton.button == sf::Mouse::Left)
			{
				_dragging = false;
			}
		}

		handleCameraInput(cam, fb);

		texture.update(fb.raw());
		_window->clear();
		_window->draw(sprite);
		_window->display();
	}
}