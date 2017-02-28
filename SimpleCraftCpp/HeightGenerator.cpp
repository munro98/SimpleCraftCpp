#include "HeightGenerator.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "Hasher.h"


HeightGenerator::HeightGenerator()
{
}

HeightGenerator::~HeightGenerator()
{
}

int HeightGenerator::generateHeight(int x, int z)
{
	//return generateSmoothNoise(x, z);
	float value = generateInterpolatedNoise((float)x / 16.0f, (float)z / 16.0f) * AMPLITUDE;
	//float value = generateSmoothNoise((float)x / 16.0f, (float)z / 16.0f) * AMPLITUDE;
	//

	//int value = generateInterpolatedNoise2(x, z);
	//std::cout << value << "\n";
	return value;
}

float HeightGenerator::generateInterpolatedNoise(float x, float z)
{
	int intX = (int)std::floor(x);
	int intZ = (int)std::floor(z);
	float fracX = x - intX;
	float fracZ = z - intZ;

	float v1 = generateSmoothNoise(intX, intZ);
	float v2 = generateSmoothNoise(intX + 1, intZ);
	float v3 = generateSmoothNoise(intX, intZ + 1);
	float v4 = generateSmoothNoise(intX + 1, intZ + 1);

	float i1 = interpolate(v1, v2, fracX);
	float i2 = interpolate(v3, v4, fracX);
	return interpolate(i1, i2, fracZ);

	//float i1 = lerp(v1, v2, fracX);
	//float i2 = lerp(v3, v4, fracX);
	//return lerp(i1, i2, fracZ);
}

float HeightGenerator::interpolate(float a, float b, float blend)
{
	double theta = blend * PI;
	float f = (1.0f - cos(theta)) * 0.5f;
	return a * (1.0f - f) + b * f;
}

float HeightGenerator::lerp(float a, float b, float f)
{
	return a + f * (b - a);
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
	//rng.seed(Hasher::hash(x,z));
	

	//std::cout << dist(rng) << std::endl;
	float value = dist(rng);// *2.0f - 1.0f;
	//std::cout << value << "\n";
	return value;
}

//////////////////////////////////////////////////////

int HeightGenerator::generateInterpolatedNoise2(int x, int z)
{
	int intX = std::floor(x)/8;
	int intZ = std::floor(z)/8;
	int fracX = x - x % 8;
	int fracZ = z - z % 8;

	int v1 = generateNoise2(intX, intZ);
	int v2 = generateNoise2(intX + 1, intZ);
	int v3 = generateNoise2(intX, intZ + 1);
	int v4 = generateNoise2(intX + 1, intZ + 1);

	int i1 = interpolate2(v1, v2, 8, fracX);
	int i2 = interpolate2(v3, v4, 8, fracX);
	return interpolate2(i1, i2, 8, fracZ);
}

int HeightGenerator::interpolate2(int y1, int y2, int length, int x)
{
	int v = (int)(y2 - cos(x * PI)) / 2;
	return (y1 + x * (y2 - y1) / (length - 1)) * v;
}

int HeightGenerator::generateNoise2(int x, int z)
{
	//return rand() % 100;

	//rng.seed(std::random_device()());
	static std::mt19937 rng;
	static std::uniform_int_distribution<int> dist(0, 6);
	rng.seed(x * 49632 + z * 325176);
	//rng.seed(x * 496323 + z * 3251);


	//std::cout << dist(rng) << std::endl;
	float value = dist(rng);// *2.0f - 1.0f;
							//std::cout << value << "\n";
	return value;
}



