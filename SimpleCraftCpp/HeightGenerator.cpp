#include "HeightGenerator.h"
#include <cstdlib>
#include <cmath>
#include <iostream>



HeightGenerator::HeightGenerator()
{
}


HeightGenerator::~HeightGenerator()
{
}

int HeightGenerator::generateHeight(int x, int z)
{
	//return generateSmoothNoise(x, z);
	float value = generateInterpolatedNoise((float)x / 4.0f, (float)z / 4.0f) * AMPLITUDE;
	//std::cout << value << "\n";
	return (int)value;
}

float HeightGenerator::generateInterpolatedNoise(float x, float z)
{
	int intX = (int)x;
	int intZ = (int)z;
	float fracX = x - intX;
	float fracZ = z - intZ;

	float v1 = generateSmoothNoise(intX, intZ);
	float v2 = generateSmoothNoise(intX + 1, intZ);
	float v3 = generateSmoothNoise(intX, intZ + 1);
	float v4 = generateSmoothNoise(intX + 1, intZ + 1);

	float i1 = interpolate(v1, v2, fracX);
	float i2 = interpolate(v3, v4, fracX);
	return interpolate(i1, i2, fracZ);
}

float HeightGenerator::interpolate(float a, float b, float blend)
{
	double theta = blend * PI;
	float f = (1.0f - cos(theta)) * 0.5f;
	return a * (1.0f - f) + b * f;
}

//TODO: optimise this
float HeightGenerator::generateSmoothNoise(int x, int z)
{
	float corners = (generateNoise(x-1, z-1) + generateNoise(x+1, z-1) + generateNoise(x-1, z+1) + generateNoise(x+1, z+1)) / 16;
	//float sides = (generateNoise(x - 1, z) + generateNoise(x + 1, z) + generateNoise(x, z - 1) + generateNoise(x, z + 1)) / 8;
	float center = generateNoise(x, z) / 4;
	return corners + center;// + sides
}

float HeightGenerator::generateNoise(int x, int z)
{
	//return rand() % 100;
	
	//rng.seed(std::random_device()());
	static std::mt19937 rng;
	static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
	rng.seed(x * 49632 + z * 325176);
	//rng.seed(x * 496323 + z * 3251);
	

	//std::cout << dist(rng) << std::endl;
	float value = dist(rng);// *2.0f - 1.0f;
	//std::cout << value << "\n";
	return value;
}