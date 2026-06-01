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
	if (W <= 0 || H <= 0)
		throw SetterException("Resolution must be positive an non-zero");
	_W = W;
	_H = H;
}

void	WavefunctionRenderer::setXResolution(int W)
{
	if (W <= 0)
		throw SetterException("Resolution must be positive an non-zero");
	_W = W;
}

void	WavefunctionRenderer::setYResolution(int H)
{
	if (H <= 0)
		throw SetterException("Resolution must be positive an non-zero");
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
	if (x <= 0)
		throw SetterException("Grid must be positive an non-zero");
	_grid = x;
	if (_mode == Mode::Wave)
		_cvolume = CVolume(x);
	else
		_volume = Volume(x);
}

void	WavefunctionRenderer::setScale(float scale)
{
	if (scale <= 0)
		throw SetterException("Scale must be positive an non-zero");
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
		case Mode::Wave:
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

void	WavefunctionRenderer::setColor1(Color color1)
{
	_color1 = color1;
}

void	WavefunctionRenderer::setColor2(Color color2)
{
	_color2 = color2;
}

void	WavefunctionRenderer::setAngles(float theta, float phi)
{
	cam.theta = theta;
	cam.phi = phi;
}

void	WavefunctionRenderer::setTheta(float theta)
{
	cam.theta = theta;
}

void	WavefunctionRenderer::setPhi(float phi)
{
	cam.phi= phi;
}

void	WavefunctionRenderer::setIsosurface(float iso)
{
	if (iso <= 0)
		throw SetterException("Isosurface must be positive an non-zero");
	_iso = iso;
}

void	WavefunctionRenderer::setIsosurfaceRate(float rate)
{
	_rate = rate;
}