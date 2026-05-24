#include "WavefunctionRenderer.hpp"

Ray generateRay(int x, int y, int width, int height, float scale, Camera& cam)
{
	Ray r;

	float py = (((float)x / width) - 0.5) * scale;

	float px = (((float)y / height) - 0.5) * scale;

	float ox = px;
	float oy = py;
	float oz = std::sqrt(3) * scale;

	float	st = std::sin(cam.theta);
	float	ct = std::cos(cam.theta);
	float	sp = std::sin(cam.phi);
	float	cp = std::cos(cam.phi);

	r.ox = ox * cp * ct - oy * sp + oz * cp * st;
	r.oy = ox * sp * ct + oy * cp + oz * sp * st;
	r.oz = -ox * st + oz * ct;

	r.dx = -cp * st;
	r.dy = -sp * st;
	r.dz = -ct;

	return r;
}

float sampleVolume(const Volume& v, float x, float y, float z, float scale)
{
	int ix = (int)(((x / scale) + 0.5) * v.voxels + 0.5);

	int iy = (int)(((y / scale) + 0.5) * v.voxels + 0.5);

	int iz = (int)(((z / scale) + 0.5) * v.voxels + 0.5);

	if (ix < 0 || ix >= v.voxels || iy < 0 || iy >= v.voxels || iz < 0 || iz >= v.voxels)
		return 0.0;

	return v.data[ix + iy * v.voxels+ iz * v.voxels * v.voxels];
}

Color	traceRayDensity(const Ray& r, const Volume& v, float scale)
{
	float t = 0.0;
	float step = scale / 25.0;

	float acc = 0.0;
	float max = 2 * std::sqrt(3) * scale;
	while (t < max)
	{
		float x = r.ox + r.dx * t;
		float y = r.oy + r.dy * t;
		float z = r.oz + r.dz * t;

		float d = sampleVolume(v, x, y, z, scale) / v.max;

		acc += d * 0.15 * step;

		if (acc > 1.0)
		{
			acc = 1.0;
			break;
		}

		t += step;
	}

	uint8_t c = (uint8_t)(acc * 255.0);

	return {c, c, c, 255};
}

Color traceRayScattering(const Ray& r, const Volume& v, float scale)
{
	float t = 0.0f;
	float step = scale / 10.0f;

	float maxDist = 2.0f * std::sqrt(3.0f) * scale;
	float densityScale = 0.4f * 5.0f / scale;

	while (t < maxDist)
	{
		float x = r.ox + r.dx * t;
		float y = r.oy + r.dy * t;
		float z = r.oz + r.dz * t;

		float d = sampleVolume(v, x, y, z, scale) / v.max;
		float sigma = d * densityScale * 100.0f;
		float p = 1.0f - expf(-sigma * step);

		if (p > 1.0f) p = 1.0f;

		float rnd = (float)rand() / (float)RAND_MAX;

		if (rnd < p)
		{
			float intensity = 1.0f - expf(-d * 20.0f);
			uint8_t c = (uint8_t)(255.0f * intensity);
			return {c, c, c, 255};
		}

		t += step;
	}
	return {0, 0, 0, 255};
}

Color traceRaySurface(const Ray& r, const Volume& v, float scale)
{
	float t = 0.0f;

	float step = scale / 30.0f;
	float maxDist = 2.0f * std::sqrt(3.0f) * scale;
	float iso = v.iso;
	float opacity = 0.0f;
	float lastD = 0.0f;

	while (t < maxDist)
	{
		float x = r.ox + r.dx * t;
		float y = r.oy + r.dy * t;
		float z = r.oz + r.dz * t;

		float d = sampleVolume(v, x, y, z, scale);

		if ((d > iso) && (lastD < iso))
		{
			opacity += 0.3;
		}
		lastD = d;
		/*float dist = fabs(d - iso);
		float w = 0.1 * expf(-(dist * dist) / (2.0f * 0.02f * 0.02f));

		opacity += w * step;*/

		t += step;
	}

	if (opacity > 1.0f)
		opacity = 1.0f;

	uint8_t c = (uint8_t)(255.0f * opacity);

	return {c, c, c, 255};
}