#pragma once
#define PI 3.1415926535897932384626433832795 
#define AMPLITUDE 32.0f
#include <random>

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

	static int generateInterpolatedNoise2(int x, int z);

	static int interpolate2(int y1, int y2, int length, int x);

	static int generateNoise2(int x, int z);

	
};

