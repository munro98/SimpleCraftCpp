#pragma once

#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16
#define CHUNK_HEIGHT 16

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Block.h"

const 	GLfloat frontVertices[] = {
	// Positions           // Normals           // Texture Coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
};

const 	GLfloat backVertices[] = {
	// Positions           // Normals           // Texture Coords
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
};

const 	GLfloat leftVertices[] = {
	// Positions           // Normals           // Texture Coords
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
};

const 	GLfloat rightVertices[] = {
	// Positions           // Normals           // Texture Coords
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
};

const 	GLfloat bottomVertices[] = {
	// Positions           // Normals           // Texture Coords
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
};

const 	GLfloat topVertices[] = {
	// Positions           // Normals           // Texture Coords
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

class RayCast;

class Chunk
{
public:
	Chunk();
	Chunk(int chunkX, int chunkZ, Chunk * frontChunk, Chunk * backChunk, Chunk * leftChunk, Chunk * rightChunk);
	~Chunk();
	void render();
	void updateMesh();
	void createVAO();
	void updateFront(Chunk * frontChunk);
	void updateBack(Chunk * backChunk);
	void updateLeft(Chunk * leftChunk);
	void updateRight(Chunk * rightChunk);
	void updateBlockFront(Chunk * frontChunk, int x, int y);
	void updateBlockBack(Chunk * backChunk, int x, int y);
	void updateBlockLeft(Chunk * leftChunk, int y, int z);
	void updateBlockRight(Chunk * rightChunk, int y, int z);
	void addFace(int vertrexIndex, int x, int y, int z, int chunkX, int chunkZ, const GLfloat vertices[]);
	void updateBlock(int x, int y, int z);
	void updateBlock(bool value, int x, int y, int z);
	void updateBlockLeft(int x, int y, int z);
	void updateBlockRight(int x, int y, int z);
	void updateBlockTop(int x, int y, int z);
	void updateBlockBottom(int x, int y, int z);

	void updateBlockFront(int x, int y, int z);
	void updateBlockBack(int x, int y, int z);
	bool rayCastBlock(glm::vec3 hitBlock, int * blockHitPosition) const;
	bool rayCastBlockRemove(glm::vec3 hitBlock, int * blockHitPosition);
	void setRender(bool value, int * blockHitPosition);
	void updateSurroundingBlockFaces(int x, int y, int z);

	int getChunkX();
	int getChunkZ();
private:
	Block* blocks;
	GLfloat* mVertices;
	GLuint mVBO;
	GLuint mVAO;
	int mFaceCount;
	int mChunkX;
	int mChunkZ;
};

