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
	for (auto it = m_Chunks.begin(); it != m_Chunks.end();) {
		auto chunk = it->second;
		delete chunk;
		auto toErase = it;
		++it;
		m_Chunks.erase(toErase);
	}
}

void Map::initialize()
{
	
}

void Map::update(float playerX, float playerZ)
{	
	///*
	playerX += 0.5f;
	playerZ += 0.5f;

	int x = (int)std::floor(playerX / CHUNK_WIDTH);
	int z = (int)std::floor(playerZ / CHUNK_DEPTH);

	//std::cout << x << ", " << z << std::endl;

	//Remove far chunks
	for (auto it = m_Chunks.begin(); it != m_Chunks.end();) {
		auto chunk = it->second;
		if (abs(chunk->getChunkX() - x) > MAP_DELETE_RADIUS || abs(chunk->getChunkZ() - z) > MAP_DELETE_RADIUS) {
			delete chunk;
			auto toErase = it;
			++it;
			m_Chunks.erase(toErase);
		} else
		{
			++it;
		}
	}


	///*
	for (int xx = -MAP_UPDATE_RADIUS; xx <= MAP_UPDATE_RADIUS; xx++)
	{
		for (int zz = -MAP_UPDATE_RADIUS; zz <= MAP_UPDATE_RADIUS; zz++)
		{
			//updateChunk(x, z);
			updateChunk(x + xx, z + zz);
		}
	}
	//*/
}

void Map::updateChunk(int x, int z) {
	auto chunkIt = m_Chunks.find(ChunkPosition(x, z));
	if (chunkIt == m_Chunks.end())
	{
		Chunk* frontChunk = findChunkAt(x,z - 1);
		Chunk* backChunk = findChunkAt(x, z + 1);
		Chunk* leftChunk = findChunkAt(x - 1, z);
		Chunk* rightChunk = findChunkAt(x + 1, z);

		Chunk* chunk = new Chunk(x, z, frontChunk, backChunk, leftChunk, rightChunk);

		m_Chunks.insert({ ChunkPosition(x, z),  chunk });

		/*
		//frontChunk
		auto chunkIt = m_Chunks.find(ChunkPosition(x, z - 1));
		if (chunkIt != m_Chunks.end())
		{
			//frontChunk->updateBack(chunk);
			chunkIt->second->updateBack(chunk);
		}
		*/
		
		///*
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
		//*/
	}
}

void Map::render(float playerX, float playerZ)
{
	for (auto it = m_Chunks.begin(); it != m_Chunks.end(); ++it) {
		it->second->render();
	}
}

void Map::rayCastBlock(glm::vec3 start, glm::vec3 forward)
{

	static float stepValue = 0.1f;
	glm::vec3 step = forward * stepValue;
	glm::vec3 hitBlock = start + forward;

	int blockPos[3] = { -1, -1, -1 };
	Chunk* hitChunk = nullptr;

	int x;
	int z;

	for (int i = 0; i < 100; i++)
	{
		hitBlock = hitBlock + step;

		float playerX = hitBlock.x + 0.5f;
		float playerZ = hitBlock.z + 0.5f;

		x = (int)std::floor(playerX / CHUNK_WIDTH);
		z = (int)std::floor(playerZ / CHUNK_DEPTH);

		auto chunkIt = m_Chunks.find(ChunkPosition(x, z));
		if (chunkIt == m_Chunks.end())
		{
			return;
		}
		
		hitChunk = chunkIt->second;

		if (hitChunk->rayCastBlock(hitBlock, blockPos))
		{

			//raycast adding on chunk edges
			if (blockPos[2] == -1)
			{
				auto chunkIt = m_Chunks.find(ChunkPosition(x, z - 1));
				if (chunkIt != m_Chunks.end())
				{
					hitChunk = chunkIt->second;
					blockPos[2] = CHUNK_DEPTH - 1;
					z--;
				}
			}

			else if (blockPos[2] == CHUNK_DEPTH)
			{
				auto chunkIt = m_Chunks.find(ChunkPosition(x, z + 1));
				if (chunkIt != m_Chunks.end())
				{
					hitChunk = chunkIt->second;
					blockPos[2] = 0;
					z++;
				}
			}

			else if (blockPos[0] == -1)
			{
				auto chunkIt = m_Chunks.find(ChunkPosition(x - 1, z));
				if (chunkIt != m_Chunks.end())
				{
					hitChunk = chunkIt->second;
					blockPos[0] = CHUNK_WIDTH - 1;
					x--;
				}

			}

			else if (blockPos[0] == CHUNK_WIDTH)
			{

				auto chunkIt = m_Chunks.find(ChunkPosition(x + 1, z));
				if (chunkIt != m_Chunks.end())
				{
					hitChunk = chunkIt->second;
					blockPos[0] = 0;
					x++;
				}

			}
			std::cout << "rayCastBlock2 " << blockPos[0] << " " << blockPos[1] << " " << blockPos[2] << std::endl;
			
			break;
		}
		else
		{
			//std::cout << "no block hit" << std::endl;
		}
		

	}

	hitChunk->setRender(true, blockPos);

	if (blockPos[2] == 0)
	{
		auto chunkIt = m_Chunks.find(ChunkPosition(x, z - 1));
		if (chunkIt != m_Chunks.end())
		{
			chunkIt->second->updateBlockBack(hitChunk, blockPos[0], blockPos[1]);

			hitChunk->updateBlockFront(chunkIt->second, blockPos[0], blockPos[1]);
		}
	}

	else if (blockPos[2] == CHUNK_DEPTH - 1)
	{
		auto chunkIt = m_Chunks.find(ChunkPosition(x, z + 1));
		if (chunkIt != m_Chunks.end())
		{

			chunkIt->second->updateBlockFront(hitChunk, blockPos[0], blockPos[1]);

			hitChunk->updateBlockBack(chunkIt->second, blockPos[0], blockPos[1]);
			std::cout << "hitChunk->updateBlockBack" << std::endl;
		}
	}

	if (blockPos[0] == 0)
	{
	
		auto chunkIt = m_Chunks.find(ChunkPosition(x - 1, z));
		if (chunkIt != m_Chunks.end())
		{
			chunkIt->second->updateBlockRight(hitChunk, blockPos[1], blockPos[2]);

			hitChunk->updateBlockLeft(chunkIt->second, blockPos[1], blockPos[2]);
		}
		
	}

	else if (blockPos[0] == CHUNK_WIDTH - 1)
	{

		auto chunkIt = m_Chunks.find(ChunkPosition(x + 1, z));
		if (chunkIt != m_Chunks.end())
		{
			chunkIt->second->updateBlockLeft(hitChunk, blockPos[1], blockPos[2]);

			hitChunk->updateBlockRight(chunkIt->second, blockPos[1], blockPos[2]);
		}
		
	}

	hitChunk->updateMesh();
	//std::cout << blockPos[0] << " " << blockPos[1] << " " << blockPos[2] << std::endl;
	
	//hitChunk->setRender(true, blockPos);
	//hitChunk->updateMesh();

}

void Map::rayCastBlockRemove(glm::vec3 start, glm::vec3 forward)
{

	static float stepValue = 0.1f;
	glm::vec3 step = forward * stepValue;
	glm::vec3 hitBlock = start + forward;

	int blockPos[3] = { -1, -1, -1 };

	Chunk* hitChunk = nullptr;

	int x;
	int z;
	std::unordered_map<int, std::unordered_map<int, Chunk*>>::iterator chunkX;

	for (int i = 0; i < 100; i++)
	{
		hitBlock = hitBlock + step;

		float playerX = hitBlock.x + 0.5f;
		float playerZ = hitBlock.z + 0.5f;

		x = (int)std::floor(playerX / CHUNK_WIDTH);
		z = (int)std::floor(playerZ / CHUNK_DEPTH);

		auto chunkIt = m_Chunks.find(ChunkPosition(x, z));
		if (chunkIt == m_Chunks.end())
		{
			return;
		}

		hitChunk = chunkIt->second;
		if (hitChunk->rayCastBlockRemove(hitBlock, blockPos))
		{

			break;
		}
		else
		{
			//std::cout << "no block hit" << std::endl;
		}

	}

	hitChunk->setRender(false, blockPos);

	if (blockPos[2] == 0)
	{
		auto chunkIt = m_Chunks.find(ChunkPosition(x, z - 1));
		if (chunkIt != m_Chunks.end())
		{
			chunkIt->second->updateBlockBack(hitChunk, blockPos[0], blockPos[1]);
		}
	}

	else if (blockPos[2] == CHUNK_DEPTH - 1)
	{
		auto chunkIt = m_Chunks.find(ChunkPosition(x, z + 1));
		if (chunkIt != m_Chunks.end())
		{
			chunkIt->second->updateBlockFront(hitChunk, blockPos[0], blockPos[1]);
		}
	}

	if (blockPos[0] == 0)
	{

		auto chunkIt = m_Chunks.find(ChunkPosition(x - 1, z));
		if (chunkIt != m_Chunks.end())
		{
			chunkIt->second->updateBlockRight(hitChunk, blockPos[1], blockPos[2]);
		}

	}

	else if (blockPos[0] == CHUNK_WIDTH - 1)
	{

		auto chunkIt = m_Chunks.find(ChunkPosition(x + 1, z));
		if (chunkIt != m_Chunks.end())
		{
			chunkIt->second->updateBlockLeft(hitChunk, blockPos[1], blockPos[2]);
		}

	}

	hitChunk->updateMesh();
	//std::cout << blockPos[0] << " " << blockPos[1] << " " << blockPos[2] << std::endl;

}

Chunk* Map::findChunkAt(int x , int z)
{
	auto chunkIt = m_Chunks.find(ChunkPosition(x, z));
	if (chunkIt == m_Chunks.end())
	{
		return nullptr;
	}
	return chunkIt->second;
}