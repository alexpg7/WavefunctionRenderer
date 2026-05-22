#include "WavefunctionRenderer.hpp"

Framebuffer::Framebuffer(int w, int h): _width(w), _height(h), _data(w * h * 4, 0)
{
}

void Framebuffer::clear(Color c)
{
	for (int i = 0; i < _width * _height; i++)
	{
		_data[i * 4 + 0] = c.r;
		_data[i * 4 + 1] = c.g;
		_data[i * 4 + 2] = c.b;
		_data[i * 4 + 3] = c.a;
	}
}

void Framebuffer::setPixel(int x, int y, Color c)
{
	if (x < 0 || y < 0 || x >= _width || y >= _height)
		return;

	int i = (x + y * _width) * 4;

	_data[i + 0] = c.r;
	_data[i + 1] = c.g;
	_data[i + 2] = c.b;
	_data[i + 3] = c.a;
}

const std::uint8_t* Framebuffer::raw() const
{
	return _data.data();
}

int Framebuffer::getWidth() const
{
	return _width;
}

int Framebuffer::getHeight() const
{
	return _height;
}