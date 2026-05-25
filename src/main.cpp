#include "WavefunctionRenderer.hpp"

int main()
{
	WavefunctionRenderer wave;

	wave.setResolution(400, 400);
	wave.setTitle("WavefunctionRenderer");
	wave.setGrid(128);
	wave.setScale(20);
	wave.setIsosurface(0.1);
	wave.setMode(Mode::Surface);
	wave.setColors({0, 255, 129, 255}, {231, 0, 255, 255});
	//wave.setColors({255, 255, 255, 255}, {255, 255, 255, 255});
	auto psi = orbital<1,1,1>;
	wave.setWaveFunction(psi);
	wave.show();
	auto psi2 = orbital<3,1,0>;
	wave.setWaveFunction(psi2);
	wave.show();
	auto psi3 = orbital<3,1,1>;
	wave.setWaveFunction(psi3);
	wave.show();
	auto psi4 = orbital<3,2,0>;
	wave.setWaveFunction(psi4);
	wave.show();
	auto psi5 = orbital<3,2,1>;
	wave.setWaveFunction(psi5);
	wave.show();
}