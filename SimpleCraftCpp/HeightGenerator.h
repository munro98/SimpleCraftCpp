#pragma once
#define PI 3.1415926535897932384626433832795 
#define AMPLITUDE 16
#include <random>

class HeightGenerator
{
public:
	HeightGenerator();
	~HeightGenerator();
	static int generateHeight(int x, int z);
	static float generateInterpolatedNoise(float x, float z);
	static float interpolate(float a, float b, float blend);
	static float generateSmoothNoise(int x, int z);
	static float generateNoise(int x, int z);

	
};

