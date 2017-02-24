#include <iostream>
#include "Map.h"

Map::Map()
{
	/*
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
	*/
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

	//int x = (int)((int)(playerX > 0.0f ? playerX : playerX - (float)CHUNK_WIDTH) / (float)CHUNK_WIDTH);
	//int z = (int)((int)(playerZ > 0.0f ? playerZ : playerZ - (float)CHUNK_DEPTH) / (float)CHUNK_DEPTH);

	int x = (int)((int)(playerX > 0.0f ? playerX + 0.5f : (playerX + 0.5f) - (float)CHUNK_WIDTH) / CHUNK_WIDTH);
	int z = (int)((int)(playerZ > 0.0f ? playerZ + 0.5f : (playerZ + 0.5f) - (float)CHUNK_DEPTH) / CHUNK_DEPTH);

	//for (int xx = -1; xx <= 1; xx++)
	{
		//for (int zz = -1; zz <= 1; zz++)
		{
			updateChunk(x, z);
			//updateChunk(x + xx, z + zz);
		}
	}

}

void Map::updateChunk(int x, int z) {
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


		Chunk* chunk = new Chunk(x, z, frontChunk, backChunk, leftChunk, rightChunk);
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

void Map::render(float playerX, float playerZ)
{
	for (auto it = mChunks.begin(); it != mChunks.end(); ++it) {
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			it2->second->render();
		}
	}
}

void Map::rayCastBlock(glm::vec3 start, glm::vec3 forward)
{

	float playerX = start.x;
	float playerZ = start.z;

	//int x = (int)((int)(playerX > 0.0f ? playerX : playerX - (float)CHUNK_WIDTH) / CHUNK_WIDTH);
	//int z = (int)((int)(playerZ > 0.0f ? playerZ : playerZ - (float)CHUNK_DEPTH) / CHUNK_DEPTH);

	int x = (int)((int)(playerX > 0.0f ? playerX + 0.5f : (playerX + 0.5f) - (float)CHUNK_WIDTH) / CHUNK_WIDTH);
	int z = (int)((int)(playerZ > 0.0f ? playerZ + 0.5f : (playerZ + 0.5f) - (float)CHUNK_DEPTH) / CHUNK_DEPTH);


	int blockPos[3] = { -1, -1, -1 };

	Chunk* hitChunk = nullptr;

	auto chunkX = mChunks.find(x);
	if (chunkX != mChunks.end())
	{
		auto chunkZ = chunkX->second.find(z);
		if (chunkZ != chunkX->second.end())
		{
			hitChunk = chunkZ->second;
			hitChunk->rayCastBlock(start, forward, blockPos);
		}
		else
		{
			return;
		}
	}

	std::cout << blockPos[0] << " " << blockPos[1] << " " << blockPos[2] << std::endl;


	if (blockPos[2] == 0)
	{
		auto chunkZ = chunkX->second.find(z - 1);
		if (chunkZ != chunkX->second.end())
		{
			chunkZ->second->updateBlockBack(hitChunk, blockPos[0], blockPos[1]);

			hitChunk->updateBlockFront(chunkZ->second, blockPos[0], blockPos[1]);
		}
	}

	else if (blockPos[2] == CHUNK_DEPTH - 1)
	{
		auto chunkZ = chunkX->second.find(z + 1);
		if (chunkZ != chunkX->second.end())
		{

			chunkZ->second->updateBlockFront(hitChunk, blockPos[0], blockPos[1]);

			hitChunk->updateBlockBack(chunkZ->second, blockPos[0], blockPos[1]);
		}
	}

	if (blockPos[0] == 0)
	{
		chunkX = mChunks.find(x - 1);
		if (chunkX != mChunks.end())
		{
			auto chunkZ = chunkX->second.find(z);
			if (chunkZ != chunkX->second.end())
			{
				chunkZ->second->updateBlockRight(hitChunk, blockPos[1], blockPos[2]);

				hitChunk->updateBlockLeft(chunkZ->second, blockPos[1], blockPos[2]);
			}
		}
	}

	else if (blockPos[0] == CHUNK_WIDTH - 1)
	{
		chunkX = mChunks.find(x + 1);
		if (chunkX != mChunks.end())
		{
			auto chunkZ = chunkX->second.find(z);
			if (chunkZ != chunkX->second.end())
			{

				chunkZ->second->updateBlockLeft(hitChunk, blockPos[1], blockPos[2]);

				hitChunk->updateBlockRight(chunkZ->second, blockPos[1], blockPos[2]);
			}
		}
	}

	hitChunk->updateMesh();
}

void Map::rayCastBlockRemove(glm::vec3 start, glm::vec3 forward)
{

	float playerX = start.x;
	float playerZ = start.z;

	//int x = (int)((int)(playerX > 0.0f ? playerX : playerX - (float)CHUNK_WIDTH) / CHUNK_WIDTH);
	//int z = (int)((int)(playerZ > 0.0f ? playerZ : playerZ - (float)CHUNK_DEPTH) / CHUNK_DEPTH);
	
	int x = (int)((int)(playerX > 0.0f ? playerX + 0.5f : (playerX + 0.5f) - (float)CHUNK_WIDTH) / CHUNK_WIDTH);
	int z = (int)((int)(playerZ > 0.0f ? playerZ + 0.5f : (playerZ + 0.5f) - (float)CHUNK_DEPTH) / CHUNK_DEPTH);
	int blockPos[3] = { -1, -1, -1 };
	//std::cout << z << " " << std::endl;
	Chunk* hitChunk = nullptr;
	auto chunkX = mChunks.find(x);
	if (chunkX != mChunks.end())
	{
		auto chunkZ = chunkX->second.find(z);
		if (chunkZ != chunkX->second.end())
		{
			hitChunk = chunkZ->second;
			chunkZ->second->rayCastBlockRemove(start, forward, blockPos);
		}
		else
		{
			return;
		}
	}

	//std::cout << blockPos[0] << " " << blockPos[1] << " " << blockPos[2] << std::endl;
	if (blockPos[2] == 0)
	{
		auto chunkZ = chunkX->second.find(z - 1);
		if (chunkZ != chunkX->second.end())
		{
			chunkZ->second->updateBlockBack(hitChunk, blockPos[0], blockPos[1]);
		}
	}

	else if (blockPos[2] == CHUNK_DEPTH - 1)
	{
		auto chunkZ = chunkX->second.find(z + 1);
		if (chunkZ != chunkX->second.end())
		{
			chunkZ->second->updateBlockFront(hitChunk, blockPos[0], blockPos[1]);
		}
	}

	if (blockPos[0] == 0)
	{
		chunkX = mChunks.find(x - 1);
		if (chunkX != mChunks.end())
		{
			auto chunkZ = chunkX->second.find(z);
			if (chunkZ != chunkX->second.end())
			{
				chunkZ->second->updateBlockRight(hitChunk, blockPos[1], blockPos[2]);
			}
		}
	}

	else if (blockPos[0] == CHUNK_WIDTH - 1)
	{
		chunkX = mChunks.find(x + 1);
		if (chunkX != mChunks.end())
		{
			auto chunkZ = chunkX->second.find(z);
			if (chunkZ != chunkX->second.end())
			{
				chunkZ->second->updateBlockLeft(hitChunk, blockPos[1], blockPos[2]);
			}
		}
	}

	hitChunk->updateMesh();

}
