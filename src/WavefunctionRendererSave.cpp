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
	return 2;
}

static std::string	modetostr(Mode mode)
{
	if (mode == Mode::Density)
		return std::string("Density");
	if (mode == Mode::Scattering)
		return std::string("Scattering");
	if (mode == Mode::Surface)
		return std::string("Surface");
	return std::string("Surface");
}

/*static Mode	inttomode(int mode)
{
	if (mode == 0)
		return Mode::Density;
	if (mode == 1)
		return Mode::Scattering;
	if (mode == 2)
		return Mode::Surface;
	return Mode::Surface;
}*/

void	WavefunctionRenderer::instructions()
{
	std::cout << "WavefunctionRenderer instructions:" << std::endl;
	std::cout << "\t-Mouse drag: Rotate perspective" << std::endl;
	std::cout << "\t-Mouse scroll: Zoom in/out" << std::endl;
	std::cout << "\t-Ctrl + mouse scroll: change isosurface by the fixed rate" << std::endl;
	std::cout << "\t-Ctrl + S: Save stats in a .wfr file" << std::endl;
	std::cout << "\t-Ctrl + P: Print instructions" << std::endl;
	std::cout << "\t-Ctrl + I: Print stats" << std::endl;
}
void	WavefunctionRenderer::stats()
{
	std::cout << "Title: " << _title << std::endl;
	std::cout << "Window width: " << _W << " px" << std::endl;
	std::cout << "Window height: " << _H << " px" << std::endl;
	std::cout << "Grid: " << _grid << " voxels" << std::endl;
	std::cout << "Scale (zoom): " << _scale << std::endl;
	std::cout << "Isosurface: " << _iso << std::endl;
	std::cout << "Isosurface change rate: " << _rate * 100 << "%" << std::endl;
	std::cout << "Primary color: {" << (int)_color1.r << ", " << (int)_color1.g << ", " << (int)_color1.b << ", " << (int)_color1.a << "}" << std::endl;
	std::cout << "Secondary color: {" << (int)_color2.r << ", " << (int)_color2.g << ", " << (int)_color2.b << ", " << (int)_color2.a << "}" << std::endl;
	std::cout << "Theta: " << cam.theta << " rad" << std::endl;
	std::cout << "Phi: " << cam.phi << " rad" << std::endl;
	std::cout << "Mode: " << modetostr(_mode) << std::endl;
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
	file << "rate: " << _rate << std::endl;
	file << "color1: " << (int)_color1.r << "," << (int)_color1.g << "," << (int)_color1.b << "," << (int)_color1.a << std::endl;
	file << "color2: " << (int)_color2.r << "," << (int)_color2.g << "," << (int)_color2.b << "," << (int)_color2.a << std::endl;
	file << "theta: " << cam.theta << std::endl;
	file << "phi: " << cam.phi << std::endl;
	file << "mode: " << modetoint(_mode) << std::endl;
	file.close();
	return true;
}

/*static bool	parse_line(WavefunctionRenderer& wave, std::string line)
{

}*/

bool	WavefunctionRenderer::load(std::string name)
{
	std::ifstream file(name);
	if (!file.is_open())
		return false;
	std::string line;
	while (std::getline(file, line))
	{
		//parse_line(*this, line);
	}
	file.close();
	return true;
}