#include "WavefunctionRenderer.hpp"

std::complex<float> psi(float x, float y, float z) {
	float r = std::sqrt(x*x + y*y + z*z);

	if (r < 1e-12) return std::complex<float>(0.0, 0.0);

	float radial = std::exp(-r);
	float angular = z;
	float k = 1.0;
	std::complex<float> phase = std::exp(std::complex<float>(0.0, k * r));

	return angular * radial * phase;
}

int main()
{
	WavefunctionRenderer wave;

	wave.setResolution(400, 400);
	wave.setTitle("WavefunctionRenderer");
	wave.setWaveFunction(psi);
	wave.setGrid(128);
	wave.setScale(7);
	wave.show();
}