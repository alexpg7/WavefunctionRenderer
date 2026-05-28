#include "WavefunctionRenderer.hpp"

void WavefunctionRenderer::buildVolume()
{
	float	value = -1;
	float	max = -1;
	float	min = MAXFLOAT;
	if (!_psi)
		throw SetterException("You must set a function to your WavefunctionRenderer object, use .setWaveFunction(const std::function<std::complex<float>(float, float, float)>& psi)");
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
	_volume.iso = std::clamp(_iso, _volume.min, _volume.max);
	_volume.color1 = _color1;
	_volume.color2 = _color2;
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
	cam.theta -= delta.y * sensitivity;


	this->paintScreen(fb);
}

void	WavefunctionRenderer::handleKeys(sf::Event& event, bool& pressS, bool& pressI, bool& pressP)
{
	if (event.type == sf::Event::KeyReleased)
	{	if (event.key.code == sf::Keyboard::S)
			pressS = false;
		else if (event.key.code == sf::Keyboard::I)
			pressI = false;
		else if (event.key.code == sf::Keyboard::P)
			pressP = false;
	}
	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::S)
		{
			if (!pressS && _ctrl)
				save("");
			pressS = true;
		}
		else if (event.key.code == sf::Keyboard::I)
		{
			if (!pressI && _ctrl)
				instructions();
			pressI = true;
		}
		else if (event.key.code == sf::Keyboard::P)
		{
			if (!pressP && _ctrl)
				stats();
			pressP = true;
		}
	}
}

void	WavefunctionRenderer::show()
{
	//check if everything is set up
	this->buildVolume();
	_window = new sf::RenderWindow(sf::VideoMode(_W, _H), _title);
	Framebuffer fb(_W, _H);
	bool	pressS = false;
	bool	pressI = false;
	bool	pressP = false;

	sf::Texture texture;
	texture.create(_W, _H);

	sf::Sprite sprite(texture);
	paintScreen(fb);

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
			else if (event.type == sf::Event::KeyPressed
				|| event.type == sf::Event::KeyReleased)
				this->handleKeys(event, pressS, pressI, pressP);
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
						_iso += _rate * (_volume.max - _volume.min);
					else if (event.mouseWheelScroll.delta < 0)
						_iso -= _rate * (_volume.max - _volume.min);
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