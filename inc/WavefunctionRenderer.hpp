#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>
#include <functional>
#include <algorithm>
#include <complex>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <string>
#include <fstream>

struct Color
{
	std::uint8_t r, g, b, a;
};

struct CVolume
{
	int	voxels = 0;
	float	max = 0;
	float	min = 0;
	Color	color1 = {0,0,0,0};
	Color	color2 = {0,0,0,0};
	std::vector<std::complex<float>> data;

	CVolume(int vox): voxels(vox), data(vox * vox * vox, 0.0)
	{
		if (vox <= 0)
			throw std::exception();
	}

	std::complex<float>& at(int x, int y, int z)
	{
		return data[x + y * voxels + z * voxels * voxels];
	}
};

struct Volume
{
	int	voxels = 0;
	float	iso = 0;
	float	max = 0;
	float	min = 0;
	Color	color1 = {0,0,0,0};
	Color	color2 = {0,0,0,0};
	std::vector<float> data;

	Volume(int vox): voxels(vox), data(vox * vox * vox, 0.0)
	{
		if (vox <= 0)
			throw std::exception();
	}

	float& at(int x, int y, int z)
	{
		return data[x + y * voxels + z * voxels * voxels];
	}
};

struct Camera
{
	float	theta;
	float	phi;
};

struct Ray
{
	float ox, oy, oz;
	float dx, dy, dz;
};

enum class Mode {
	Density, Scattering, Surface, Wave
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

class WavefunctionRenderer
{
	private:
		//parameters
		std::string	_title = "";
		int	_W;
		int	_H;
		unsigned int	_grid;
		float	_scale;
		float	_iso;
		float	_rate;
		Color	_color1;
		Color	_color2;
		Mode	_mode;
		Volume	_volume = Volume(64);
		CVolume	_cvolume = CVolume(64);

		// aux variables
		bool	_dragging = false;
		bool	_zooming = false;
		bool	_ctrl = false;
		sf::Vector2i	_lastMouse;
		sf::RenderWindow *_window;

		// customizable functions
		std::function<std::complex<float>(float, float, float)> _psi = nullptr;
		std::function<Color(const Ray&, const Volume&, const CVolume&, float)> raycast = nullptr;

		// private methods
		void	buildVolume();
		void	buildCVolume();
		void	paintScreen(Framebuffer& fb);
		void	handleCameraInput(Camera& cam, Framebuffer& fb);
		void	handleKeys(sf::Event& event, bool& pressS, bool& pressI, bool& pressP);
	public:
		WavefunctionRenderer();
		~WavefunctionRenderer();

		// camera (save angles)
		Camera	cam;

		void	setResolution(int W, int H);
		void	setXResolution(int W);
		void	setYResolution(int H);
		void	setTitle(std::string title);
		void	setWaveFunction(const std::function<std::complex<float>(float, float, float)>& psi);
		void	setGrid(unsigned int x);
		void	setScale(float scale);
		void	setMode(Mode mode);
		void	setIsosurface(float iso);
		void	setIsosurfaceRate(float rate);
		void	setColors(Color color1, Color color2);
		void	setColor1(Color color1);
		void	setColor2(Color color2);
		void	setAngles(float theta, float phi);
		void	setTheta(float theta);
		void	setPhi(float phi);
		void	show();

		// saving/loading stats
		bool	save(std::string name);
		bool	load(std::string name);
		void	instructions();
		void	stats();
	class SetterException : public std::exception
	{
		private:
			std::string message;

		public:
			explicit SetterException(const std::string& msg)
				: message(msg) {}

			const char* what() const noexcept override {
				return message.c_str();
			}
	};
};

//aux functions
Ray	generateRay(int x, int y, int width, int height, float scale, Camera& cam);
Color	traceRayDensity(const Ray& r, const Volume& v, const CVolume& cv, float scale);
Color	traceRayScattering(const Ray& r, const Volume& v, const CVolume& cv, float scale);
Color	traceRaySurface(const Ray& r, const Volume& v, const CVolume& cv, float scale);
Color	traceRayWave(const Ray& r, const Volume& v, const CVolume& cv, float scale);
float	sampleVolume(const Volume& v, float x, float y, float z, float scale);

//orbital
#include "orbital.tpp"