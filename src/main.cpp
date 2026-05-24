#include "WavefunctionRenderer.hpp"

int main()
{
	WavefunctionRenderer wave;

	wave.setResolution(400, 400);
	wave.setTitle("WavefunctionRenderer");
	wave.setGrid(64);
	wave.setScale(20);
	wave.setIsosurface(0.1);
	wave.setMode(Mode::Density);
	auto psi = orbital<1,1,1>;
	wave.setWaveFunction(psi);
	wave.show();
	/*auto psi2 = orbital<2,1,1>;
	wave.setWaveFunction(psi2);
	wave.show();
	auto psi3 = orbital<3,1,1>;
	wave.setWaveFunction(psi3);
	wave.show();
	auto psi4 = orbital<4,1,1>;
	wave.setWaveFunction(psi4);
	wave.show();
	auto psi5 = orbital<5,1,1>;
	wave.setWaveFunction(psi5);
	wave.show();*/
}