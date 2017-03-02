#pragma once

#define MAP_UPDATE_RADIUS 4
#define MAP_DELETE_RADIUS 18

#define THREADS 4

#include <mutex>
#include <atomic>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Chunk.h"
#include "ChunkPosition.h"

class Map
{
public:
	Map();
	~Map();

	void stopThreads();
	void update(float playerX, float playerZ);
	void updateChunk(int x, int z);
	void threadUpdateChunks();
	void render(float x, float y);
	void rayCastBlock(glm::vec3 start, glm::vec3 forward);
	void rayCastBlockRemove(glm::vec3 start, glm::vec3 forward);
	Chunk* findChunkAt(int x, int z);
private:
	bool m_isRunning;
	std::unordered_map<ChunkPosition,  Chunk*> m_chunks;
	std::queue<Chunk*> m_chunksGenerated;
	std::queue<ChunkPosition> m_chunksToGenerate;

	std::mutex m_chunksGeneratedMutex;
	std::mutex m_chunksToGenerateMutex;
	std::mutex m_threadMutex;
	std::condition_variable m_threadVariable;

	std::condition_variable m_threadEndVariable;
	std::mutex m_threadEndMutex;

	std::unordered_set<ChunkPosition> m_chunksToAddToMap;

	std::thread m_thread;

	
};
