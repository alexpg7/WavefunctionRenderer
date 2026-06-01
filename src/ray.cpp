#include "WavefunctionRenderer.hpp"

static	Color lerpColor(const Color& c1, const Color& c2, float x)
{
	x = std::clamp(x, 0.0f, 1.0f);

	auto lerp = [x](std::uint8_t a, std::uint8_t b) -> std::uint8_t {
		return static_cast<std::uint8_t>(a + (b - a) * x);
	};

	return Color{lerp(c1.r, c2.r), lerp(c1.g, c2.g), lerp(c1.b, c2.b), lerp(c1.a, c2.a)};
}

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

Color	traceRayDensity(const Ray& r, const Volume& v, const CVolume&, float scale)
{
	float t = 0.0;
	float step = scale / 25.0;

	float alpha = 0.0f;
	float sigma = 0.3f;
	float maxDist = 2 * std::sqrt(3) * scale;
	while (t < maxDist)
	{
		float x = r.ox + r.dx * t;
		float y = r.oy + r.dy * t;
		float z = r.oz + r.dz * t;

		float d = std::sqrt(sampleVolume(v, x, y, z, scale) / v.max);

		float opacity = 1.0f - std::exp(-sigma * d * step);

		// front-to-back compositing
		alpha += (1.0f - alpha) * opacity;

		if (alpha > 0.995f)
			break;

		t += step;
	}

	//uint8_t c = (uint8_t)(acc * 255.0);

	Color color = lerpColor(v.color2, v.color1, alpha);
	color.a = (uint8_t)(255.0f * alpha);
	return color;
}

Color traceRayScattering(const Ray& r, const Volume& v, const CVolume&, float scale)
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
			Color c = lerpColor(v.color2, v.color1, intensity);
			c.a = (uint8_t)(255.0f * intensity);
			return c;
		}

		t += step;
	}
	return {0, 0, 0, 255};
}

Color traceRaySurface(const Ray& r, const Volume& v, const CVolume&, float scale)
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

	//uint8_t c = (uint8_t)(255.0f * opacity);

	//return {c, c, c, 255};

	Color color = lerpColor(v.color2, v.color1, opacity);
	color.a = (uint8_t)(255.0f * opacity);
	return color;
}