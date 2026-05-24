#include "WavefunctionRenderer.hpp"

int main()
{
	WavefunctionRenderer wave;

	auto psi = orbital<1,1,1>;
	wave.setResolution(400, 400);
	wave.setTitle("WavefunctionRenderer");
	wave.setWaveFunction(psi);
	wave.setGrid(64);
	wave.setScale(20);
	wave.setIsosurface(0.1);
	wave.setMode(Mode::Density);
	wave.show();
}