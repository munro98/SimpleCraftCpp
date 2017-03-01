#pragma once

#define MAP_UPDATE_RADIUS 4
#define MAP_DELETE_RADIUS 10

#define THREADS 4

#include <unordered_map>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Chunk.h"
#include "ChunkPosition.h"
#include "ChunkSetQueue.h"
#include <mutex>
#include <atomic>

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
	std::queue<Chunk*> m_chunkQueue;
	ChunkSetQueue m_chunkSetQueue;

	std::mutex m_chunkQueueMutex;
	std::mutex m_chunkSetQueueMutex;
	std::mutex m_threadMutex;
	std::condition_variable m_threadVariable;
	
	std::thread m_thread;
	//std::atomic<bool> m_isRunning;
	
};
