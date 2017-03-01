#pragma once

#define MAP_UPDATE_RADIUS 2
#define MAP_DELETE_RADIUS 8

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

	void initialize();
	void update(float playerX, float playerZ);
	void updateChunk(int x, int z);
	void render(float x, float y);
	void rayCastBlock(glm::vec3 start, glm::vec3 forward);
	void rayCastBlockRemove(glm::vec3 start, glm::vec3 forward);
	Chunk* findChunkAt(int x, int z);
private:
	std::unordered_map<ChunkPosition,  Chunk*> m_Chunks;
};
