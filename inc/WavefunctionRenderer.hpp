#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>
#include <functional>
#include <algorithm>
#include <complex>
#include <cmath>

struct Volume
{
	int nx, ny, nz;
	std::vector<double> data;

	Volume(int x, int y, int z): nx(x), ny(y), nz(z), data(x * y * z, 0.0)
	{
	}

	double& at(int x, int y, int z)
	{
		return data[x + y * nx + z * nx * ny];
	}
};

struct Ray
{
	double ox, oy, oz;
	double dx, dy, dz;
};

class WavefunctionRenderer
{
	private:
		int	_W;
		int	_H;
		std::string	_title = "";
		void	buildVolume();
		Volume	_volume = Volume(64, 64, 64);
		unsigned int	_grid;
		std::function<std::complex<double>(double, double, double)> _psi = nullptr;
		double	_scale;
	public:
		WavefunctionRenderer();
		~WavefunctionRenderer();
		void	setResolution(int W, int H);
		void	setTitle(std::string title);
		void	setWaveFunction(const std::function<std::complex<double>(double, double, double)>& psi);
		void	setGrid(unsigned int x);
		void	setScale(double scale);
		void	show();
};

struct Color
{
	std::uint8_t r, g, b, a;
};

class Framebuffer
{
	private:
		int _width;
		int _height;
		std::vector<std::uint8_t> _data;
	public:
		Framebuffer(int w, int h);
		void clear(Color c);
		void setPixel(int x, int y, Color c);
		const std::uint8_t* raw() const;
		int getWidth() const;
		int getHeight() const;
};
