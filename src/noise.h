#pragma once

namespace KtnEm::Noise
{
	double Noise(int, int, int);
	double SmoothedNoise(int, int, int);
	double Interpolate(double, double, double);
	double InterpolatedNoise(int, double, double);
	double ValueNoise_2D(double, double);
    float GetNoiseAt(int32_t x, int32_t y);
}