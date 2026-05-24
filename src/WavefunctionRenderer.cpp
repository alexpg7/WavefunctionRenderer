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
}

WavefunctionRenderer::~WavefunctionRenderer()
{
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

void	WavefunctionRenderer::setWaveFunction(const std::function<std::complex<double>(double, double, double)>& psi)
{
	_psi = psi;
}

void	WavefunctionRenderer::setGrid(unsigned int x)
{
	_grid = x;
	_volume = Volume(x);
}

void	WavefunctionRenderer::setScale(double scale)
{
	_scale = scale;
}

void WavefunctionRenderer::buildVolume()
{
	for (int z = 0; z < _volume.voxels; z++)
	{
		for (int y = 0; y < _volume.voxels; y++)
		{
			for (int x = 0; x < _volume.voxels; x++)
			{
				double px = ((double)x / _volume.voxels - 0.5) * _scale;
				double py = ((double)y / _volume.voxels - 0.5) * _scale;
				double pz = ((double)z / _volume.voxels - 0.5) * _scale;

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

			color = traceRay(ray, _volume, _scale);
			fb.setPixel(x, y, color);
		}
	}
}

void WavefunctionRenderer::handleCameraInput(Camera& cam, Framebuffer& fb)
{
	double step = _dt * 3;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		cam.phi -= step;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		cam.phi += step;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		cam.theta += step;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		cam.theta -= step;

	sf::Clock clock;
	sf::Time time = clock.getElapsedTime();
	this->paintScreen(fb);
	_dt = (clock.getElapsedTime().asSeconds() - time.asSeconds());
}

void	WavefunctionRenderer::show()
{
	//check if everything is set up
	this->buildVolume();
	sf::RenderWindow window(sf::VideoMode(_W, _H), _title);
	Framebuffer fb(_W, _H);

	sf::Texture texture;
	texture.create(_W, _H);

	sf::Sprite sprite(texture);
	sf::Clock clock;
	sf::Time time = clock.getElapsedTime();
	paintScreen(fb);
	_dt = (clock.getElapsedTime().asSeconds() - time.asSeconds());

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		handleCameraInput(cam, fb);

		texture.update(fb.raw());
		window.clear();
		window.draw(sprite);
		window.display();
	}
}