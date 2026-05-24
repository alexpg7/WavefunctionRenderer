#include "WavefunctionRenderer.hpp"

std::complex<double> psi(double x, double y, double z) {
	double r = std::sqrt(x*x + y*y + z*z);

	if (r < 1e-12) return std::complex<double>(0.0, 0.0);

	double radial = std::exp(-r);
	double angular = z;
	double k = 1.0;
	std::complex<double> phase = std::exp(std::complex<double>(0.0, k * r));

	return angular * radial * phase;
}

int main()
{
	WavefunctionRenderer wave;

	wave.setResolution(500, 500);
	wave.setTitle("WavefunctionRenderer");
	wave.setWaveFunction(psi);
	wave.setGrid(128);
	wave.setScale(5);
	wave.show();
}