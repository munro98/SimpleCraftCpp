#pragma once
#define MAP_CHUNK 5
#define MAP_WIDTH 2
#define MAP_DEPTH 1

#include <unordered_map>
#include <GL/glew.h>

#include "Chunk.h"

class Map
{
public:
	Map();
	~Map();
	void initialize();
	void update(float playerX, float playerZ);
	void render(GLuint modelLocation, GLuint blockPositionLocation, float x, float y);
	void rayCastBlock(RayCast & rayCast, glm::vec3 start, glm::vec3 forward);
	void rayCastBlockRemove(RayCast & rayCast, glm::vec3 start, glm::vec3 forward);
private:
	std::unordered_map<int64_t, Chunk*> mChunks;
};
