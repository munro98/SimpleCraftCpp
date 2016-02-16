#include <iostream>
#include "Map.h"

Map::Map()
{
	for (int x = 0; x < MAP_WIDTH; x++)
	{
		auto chunkX = mChunks.find(x);
		if (chunkX == mChunks.end())
		{
			std::unordered_map<int, Chunk*> newChunkX;
			mChunks.insert({ x,  newChunkX });

			chunkX = mChunks.find(x);
		}

		for (int z = 0; z < MAP_DEPTH; z++)
		{
			Chunk* chunk = new Chunk();
			chunk->update(x, z, nullptr, nullptr, nullptr, nullptr);

			chunkX->second.insert({ z,  chunk });
		}
	}
}

Map::~Map()
{
	for (auto it = mChunks.begin(); it != mChunks.end();) {
		auto chunkZ = it->second;
		for (auto it2 = chunkZ.begin(); it2 != chunkZ.end();) {
			delete it2->second;
			auto toErase = it2;
			++it2;
			chunkZ.erase(toErase);
		}
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

	int x = (int)((int)(playerX > 0.0f ? playerX : playerX - (float)CHUNK_WIDTH) / CHUNK_WIDTH);
	int z = (int)((int)(playerZ > 0.0f ? playerZ : playerZ - (float)CHUNK_DEPTH) / CHUNK_DEPTH);


	auto chunkX = mChunks.find(x);
	if (chunkX == mChunks.end())
	{
		std::unordered_map<int, Chunk*> newChunkX;
		mChunks.insert({ x,  newChunkX });

		chunkX = mChunks.find(x);
	}

	auto chunkZ = chunkX->second.find(z);
	if (chunkZ == chunkX->second.end())
	{
		Chunk* frontChunk = nullptr;
		Chunk* backChunk = nullptr;
		Chunk* leftChunk = nullptr;
		Chunk* rightChunk = nullptr;

		auto chunkXTemp = mChunks.find(x - 1);
		if (chunkXTemp != mChunks.end())
		{
			auto chunkZTemp = chunkXTemp->second.find(z);
			if (chunkZTemp != chunkXTemp->second.end())
			{
				leftChunk = chunkZTemp->second;
			}
		}

		chunkXTemp = mChunks.find(x + 1);
		if (chunkXTemp != mChunks.end())
		{
			auto chunkZTemp = chunkXTemp->second.find(z);
			if (chunkZTemp != chunkXTemp->second.end())
			{
				rightChunk = chunkZTemp->second;
			}
		}

		
		auto chunkZTemp = chunkX->second.find(z - 1);
		if (chunkZTemp != chunkX->second.end())
		{
			frontChunk = chunkZTemp->second;
		}
		
		chunkZTemp = chunkX->second.find(z + 1);
		if (chunkZTemp != chunkX->second.end())
		{
			backChunk = chunkZTemp->second;
		}


		Chunk* chunk = new Chunk();
		chunk->update(x, z, frontChunk, backChunk, leftChunk, rightChunk);
		chunkX->second.insert({ z,  chunk });

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
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			it2->second->render(modelLocation, blockPositionLocation);
		}
	}
}

void Map::rayCastBlock(RayCast& rayCast, glm::vec3 start, glm::vec3 forward)
{

	float playerX = start.x;
	float playerZ = start.z;

	int x = (int)((int)(playerX > 0.0f ? playerX : playerX - (float)CHUNK_WIDTH) / CHUNK_WIDTH);
	int z = (int)((int)(playerZ > 0.0f ? playerZ : playerZ - (float)CHUNK_DEPTH) / CHUNK_DEPTH);

	auto chunkX = mChunks.find(x);
	if (chunkX != mChunks.end())
	{
		auto chunkZ = chunkX->second.find(z);
		if (chunkZ != chunkX->second.end())
		{
			chunkZ->second->rayCastBlock(rayCast, start, forward);
		}
	}
}

void Map::rayCastBlockRemove(RayCast& rayCast, glm::vec3 start, glm::vec3 forward)
{

	float playerX = start.x;
	float playerZ = start.z;

	int x = (int)((int)(playerX > 0.0f ? playerX : playerX - (float)CHUNK_WIDTH) / CHUNK_WIDTH);
	int z = (int)((int)(playerZ > 0.0f ? playerZ : playerZ - (float)CHUNK_DEPTH) / CHUNK_DEPTH);

	auto chunkX = mChunks.find(x);
	if (chunkX != mChunks.end())
	{
		auto chunkZ = chunkX->second.find(z);
		if (chunkZ != chunkX->second.end())
		{
			chunkZ->second->rayCastBlockRemove(rayCast, start, forward);
		}
	}
}
