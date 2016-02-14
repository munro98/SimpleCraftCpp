#include <iostream>
#include "Map.h"

Map::Map()
{
	for (int64_t x = 0; x < MAP_WIDTH; x++)
	{
		for (int64_t z = 0; z < MAP_DEPTH; z++)
		{
			int64_t key = (x + 2147483647) << 32 | z + 2147483647;
			Chunk* chunk = new Chunk();
			chunk->update(x, z, nullptr, nullptr, nullptr, nullptr);
			mChunks.insert({ key,  chunk });
		}
	}
}

Map::~Map()
{
	for (auto it = mChunks.begin(); it != mChunks.end();) {
		delete it->second;
		auto toErase = it;
		++it;
		mChunks.erase(toErase);
	}
}

void Map::initialize()
{
	
}

void Map::update(float playerX, float playerZ)
{
	int64_t chunkX = (int64_t)((int64_t)(playerX > 0.0f ? playerX : playerX - (float)CHUNK_WIDTH) / CHUNK_WIDTH);
	int64_t chunkZ = (int64_t)((int64_t)(playerZ > 0.0f ? playerZ : playerZ - (float)CHUNK_DEPTH) / CHUNK_DEPTH);

	int64_t key = (chunkX + 2147483647) << 32 | (chunkZ + 2147483647);

	auto result = mChunks.find(key);
	if (result == mChunks.end())
	{

		Chunk* frontChunk = nullptr;
		Chunk* backChunk = nullptr;
		Chunk* leftChunk = nullptr;
		Chunk* rightChunk = nullptr;


		//Left chunk
		int64_t key2 = ((int64_t)(chunkX - 1) + 2147483647) << 32 | ((int64_t)(chunkZ)+2147483647);
		auto result = mChunks.find(key2);
		if (result != mChunks.end())
		{
			leftChunk = result->second;
		}
		//Right chunk
		key2 = ((int64_t)(chunkX + 1) + 2147483647) << 32 | ((int64_t)(chunkZ)+2147483647);
		result = mChunks.find(key2);
		if (result != mChunks.end())
		{
			rightChunk = result->second;
		}
		//Front chunk
		key2 = ((int64_t)(chunkX)+2147483647) << 32 | ((int64_t)(chunkZ - 1) + 2147483647);
		result = mChunks.find(key2);
		if (result != mChunks.end())
		{
			frontChunk = result->second;
		}
		//Back chunk
		key2 = ((int64_t)(chunkX)+2147483647) << 32 | ((int64_t)(chunkZ + 1) + 2147483647);
		result = mChunks.find(key2);
		if (result != mChunks.end())
		{
			backChunk = result->second;
		}

		Chunk* chunk = new Chunk();
		chunk->update(chunkX, chunkZ, frontChunk, backChunk, leftChunk, rightChunk);
		mChunks.insert({ key,  chunk });
		std::cout << "insert " << chunkX << " " << chunkZ << " " << chunkX + chunkZ * MAP_WIDTH  << std::endl;

		if (frontChunk != nullptr)
		{
			frontChunk->updateBack(chunk);
		}

		if (backChunk != nullptr)
		{

			backChunk->updateFront(chunk);
		}

		if (leftChunk != nullptr)
		{
			leftChunk->updateRight(chunk);
		}

		if (rightChunk != nullptr)
		{
			rightChunk->updateLeft(chunk);
		}
	} 
}

void Map::render(GLuint modelLocation, GLuint blockPositionLocation, float playerX, float playerZ)
{
	for (auto it = mChunks.begin(); it != mChunks.end(); ++it) {
		it->second->render(modelLocation, blockPositionLocation);
	}
}

void Map::rayCastBlock(RayCast& rayCast, glm::vec3 start, glm::vec3 forward)
{

	float playerX = start.x;
	float playerZ = start.z;

	int64_t chunkX = (int64_t)((int64_t)(playerX > 0.0f ? playerX : playerX - (float)CHUNK_WIDTH) / CHUNK_WIDTH);
	int64_t chunkZ = (int64_t)((int64_t)(playerZ > 0.0f ? playerZ : playerZ - (float)CHUNK_DEPTH) / CHUNK_DEPTH);

	int64_t key = (chunkX + 2147483647) << 32 | (chunkZ + 2147483647);
	auto result = mChunks.find(key);
	if (result != mChunks.end())
	{
		result->second->rayCastBlock(rayCast, start, forward);
	}
}

void Map::rayCastBlockRemove(RayCast& rayCast, glm::vec3 start, glm::vec3 forward)
{

	float playerX = start.x;
	float playerZ = start.z;

	int64_t chunkX = (int64_t)((int64_t)(playerX > 0.0f ? playerX : playerX - (float)CHUNK_WIDTH) / CHUNK_WIDTH);
	int64_t chunkZ = (int64_t)((int64_t)(playerZ > 0.0f ? playerZ : playerZ - (float)CHUNK_DEPTH) / CHUNK_DEPTH);

	int64_t key = (chunkX + 2147483647) << 32 | (chunkZ + 2147483647);
	auto result = mChunks.find(key);
	if (result != mChunks.end())
	{
		result->second->rayCastBlockRemove(rayCast, start, forward);
	}
}