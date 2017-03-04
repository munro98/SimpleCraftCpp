#include <iostream>
#include "Map.h"


Map::Map() : m_isRunning(true)
{
	for (int i = 0; i < THREADS; i++)
	{
		m_threads.push_back(std::thread(&Map::threadUpdateChunks, this));
	}
}

Map::~Map()
{

	for (auto it = m_chunks.begin(); it != m_chunks.end();) {
		auto chunk = it->second;
		delete chunk;
		auto toErase = it;
		++it;
		m_chunks.erase(toErase);
	}

	m_chunksGeneratedMutex.lock();
	while (!m_chunksGenerated.empty())
	{
		delete m_chunksGenerated.front();
		m_chunksGenerated.pop();
	}
	m_chunksGeneratedMutex.unlock();
}


void Map::update(float playerX, float playerZ)
{	
	///*

	int x = (int)std::floor(playerX / CHUNK_WIDTH);
	int z = (int)std::floor(playerZ / CHUNK_DEPTH);

	//std::cout << x << ", " << z << std::endl;

	//Change this algorithm
	/*
	for (int xx = -MAP_UPDATE_RADIUS; xx <= MAP_UPDATE_RADIUS; xx++)
	{
		for (int zz = -MAP_UPDATE_RADIUS; zz <= MAP_UPDATE_RADIUS; zz++)
		{
			//updateChunk(x, z);
			updateChunk(x + xx, z + zz);
		}
	}
	*/

	updateChunk(x, z);

	int di = 1;
	int dj = 0;
	int segmentLength = 1;

	// current position (i, j) and how much of current segment we passed
	int i = 0;
	int j = 0;
	int segmentPassed = 0;
	for (int k = 0; k < 128; ++k) {
		// make a step, add 'direction' vector (di, dj) to current position (i, j)
		i += di;
		j += dj;
		++segmentPassed;
		//std::cout << i << " " << j << "\n";
		updateChunk(x + i, z + j);

		if (segmentPassed == segmentLength) {
			// done with current segment
			segmentPassed = 0;

			//rotate direction
			int buffer = di;
			di = -dj;
			dj = buffer;

			// increase segment length if necessary
			if (dj == 0) {
				++segmentLength;
			}
		}
	}

	//Remove far chunks
	for (auto it = m_chunks.begin(); it != m_chunks.end();) {
		auto chunk = it->second;
		if (abs(chunk->getChunkX() - x) > MAP_DELETE_RADIUS || abs(chunk->getChunkZ() - z) > MAP_DELETE_RADIUS) {
			delete chunk;
			auto toErase = it;
			++it;
			m_chunks.erase(toErase);
		}
		else
		{
			++it;
		}
	}

	///*
	m_chunksGeneratedMutex.lock();
	if(!m_chunksGenerated.empty())
	{
		Chunk* chunk = m_chunksGenerated.front();
		m_chunksGenerated.pop();
		m_chunksGeneratedMutex.unlock();
		ChunkPosition chunkPos(chunk->getChunkX(), chunk->getChunkZ());

		Chunk* frontChunk = findChunkAt(chunkPos.x, chunkPos.z - 1);
		Chunk* backChunk = findChunkAt(chunkPos.x, chunkPos.z + 1);
		Chunk* leftChunk = findChunkAt(chunkPos.x - 1, chunkPos.z);
		Chunk* rightChunk = findChunkAt(chunkPos.x + 1, chunkPos.z);

		chunk->updateBlockFaces(frontChunk, backChunk, leftChunk, rightChunk);
		chunk->updateMesh();

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

		m_chunks.insert({ chunkPos,  chunk });

		m_chunksToAddToMap.erase(chunkPos);
	
	} else {
		m_chunksGeneratedMutex.unlock();
		
	}
	
}

void Map::updateChunk(int x, int z) {

	ChunkPosition chunkPos(x, z);
	auto chunkIt = m_chunks.find(chunkPos);
	auto chunkSetIt = m_chunksToAddToMap.find(chunkPos);
	if (chunkIt == m_chunks.end() && chunkSetIt == m_chunksToAddToMap.end())
	{
		m_chunksToAddToMap.insert(chunkPos);

		m_chunksToGenerateMutex.lock();
		m_chunksToGenerate.push(chunkPos);
		m_chunksToGenerateMutex.unlock();

		m_threadVariable.notify_all();
	}

}

void Map::threadUpdateChunks() {

	while (m_isRunning) {
		//m_ChunkSetQueue lock
		m_chunksToGenerateMutex.lock();
		if (!m_chunksToGenerate.empty())
		{
			ChunkPosition chunkPos = m_chunksToGenerate.front();
			m_chunksToGenerate.pop();
			m_chunksToGenerateMutex.unlock();

			Chunk* chunk = new Chunk(chunkPos.x, chunkPos.z);

			m_chunksGeneratedMutex.lock();
			m_chunksGenerated.push(chunk);
			m_chunksGeneratedMutex.unlock();
		} else
		{
			m_chunksToGenerateMutex.unlock();
			std::unique_lock<std::mutex> lck(m_threadMutex);
			m_threadVariable.wait(lck);
		}
	}
	//m_threadEndVariable.notify_all();

}

void Map::stopThreads()
{
	std::cout << "stopping" << std::endl;
	m_isRunning = false;
	m_threadVariable.notify_all();
	for (int i = 0; i < THREADS; i++)
	{
		m_threads[i].join();
	}
}

void Map::render(float playerX, float playerZ)
{
	for (auto it = m_chunks.begin(); it != m_chunks.end(); ++it) {
		it->second->render();
	}
}

void Map::rayCastBlock(glm::vec3& start, glm::vec3& forward)
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

		float playerX = hitBlock.x;
		float playerZ = hitBlock.z;

		x = (int)std::floor(playerX / CHUNK_WIDTH);
		z = (int)std::floor(playerZ / CHUNK_DEPTH);

		{
			auto chunkIt = m_chunks.find(ChunkPosition(x, z));
			if (chunkIt == m_chunks.end())
			{
				return;
			}
		
			hitChunk = chunkIt->second;
		}

		if (hitChunk->rayCastBlock(hitBlock, blockPos))
		{

			//raycast adding on chunk edges
			if (blockPos[2] == -1)
			{
				auto chunkIt = m_chunks.find(ChunkPosition(x, z - 1));
				if (chunkIt != m_chunks.end())
				{
					hitChunk = chunkIt->second;
					blockPos[2] = CHUNK_DEPTH - 1;
					z--;
				}
			}

			else if (blockPos[2] == CHUNK_DEPTH)
			{
				auto chunkIt = m_chunks.find(ChunkPosition(x, z + 1));
				if (chunkIt != m_chunks.end())
				{
					hitChunk = chunkIt->second;
					blockPos[2] = 0;
					z++;
				}
			}

			else if (blockPos[0] == -1)
			{
				auto chunkIt = m_chunks.find(ChunkPosition(x - 1, z));
				if (chunkIt != m_chunks.end())
				{
					hitChunk = chunkIt->second;
					blockPos[0] = CHUNK_WIDTH - 1;
					x--;
				}
			}

			else if (blockPos[0] == CHUNK_WIDTH)
			{
				auto chunkIt = m_chunks.find(ChunkPosition(x + 1, z));
				if (chunkIt != m_chunks.end())
				{
					hitChunk = chunkIt->second;
					blockPos[0] = 0;
					x++;
				}
			}
			//std::cout << "rayCastBlock2 " << blockPos[0] << " " << blockPos[1] << " " << blockPos[2] << std::endl;
			
			break;
		}
		//No block has been hit

	}

	//hitChunk->setRender(true, blockPos);
	hitChunk->addBlock(1, blockPos[0], blockPos[1], blockPos[2]);
	hitChunk->saveBlock(blockPos[0], blockPos[1], blockPos[2]);

	if (blockPos[2] == 0)
	{
		auto chunkIt = m_chunks.find(ChunkPosition(x, z - 1));
		if (chunkIt != m_chunks.end())
		{
			chunkIt->second->updateBlockBack(hitChunk, blockPos[0], blockPos[1]);

			hitChunk->updateBlockFront(chunkIt->second, blockPos[0], blockPos[1]);
		}
	}

	else if (blockPos[2] == CHUNK_DEPTH - 1)
	{
		auto chunkIt = m_chunks.find(ChunkPosition(x, z + 1));
		if (chunkIt != m_chunks.end())
		{

			chunkIt->second->updateBlockFront(hitChunk, blockPos[0], blockPos[1]);

			hitChunk->updateBlockBack(chunkIt->second, blockPos[0], blockPos[1]);
		}
	}

	if (blockPos[0] == 0)
	{
	
		auto chunkIt = m_chunks.find(ChunkPosition(x - 1, z));
		if (chunkIt != m_chunks.end())
		{
			chunkIt->second->updateBlockRight(hitChunk, blockPos[1], blockPos[2]);

			hitChunk->updateBlockLeft(chunkIt->second, blockPos[1], blockPos[2]);
		}
		
	}

	else if (blockPos[0] == CHUNK_WIDTH - 1)
	{

		auto chunkIt = m_chunks.find(ChunkPosition(x + 1, z));
		if (chunkIt != m_chunks.end())
		{
			chunkIt->second->updateBlockLeft(hitChunk, blockPos[1], blockPos[2]);

			hitChunk->updateBlockRight(chunkIt->second, blockPos[1], blockPos[2]);
		}
		
	}

	hitChunk->updateMesh();
	//std::cout << blockPos[0] << " " << blockPos[1] << " " << blockPos[2] << std::endl;
}

void Map::rayCastBlockRemove(glm::vec3& start, glm::vec3& forward)
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

		float playerX = hitBlock.x;
		float playerZ = hitBlock.z;

		x = (int)std::floor(playerX / CHUNK_WIDTH);
		z = (int)std::floor(playerZ / CHUNK_DEPTH);

		auto chunkIt = m_chunks.find(ChunkPosition(x, z));
		if (chunkIt == m_chunks.end())
		{
			return;
		}

		hitChunk = chunkIt->second;
		if (hitChunk->rayCastBlockRemove(hitBlock, blockPos))
		{

			break;
		}
	}

	//hitChunk->setRender(false, blockPos);
	hitChunk->removeBlock(blockPos[0], blockPos[1], blockPos[2]);
	hitChunk->saveBlock(blockPos[0], blockPos[1], blockPos[2]);

	if (blockPos[2] == 0)
	{
		auto chunkIt = m_chunks.find(ChunkPosition(x, z - 1));
		if (chunkIt != m_chunks.end())
		{
			chunkIt->second->updateBlockBack(hitChunk, blockPos[0], blockPos[1]);
		}
	}

	else if (blockPos[2] == CHUNK_DEPTH - 1)
	{
		auto chunkIt = m_chunks.find(ChunkPosition(x, z + 1));
		if (chunkIt != m_chunks.end())
		{
			chunkIt->second->updateBlockFront(hitChunk, blockPos[0], blockPos[1]);
		}
	}

	if (blockPos[0] == 0)
	{

		auto chunkIt = m_chunks.find(ChunkPosition(x - 1, z));
		if (chunkIt != m_chunks.end())
		{
			chunkIt->second->updateBlockRight(hitChunk, blockPos[1], blockPos[2]);
		}

	}

	else if (blockPos[0] == CHUNK_WIDTH - 1)
	{

		auto chunkIt = m_chunks.find(ChunkPosition(x + 1, z));
		if (chunkIt != m_chunks.end())
		{
			chunkIt->second->updateBlockLeft(hitChunk, blockPos[1], blockPos[2]);
		}

	}

	hitChunk->updateMesh();
	//std::cout << blockPos[0] << " " << blockPos[1] << " " << blockPos[2] << std::endl;

}

bool Map::hitBlock(glm::vec3& position)
{
	Chunk* hitChunk;

	int x;
	int z;
	std::unordered_map<int, std::unordered_map<int, Chunk*>>::iterator chunkX;

	float playerX = position.x;
	float playerZ = position.z;

	x = (int)std::floor(playerX / CHUNK_WIDTH);
	z = (int)std::floor(playerZ / CHUNK_DEPTH);

	auto chunkIt = m_chunks.find(ChunkPosition(x, z));
	if (chunkIt == m_chunks.end())
	{
		return false;
	}

	hitChunk = chunkIt->second;
	if (hitChunk->hitBlock(position))
	{
		return true;
	}

	return false;

}

Chunk* Map::findChunkAt(int x , int z)
{
	auto chunkIt = m_chunks.find(ChunkPosition(x, z));
	if (chunkIt == m_chunks.end())
	{
		return nullptr;
	}
	return chunkIt->second;
}