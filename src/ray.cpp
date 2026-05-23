#include "WavefunctionRenderer.hpp"

Ray	generateRay(int x, int y, int width, int height, double fov)
{
	double aspect = (double)width / (double)height;

	double px = (2.0 * ((x + 0.5) / (double)width) - 1.0) * aspect * std::tan(fov * 0.5);

	double py = (1.0 - 2.0 * ((y + 0.5) / (double)height)) * std::tan(fov * 0.5);

	Ray r;

	r.ox = 0.0;
	r.oy = 0.0;
	r.oz = -5.0;

	r.dx = px;
	r.dy = py;
	r.dz = 1.0;

	double norm = std::sqrt(r.dx*r.dx + r.dy*r.dy + r.dz*r.dz);

	r.dx /= norm;
	r.dy /= norm;
	r.dz /= norm;

	return r;
}

double sampleVolume(const Volume& v, double x, double y, double z)
{
	int ix = (int)(x + v.nx * 0.5);
	int iy = (int)(y + v.ny * 0.5);
	int iz = (int)(z + v.nz * 0.5);

	if (ix < 0 || ix >= v.nx || iy < 0 || iy >= v.ny || iz < 0 || iz >= v.nz)
		return 0.0;

	return v.data[ix + iy * v.nx + iz * v.nx * v.ny];
}

Color traceRay(const Ray& r, const Volume& v)
{
	double t = 0.0;
	double step = 0.05;

	double acc = 0.0;

	while (t < 10.0)
	{
		double x = r.ox + r.dx * t;
		double y = r.oy + r.dy * t;
		double z = r.oz + r.dz * t;

		double d = sampleVolume(v, x, y, z);

		acc += d * 0.01;

		if (acc > 1.0)
			acc = 1.0;

		t += step;
	}

	uint8_t c = (uint8_t)(acc * 255.0);

	return {c, c, c, 255};
}