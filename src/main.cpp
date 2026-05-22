#include "WavefunctionRenderer.hpp"

std::complex<double> psi(double x, double y, double z)
{
	double r2 = x*x + y*y + z*z;
	return std::pow(2.0 / M_PI, 3.0 / 4.0) * std::exp(-r2);
}

int main()
{
	WavefunctionRenderer wave;

	wave.setResolution(800, 600);
	wave.setTitle("WavefunctionRenderer");
	wave.setWaveFunction(psi);
	wave.setGrid(64);
	wave.setScale(0.1);
	wave.show();
}