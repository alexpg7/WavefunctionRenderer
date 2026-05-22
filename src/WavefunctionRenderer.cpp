#include "WavefunctionRenderer.hpp"

WavefunctionRenderer::WavefunctionRenderer()
{
	_W = 100;
	_H = 100;
	_grid = 64;
	_scale = 0.1;
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
	_volume = Volume(x, x, x);
}

void	WavefunctionRenderer::setScale(double scale)
{
	_scale = scale;
}

void WavefunctionRenderer::buildVolume()
{
	for (int z = 0; z < _volume.nz; z++)
	{
		for (int y = 0; y < _volume.ny; y++)
		{
			for (int x = 0; x < _volume.nx; x++)
			{
				double px = (x - _volume.nx * 0.5) * _scale;
				double py = (y - _volume.ny * 0.5) * _scale;
				double pz = (z - _volume.nz * 0.5) * _scale;

				_volume.at(x, y, z) = std::norm(_psi(px, py, pz));
			}
		}
	}
}

void	WavefunctionRenderer::show()
{
	//check if everything is set up
	WavefunctionRenderer::buildVolume();
	sf::RenderWindow window(sf::VideoMode(_W, _H), _title);
	Framebuffer fb(_W, _H);

	sf::Texture texture;
	texture.create(_W, _H);

	sf::Sprite sprite(texture);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		fb.clear({0, 0, 0, 255});
		for (int y = 0; y < fb.getHeight(); y++)
		{
			for (int x = 0; x < fb.getWidth(); x++)
				fb.setPixel(x, y, {(uint8_t)(x % 255), (uint8_t)(y % 255), 128, 255});
		}

		texture.update(fb.raw());
		window.clear();
		window.draw(sprite);
		window.display();
	}
}