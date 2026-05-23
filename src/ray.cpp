#include "WavefunctionRenderer.hpp"

Ray generateRay(int x, int y, int width, int height, double scale, Camera& cam)
{
	Ray r;

	double py = (((double)x / width) - 0.5) * scale;

	double pz = (0.5 - ((double)y / height)) * scale;

	r.ox = -scale;
	r.oy = py;
	r.oz = pz;

	r.dx = 1.0;
	r.dy = 0.0;
	r.dz = 0.0;

	return r;
}

double sampleVolume(const Volume& v, double x, double y, double z, double scale)
{
	int ix = (int)(((x / scale) + 0.5) * v.voxels);

	int iy = (int)(((y / scale) + 0.5) * v.voxels);

	int iz = (int)(((z / scale) + 0.5) * v.voxels);

	if (ix < 0 || ix >= v.voxels || iy < 0 || iy >= v.voxels || iz < 0 || iz >= v.voxels)
		return 0.0;

	return v.data[ix + iy * v.voxels+ iz * v.voxels * v.voxels];
}

Color traceRay(const Ray& r, const Volume& v, double scale)
{
	double t = 0.0;
	double step = 0.05;

	double acc = 0.0;

	while (t < 10.0)
	{
		double x = r.ox + r.dx * t;
		double y = r.oy + r.dy * t;
		double z = r.oz + r.dz * t;

		double d = sampleVolume(v, x, y, z, scale);

		acc += d * 0.08;
		//(1.0 - exp(-d * 2.0)) * step * 0.1;

		if (acc > 1.0)
			acc = 1.0;

		t += step;
	}

	uint8_t c = (uint8_t)(acc * 255.0);

	return {c, c, c, 255};
}