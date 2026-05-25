#include "WavefunctionRenderer.hpp"
#include <ctime>

static int	modetoint(Mode mode)
{
	if (mode == Mode::Density)
		return 0;
	if (mode == Mode::Scattering)
		return 1;
	if (mode == Mode::Surface)
		return 2;
}

static Mode	inttomode(int mode)
{
	if (mode == 0)
		return Mode::Density;
	if (mode == 1)
		return Mode::Scattering;
	if (mode == 2)
		return Mode::Surface;
}

bool	WavefunctionRenderer::save(std::string name)
{
	std::time_t	t = std::time(0);
	if (name == "")
		name = _title + "_" + std::to_string((int)t) + ".wfr";
	if (!(name.size() >= 4 && name.compare(name.size() - 4, 4, ".wfr") == 0))
		name = name + ".wfr";

	std::ofstream file(name);
	if (!file.is_open())
		return false;
	file << "title: " << _title << std::endl;
	file << "width: " << _W << std::endl;
	file << "height: " << _H << std::endl;
	file << "grid: " << _grid << std::endl;
	file << "scale: " << _scale << std::endl;
	file << "isosurface: " << _iso << std::endl;
	file << "color1: " << _color1.r << "," << _color1.g << "," << _color1.b << "," << _color1.a << std::endl;
	file << "color2: " << _color2.r << "," << _color2.g << "," << _color2.b << "," << _color2.a << std::endl;
	file << "theta: " << cam.theta << std::endl;
	file << "phi: " << cam.phi << std::endl;
	file << "mode: " << modetoint(_mode) << std::endl;
	file.close();
	return true;
}