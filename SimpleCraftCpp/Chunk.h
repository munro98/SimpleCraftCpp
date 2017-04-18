#pragma once

#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16
#define CHUNK_HEIGHT 3

#define CHUNK_BLOCK_COUNT CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT

#define TEXTURE_MAP_ROWS 2

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Block.h"

const 	GLfloat frontVertices[] = {
	// Positions           // Normals           // Texture Coords
	0.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	1.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	1.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	0.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	0.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
};

const 	GLfloat backVertices[] = {
	// Positions           // Normals           // Texture Coords
	0.0f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	1.0f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	0.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	0.0f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
};

const 	GLfloat leftVertices[] = {
	// Positions           // Normals           // Texture Coords
	0.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	0.0f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.0f, 0.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	0.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
};

const 	GLfloat rightVertices[] = {
	// Positions           // Normals           // Texture Coords
	1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	1.0f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	1.0f, 0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
};

const 	GLfloat bottomVertices[] = {
	// Positions           // Normals           // Texture Coords
	0.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	1.0f, 0.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	1.0f, 0.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	0.0f, 0.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
};

const 	GLfloat topVertices[] = {
	// Positions           // Normals           // Texture Coords
	0.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	1.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	0.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

class Chunk
{
public:
	Chunk(int chunkX, int chunkZ);
	~Chunk();

	void render();
	void updateBlockFaces(Chunk * frontChunk, Chunk * backChunk, Chunk * leftChunk, Chunk * rightChunk);
	void updateMesh();
	void createVAO();
	void updateFront(const Chunk *frontChunk);
	void updateBack(const Chunk *backChunk);
	void updateLeft(const Chunk *leftChunk);
	void updateRight(const Chunk *rightChunk);
	void updateBlockFront(const Chunk *frontChunk, int x, int y);
	void updateBlockBack(const Chunk *backChunk, int x, int y);
	void updateBlockLeft(const Chunk *leftChunk, int y, int z);
	void updateBlockRight(const Chunk *rightChunk, int y, int z);
	void addFace(int vertrexIndex, int x, int y, int z, int chunkX, int chunkZ, int blockType, const GLfloat vertices[]);
	void updateBlockLeft(int x, int y, int z);
	void updateBlockRight(int x, int y, int z);
	void updateBlockTop(int x, int y, int z);
	void updateBlockBottom(int x, int y, int z);

	void addBlock(unsigned char type, int x, int y, int z);

	void removeBlock(int x, int y, int z);

	void updateBlockFront(int x, int y, int z);
	void updateBlockBack(int x, int y, int z);
	bool rayCastBlock(glm::vec3 hitBlock, int * blockHitPosition) const;
	bool rayCastBlockRemove(glm::vec3 hitBlock, int * blockHitPosition);
	void updateSurroundingBlockFaces(int x, int y, int z);

	bool hitBlock(glm::vec3 & position);

	void saveBlock(int x, int y, int z);

	int getChunkX();
	int getChunkZ();
private:
	int m_chunkX;
	int m_chunkZ;

	Block* m_blocks;
	SavedBlock* m_savedBlocks;
	GLfloat* m_vertices;
	GLuint m_VBO;
	GLuint m_VAO;
	int m_faceCount;

	bool m_meshUpdateCalled;
	bool m_modified;
	
};
