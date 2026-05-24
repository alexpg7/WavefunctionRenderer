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
	int voxels;
	std::vector<float> data;

	Volume(int vox): voxels(vox), data(vox * vox * vox, 0.0)
	{
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
	float	fov;
};

struct Ray
{
	float ox, oy, oz;
	float dx, dy, dz;
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

class WavefunctionRenderer
{
	private:
		int	_W;
		int	_H;
		std::string	_title = "";
		void	buildVolume();
		void	paintScreen(Framebuffer& fb);
		void	handleCameraInput(Camera& cam, Framebuffer& fb);
		bool	_dragging = false;
		sf::Vector2i	_lastMouse;
		sf::RenderWindow *_window;
		float	_dt;
		Volume	_volume = Volume(64);
		unsigned int	_grid;
		std::function<std::complex<float>(float, float, float)> _psi = nullptr;
		float	_scale;
	public:
		WavefunctionRenderer();
		~WavefunctionRenderer();
		Camera	cam;
		void	setResolution(int W, int H);
		void	setTitle(std::string title);
		void	setWaveFunction(const std::function<std::complex<float>(float, float, float)>& psi);
		void	setGrid(unsigned int x);
		void	setScale(float scale);
		void	show();
};

//aux functions
Ray	generateRay(int x, int y, int width, int height, float scale, Camera& cam);
Color	traceRay(const Ray& r, const Volume& v, float scale);
Color	traceRay2(const Ray& r, const Volume& v, float scale);
float	sampleVolume(const Volume& v, float x, float y, float z, float scale);