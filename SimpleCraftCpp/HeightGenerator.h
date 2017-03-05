#pragma once
#define AMPLITUDE 6.0f
#include <random>

#include "Math.h"

class HeightGenerator
{
public:
	HeightGenerator();
	~HeightGenerator();

	static int generateHeight(int x, int z);
	static float generateInterpolatedNoise(float x, float z);
	static float interpolate(float a, float b, float blend);
	static float lerp(float a, float b, float f);
	static float generateSmoothNoise(int x, int z);
	static float generateNoise(int x, int z);

	static float generateNoise2(int x, int z);

};

