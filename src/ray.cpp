#include "WavefunctionRenderer.hpp"

Ray generateRay(int x, int y, int width, int height, double scale, Camera& cam)
{
	Ray r;

	double py = (((double)x / width) - 0.5) * scale;

	double px = (((double)y / height) - 0.5) * scale;

	double ox = px;
	double oy = py;
	double oz = std::sqrt(3) * scale;

	double	st = std::sin(cam.theta);
	double	ct = std::cos(cam.theta);
	double	sp = std::sin(cam.phi);
	double	cp = std::cos(cam.phi);

	r.ox = ox * cp * ct - oy * sp + oz * cp * st;
	r.oy = ox * sp * ct + oy * cp + oz * sp * st;
	r.oz = -ox * st + oz * ct;

	r.dx = -cp * st;
	r.dy = -sp * st;
	r.dz = -ct;

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

		if (acc > 1.0)
			acc = 1.0;

		t += step;
	}

	uint8_t c = (uint8_t)(acc * 255.0);

	return {c, c, c, 255};
}