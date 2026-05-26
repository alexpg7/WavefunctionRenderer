#include "WavefunctionRenderer.hpp"

WavefunctionRenderer::WavefunctionRenderer()
{
	_W = 100;
	_H = 100;
	_grid = 64;
	_scale = 20;
	cam.theta = M_PI / 2.0;
	cam.phi = 0;
	raycast = &traceRayDensity;
	_iso = 0.1f;
	_color1 = {255, 255, 255, 255};
	_color2 = {255, 255, 255, 255};
	_mode = Mode::Density;
	_rate = 0.01;
	_window = nullptr;
}

WavefunctionRenderer::~WavefunctionRenderer()
{
	if (_window)
		delete _window;
}

void	WavefunctionRenderer::setResolution(int W, int H)
{
	_W = W;
	_H = H;
}

void	WavefunctionRenderer::setXResolution(int W)
{
	_W = W;
}

void	WavefunctionRenderer::setYResolution(int H)
{
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
	_mode = mode;
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

void	WavefunctionRenderer::setColors(Color color1, Color color2)
{
	_color1 = color1;
	_color2 = color2;
}

void	WavefunctionRenderer::setIsosurface(float iso)
{
	_iso = iso;
}

void	WavefunctionRenderer::setIsosurfaceRate(float rate)
{
	_rate = rate;
}