#include "WavefunctionRenderer.hpp"

// Factorial
static float factorial(int n)
{
	float r = 1.0f;
	for (int i = 2; i <= n; i++) r *= i;
	return r;
}

// Laguerre
static float laguerre(int k, int a, float x)
{
	if (k == 0) return 1.0f;
	if (k == 1) return 1.0f + a - x;

	float L0 = 1.0f;
	float L1 = 1.0f + a - x;
	float L2 = 0.0f;

	for (int i = 2; i <= k; i++)
	{
		L2 = ((2*i - 1 + a - x) * L1 - (i - 1 + a) * L0) / i;
		L0 = L1;
		L1 = L2;
	}
	return L1;
}

// Legendre P_l^m(x)
template<int l, int m> float legendreP(float x)
{
	float pmm = 1.0f;

	if (m > 0)
	{
		float somx2 = std::sqrt((1.0f - x) * (1.0f + x));
		float fact = 1.0f;

		for (int i = 1; i <= m; i++)
		{
			pmm *= -fact * somx2;
			fact += 2.0f;
		}
	}

	if (l == m)
		return pmm;

	float pmmp1 = x * (2*m + 1) * pmm;
	if (l == m + 1)
		return pmmp1;

	float pll = 0.0f;
	float p0 = pmm;
	float p1 = pmmp1;

	for (int ll = m + 2; ll <= l; ll++)
	{
		pll = ((2*ll - 1)*x*p1 - (ll + m - 1)*p0) / (ll - m);
		p0 = p1;
		p1 = pll;
	}

	return p1;
}

// Spherical harmonic
template<int l, int m> std::complex<float> Ylm(float theta, float phi)
{
	constexpr int abs_m = (m < 0) ? -m : m;

	float norm = std::sqrt((2.0f*l + 1.0f)/(4.0f*M_PI) * factorial(l - abs_m) / factorial(l + abs_m));

	float P = legendreP<l, abs_m>(std::cos(theta));

	std::complex<float> angular = norm * P * std::exp(std::complex<float>(0.0f, m * phi));

	if (m < 0)
	{
		angular = (float)std::pow(-1.0f, abs_m) * std::conj(angular);
	}

	return angular;
}

// Hidrogen atom
template<int n, int l> float Rnl(float r)
{
	float rho = 2.0f * r / n;
	int k = n - l - 1;

	float norm =
		std::sqrt(std::pow(2.0f/n, 3.0f) * factorial(n - l - 1) / (2.0f * n * factorial(n + l)));

	return norm * std::exp(-r / n) * std::pow(rho, l) * laguerre(k, 2*l + 1, rho);
}

// cartesian to spherical
static void to_spherical(float x, float y, float z, float &r, float &theta, float &phi)
{
	r = std::sqrt(x*x + y*y + z*z);
	theta = (r == 0.0f) ? 0.0f : std::acos(z / r);
	phi = std::atan2(y, x);
}

// Hydrogen orbital
template<int n, int l, int m> std::complex<float> orbital(float x, float y, float z)
{
	float r, theta, phi;
	to_spherical(x, y, z, r, theta, phi);

	float R = Rnl<n, l>(r);
	std::complex<float> Y = Ylm<l, m>(theta, phi);

	return R * Y;
}