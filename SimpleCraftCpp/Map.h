#pragma once
#define MAP_CHUNK 5
#define MAP_WIDTH 2
#define MAP_DEPTH 1

#include <unordered_map>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Chunk.h"

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
private:
	std::unordered_map<int, std::unordered_map<int, Chunk*>> mChunks;
};
