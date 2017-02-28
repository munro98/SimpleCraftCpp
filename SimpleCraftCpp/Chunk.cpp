#include <iostream>
#include <glm/mat4x2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Chunk.h"
#include <chrono>
#include "HeightGenerator.h"


Chunk::Chunk(int chunkX, int chunkZ, Chunk* frontChunk, Chunk* backChunk, Chunk* leftChunk, Chunk* rightChunk): m_blocks(new Block[CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT]), m_ChunkX(0), m_ChunkZ(0), m_Vertices(nullptr), m_VBO(0), m_VAO(0)
{
	m_ChunkX = chunkX;
	m_ChunkZ = chunkZ;

	auto startTime = std::chrono::high_resolution_clock::now();
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			int height = HeightGenerator::generateHeight(x + (chunkX * CHUNK_WIDTH), z + (chunkZ * CHUNK_DEPTH));
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
			
					int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
					//std::cout << i << std::endl;
					/*
					if (y > 125)
					{
					blocks[i].SetRender(true);
					}
					*/
					/*
					if (x == 1)
					blocks[i].SetRender(false);
					else
					blocks[i].SetRender(true);

					int random = rand() % 100;
					if (random > 50)
					{
					blocks[i].setRender(true);
					}
					*/
					if (y > 8 - height)
					{
						m_blocks[i].setRender(true);
					}
				}
			}
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	auto deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
	std::cout << deltaTime.count() << std::endl;

	m_FaceCount = 0;
	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
				if (!m_blocks[i].getRender())
					continue;

				unsigned int facesExposed = 0;

				if (y == 0)
				{
					facesExposed |= TOP_FACE;
				}

				if (i - CHUNK_WIDTH * CHUNK_DEPTH > -1 && !m_blocks[i - CHUNK_WIDTH * CHUNK_DEPTH].getRender())
				{
					facesExposed |= TOP_FACE;
				}

				if (i + CHUNK_WIDTH * CHUNK_DEPTH < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT && !m_blocks[i + CHUNK_WIDTH * CHUNK_DEPTH].getRender())
				{
					facesExposed |= BOTTOM_FACE;
				}

				if (leftChunk != nullptr && x == 0)
				{
					int j = CHUNK_WIDTH - 1 + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
					if (!leftChunk->m_blocks[j].getRender())
					{
						facesExposed |= LEFT_FACE;
					}

				}
				else if (x != 0 && i - 1 > -1 && !m_blocks[i - 1].getRender())
				{
					facesExposed |= LEFT_FACE;
				}

				if (rightChunk != nullptr && x == CHUNK_WIDTH - 1)
				{
					int j = 0 + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
					if (!rightChunk->m_blocks[j].getRender())
					{
						facesExposed |= RIGHT_FACE;
					}

				}
				else if (x != CHUNK_WIDTH - 1 && i + 1 < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT && !m_blocks[i + 1].getRender())
				{
					facesExposed |= RIGHT_FACE;
				}

				if (frontChunk != nullptr && z == 0)
				{
					int j = x + (CHUNK_DEPTH - 1) * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
					if (!frontChunk->m_blocks[j].getRender())
					{
						facesExposed |= FRONT_FACE;
					}

				}
				else if (z != 0 && i - CHUNK_WIDTH > -1 && !m_blocks[i - CHUNK_WIDTH].getRender())
				{
					facesExposed |= FRONT_FACE;
				}

				if (backChunk != nullptr && z == CHUNK_DEPTH - 1)
				{
					int j = x + 0 * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
					if (!backChunk->m_blocks[j].getRender())
					{
						facesExposed |= BACK_FACE;
					}

				}
				else if (z != CHUNK_DEPTH - 1 && z < CHUNK_DEPTH - 1 && i + CHUNK_WIDTH < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT && !m_blocks[i + CHUNK_WIDTH].getRender())
				{
					facesExposed |= BACK_FACE;
				}

				/////////////////////////////

				if (facesExposed & FRONT_FACE)
				{
					m_FaceCount++;
				}

				if (facesExposed & BACK_FACE)
				{
					m_FaceCount++;
				}

				if (facesExposed & LEFT_FACE)
				{
					m_FaceCount++;
				}

				if (facesExposed & RIGHT_FACE)
				{
					m_FaceCount++;
				}

				if (facesExposed & TOP_FACE)
				{
					m_FaceCount++;
				}

				if (facesExposed & BOTTOM_FACE)
				{
					m_FaceCount++;
				}
				//std::cout << "blocks[i] " << i << " " << std::endl;
				m_blocks[i].setExposedFaces(facesExposed);//

			}
		}
	}

	//std::cout << mFaceCount << std::endl;
	m_Vertices = new GLfloat[8 * 6 * m_FaceCount];
	//mVertices = new GLfloat[48];
	//mVertices = new GLfloat[sizeof(topVertices) / sizeof(topVertices[0])];
	int counter = 0;


	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;


				unsigned int facesExposed = m_blocks[i].getExposedFaces();

				if (facesExposed & FRONT_FACE)
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, chunkX, chunkZ, frontVertices);
				}

				if (facesExposed & BACK_FACE)
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, chunkX, chunkZ, backVertices);
				}

				if (facesExposed & LEFT_FACE)
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, chunkX, chunkZ, leftVertices);
				}

				if (facesExposed & RIGHT_FACE)
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, chunkX, chunkZ, rightVertices);
				}

				if (facesExposed & TOP_FACE) //  && z == 0
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, chunkX, chunkZ, topVertices);
				}

				if (facesExposed & BOTTOM_FACE)
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, chunkX, chunkZ, bottomVertices);
				}

			}
		}
	}

	

	createVAO();
}

Chunk::~Chunk()
{
	//std::cout << mVertices << "\n";
	delete[] m_blocks;
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);
	

	//char c;
	//std::cin >> c;
}

void Chunk::render()
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6 * m_FaceCount);
	glBindVertexArray(0);

}

void Chunk::updateMesh()
{
	m_FaceCount = 0;

	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
				if (!m_blocks[i].getRender())
					continue;

				unsigned int facesExposed = m_blocks[i].getExposedFaces();

				/////////////////////////////

				if (facesExposed & FRONT_FACE)
				{
					m_FaceCount++;
				}

				if (facesExposed & BACK_FACE)
				{
					m_FaceCount++;
				}

				if (facesExposed & LEFT_FACE)
				{
					m_FaceCount++;
				}

				if (facesExposed & RIGHT_FACE)
				{
					m_FaceCount++;
				}

				if (facesExposed & TOP_FACE)
				{
					m_FaceCount++;
				}

				if (facesExposed & BOTTOM_FACE)
				{
					m_FaceCount++;
				}
				//std::cout << "blocks[i] " << i << " " << std::endl;
				//blocks[i].SetExposedFaces(facesExposed);//
			}
		}
	}

	//std::cout << mFaceCount << std::endl;
	m_Vertices = new GLfloat[8 * 6 * m_FaceCount];

	int counter = 0;

	
	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;


				unsigned int facesExposed = m_blocks[i].getExposedFaces();

				if (facesExposed & FRONT_FACE)
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, m_ChunkX, m_ChunkZ, frontVertices);
				}

				if (facesExposed & BACK_FACE)
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, m_ChunkX, m_ChunkZ, backVertices);
				}

				if (facesExposed & LEFT_FACE)
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, m_ChunkX, m_ChunkZ, leftVertices);
				}

				if (facesExposed & RIGHT_FACE)
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, m_ChunkX, m_ChunkZ, rightVertices);
				}

				if (facesExposed & TOP_FACE)
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, m_ChunkX, m_ChunkZ, topVertices);
				}

				if (facesExposed & BOTTOM_FACE)
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, m_ChunkX, m_ChunkZ, bottomVertices);
				}

			}
		}
	}

	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);
	createVAO();
}

void Chunk::createVAO()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//Copy Chunk data to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * 6 * m_FaceCount, m_Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	delete[] m_Vertices;
}

void Chunk::updateFront(Chunk* frontChunk)
{
	
	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			int i = x + 0 * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;

			if (!m_blocks[i].getRender())
			{
				continue;
			}

			unsigned int facesExposed = m_blocks[i].getExposedFaces();
			int j = x + (CHUNK_DEPTH - 1) * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
			if (!frontChunk->m_blocks[j].getRender())
			{
				facesExposed |= FRONT_FACE;
			}
			else
			{
				facesExposed &= ~FRONT_FACE;
			}
			m_blocks[i].setExposedFaces(facesExposed);
		}
	}
	updateMesh();
}

void Chunk::updateBack(Chunk* backChunk)
{
	
	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			int i = x + (CHUNK_DEPTH - 1) * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;

			if (!m_blocks[i].getRender())
			{
				continue;
			}

			unsigned int facesExposed = m_blocks[i].getExposedFaces();
			int j = x + 0 * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
			if (!backChunk->m_blocks[j].getRender())
			{
				facesExposed |= BACK_FACE;
			} else
			{
				facesExposed &= ~BACK_FACE;
			}

			m_blocks[i].setExposedFaces(facesExposed);
		}
	}
	updateMesh();
}

void Chunk::updateLeft(Chunk* leftChunk)
{
	for (int z = 0; z < CHUNK_DEPTH; z++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			int i = 0 + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;

			if (!m_blocks[i].getRender())
			{
				continue;
			}

			unsigned int facesExposed = m_blocks[i].getExposedFaces();
			int j = (CHUNK_WIDTH - 1) + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
			if (!leftChunk->m_blocks[j].getRender())
			{
				facesExposed |= LEFT_FACE;
			}
			else
			{
				facesExposed &= ~LEFT_FACE;
			}
			m_blocks[i].setExposedFaces(facesExposed);
		}
	}
	updateMesh();
}

void Chunk::updateRight(Chunk* rightChunk)
{
	for (int z = 0; z < CHUNK_DEPTH; z++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			int i = (CHUNK_WIDTH - 1) + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;

			if (!m_blocks[i].getRender())
			{
				continue;
			}

			unsigned int facesExposed = m_blocks[i].getExposedFaces();
			int j = 0 + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
			if (!rightChunk->m_blocks[j].getRender())
			{
				facesExposed |= RIGHT_FACE;
			}
			else
			{
				facesExposed &= ~RIGHT_FACE;
			}
			m_blocks[i].setExposedFaces(facesExposed);
		}
	}
	updateMesh();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void Chunk::updateBlockFront(Chunk* frontChunk, int x, int y)
{

	int i = x + 0 * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;

	if (!m_blocks[i].getRender())
	{
		return;
	}

	unsigned int facesExposed = m_blocks[i].getExposedFaces();
	int j = x + (CHUNK_DEPTH - 1) * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	if (!frontChunk->m_blocks[j].getRender())
	{
		facesExposed |= FRONT_FACE;
	}
	else
	{
		facesExposed &= ~FRONT_FACE;
	}
	m_blocks[i].setExposedFaces(facesExposed);

	updateMesh();
}


void Chunk::updateBlockBack(Chunk* backChunk, int x, int y)
{
	int i = x + (CHUNK_DEPTH - 1) * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;

	if (!m_blocks[i].getRender())
	{
		return;
	}

	unsigned int facesExposed = m_blocks[i].getExposedFaces();
	int j = x + 0 * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	if (!backChunk->m_blocks[j].getRender())
	{
		facesExposed |= BACK_FACE;
	}
	else
	{
		facesExposed &= ~BACK_FACE;
	}

	m_blocks[i].setExposedFaces(facesExposed);

	updateMesh();
}

void Chunk::updateBlockLeft(Chunk* leftChunk, int y, int z)
{
	int i = 0 + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;

	if (!m_blocks[i].getRender())
	{
		return;
	}

	unsigned int facesExposed = m_blocks[i].getExposedFaces();
	int j = (CHUNK_WIDTH - 1) + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	if (!leftChunk->m_blocks[j].getRender())
	{
		facesExposed |= LEFT_FACE;
	}
	else
	{
		facesExposed &= ~LEFT_FACE;
	}
	m_blocks[i].setExposedFaces(facesExposed);

	updateMesh();
}

void Chunk::updateBlockRight(Chunk* rightChunk, int y, int z)
{
	int i = (CHUNK_WIDTH - 1) + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;

	if (!m_blocks[i].getRender())
	{
		return;
	}

	unsigned int facesExposed = m_blocks[i].getExposedFaces();
	int j = 0 + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	if (!rightChunk->m_blocks[j].getRender())
	{
		facesExposed |= RIGHT_FACE;
	}
	else
	{
		facesExposed &= ~RIGHT_FACE;
	}
	m_blocks[i].setExposedFaces(facesExposed);

	updateMesh();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Chunk::addFace(int vertrexIndex ,int x, int y, int z,int chunkX, int chunkZ, const GLfloat vertices[])
{
	y = -y;
	memcpy(m_Vertices + vertrexIndex, vertices, 192);
	//std::cout << "hit " << sizeof(vertices) << " " << std::endl;
	m_Vertices[vertrexIndex] += (GLfloat)(x + chunkX * CHUNK_WIDTH);
	m_Vertices[vertrexIndex + 8] += (GLfloat)(x + chunkX * CHUNK_WIDTH);
	m_Vertices[vertrexIndex + 16] += (GLfloat)(x + chunkX * CHUNK_WIDTH);
	m_Vertices[vertrexIndex + 24] += (GLfloat)(x + chunkX * CHUNK_WIDTH);
	m_Vertices[vertrexIndex + 32] += (GLfloat)(x + chunkX * CHUNK_WIDTH);
	m_Vertices[vertrexIndex + 40] += (GLfloat)(x + chunkX * CHUNK_WIDTH);

	m_Vertices[vertrexIndex + 1] += (GLfloat)(y);
	m_Vertices[vertrexIndex + 8 + 1] += (GLfloat)(y);
	m_Vertices[vertrexIndex + 16 + 1] += (GLfloat)(y);
	m_Vertices[vertrexIndex + 24 + 1] += (GLfloat)(y);
	m_Vertices[vertrexIndex + 32 + 1] += (GLfloat)(y);
	m_Vertices[vertrexIndex + 40 + 1] += (GLfloat)(y);

	m_Vertices[vertrexIndex + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	m_Vertices[vertrexIndex + 8 + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	m_Vertices[vertrexIndex + 16 + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	m_Vertices[vertrexIndex + 24 + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	m_Vertices[vertrexIndex + 32 + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	m_Vertices[vertrexIndex + 40 + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
}

void Chunk::updateBlock(int x, int y, int z)
{
	if (x < 0)
	{
		return;
	}

	if (x >= CHUNK_WIDTH)
	{
		return;
	}

	if (z < 0)
	{
		return;
	}

	if (z >= CHUNK_DEPTH)
	{
		return;
	}

	if (y < 0)
	{
		return;
	}

	if (y >= CHUNK_HEIGHT)
	{
		return;
	}

	int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	unsigned int facesExposed = 0;

	if (!m_blocks[i].getRender())
	{
		m_blocks[i].setExposedFaces(facesExposed);
		return;
	}
		
	if (y == 0)
	{
		facesExposed |= TOP_FACE;
	}

	if (i - CHUNK_WIDTH * CHUNK_DEPTH > -1 && !m_blocks[i - CHUNK_WIDTH * CHUNK_DEPTH].getRender())
	{
		facesExposed |= TOP_FACE;
	}

	if (i + CHUNK_WIDTH * CHUNK_DEPTH < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT && !m_blocks[i + CHUNK_WIDTH * CHUNK_DEPTH].getRender())
	{
		facesExposed |= BOTTOM_FACE;
	}

	if (x > 0 && i - 1 > -1 && !m_blocks[i - 1].getRender())
	{
		facesExposed |= LEFT_FACE;
	}

	if (x < CHUNK_WIDTH - 1 && i + 1 < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT && !m_blocks[i + 1].getRender())
	{
		facesExposed |= RIGHT_FACE;
	}

	if (z > 0 && i - CHUNK_WIDTH > -1 && !m_blocks[i - CHUNK_WIDTH].getRender())
	{
		facesExposed |= FRONT_FACE;
	}

	if (z < CHUNK_DEPTH - 1 && i + CHUNK_WIDTH < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT && !m_blocks[i + CHUNK_WIDTH].getRender())
	{
		facesExposed |= BACK_FACE;
	}

	m_blocks[i].setExposedFaces(facesExposed);
}

void Chunk::updateBlock(bool value, int x, int y, int z)
{
	if (x < 0)
	{
		return;
	}

	if (x >= CHUNK_WIDTH)
	{
		return;
	}

	if (z < 0)
	{
		return;
	}

	if (z >= CHUNK_DEPTH)
	{
		return;
	}

	if (y < 0)
	{
		return;
	}

	if (y >= CHUNK_HEIGHT)
	{
		return;
	}

	int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	unsigned int facesExposed = 0;

	m_blocks[i].setRender(value);
	if (!m_blocks[i].getRender())
	{
		m_blocks[i].setExposedFaces(facesExposed);
		return;
	}

	if (y == 0)
	{
		facesExposed |= TOP_FACE;
	}

	if (i - CHUNK_WIDTH * CHUNK_DEPTH > -1 && !m_blocks[i - CHUNK_WIDTH * CHUNK_DEPTH].getRender())
	{
		facesExposed |= TOP_FACE;
	}

	if (i + CHUNK_WIDTH * CHUNK_DEPTH < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT && !m_blocks[i + CHUNK_WIDTH * CHUNK_DEPTH].getRender())
	{
		facesExposed |= BOTTOM_FACE;
	}

	if (x > 0 && i - 1 > -1 && !m_blocks[i - 1].getRender())
	{
		facesExposed |= LEFT_FACE;
	}

	if (x < CHUNK_WIDTH - 1 && i + 1 < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT && !m_blocks[i + 1].getRender())
	{
		facesExposed |= RIGHT_FACE;
	}

	if (z > 0 && i - CHUNK_WIDTH > -1 && !m_blocks[i - CHUNK_WIDTH].getRender())
	{
		facesExposed |= FRONT_FACE;
	}

	if (z < CHUNK_DEPTH - 1 && i + CHUNK_WIDTH < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT && !m_blocks[i + CHUNK_WIDTH].getRender())
	{
		facesExposed |= BACK_FACE;
	}

	m_blocks[i].setExposedFaces(facesExposed);
}

void Chunk::updateBlockFront(int x, int y, int z)
{
	if (z < 0)
	{
		return;
	}

	int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	unsigned int facesExposed = m_blocks[i].getExposedFaces();

	if (!m_blocks[i].getRender())
	{
		m_blocks[i].setExposedFaces(0);
		return;
	}

	if (!m_blocks[i + CHUNK_WIDTH].getRender())
	{
		facesExposed |= BACK_FACE;
	}
	else
	{
		facesExposed &= ~BACK_FACE;
	}

	//std::cout << "blocks[i] " << i << " " << std::endl;
	m_blocks[i].setExposedFaces(facesExposed);//
}

void Chunk::updateBlockBack(int x, int y, int z)
{
	if (z >= CHUNK_DEPTH)
	{
		return;
	}

	int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	unsigned int facesExposed = m_blocks[i].getExposedFaces();

	if (!m_blocks[i].getRender())
	{
		m_blocks[i].setExposedFaces(0);
		return;
	}

	if (!m_blocks[i - CHUNK_WIDTH].getRender())
	{
		facesExposed |= FRONT_FACE;
	}
	else
	{
		facesExposed &= ~FRONT_FACE;
	}

	m_blocks[i].setExposedFaces(facesExposed);//
}

void Chunk::updateBlockLeft(int x, int y, int z)
{
	if (x < 0)
	{
		return;
	}

	int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	unsigned int facesExposed = m_blocks[i].getExposedFaces();

	if (!m_blocks[i].getRender())
	{
		m_blocks[i].setExposedFaces(0);
		return;
	}

	if (!m_blocks[i + 1].getRender())
	{
		facesExposed |= RIGHT_FACE;
	}
	else
	{
		facesExposed &= ~RIGHT_FACE;
	}

	//std::cout << "blocks[i] " << i << " " << std::endl;
	m_blocks[i].setExposedFaces(facesExposed);//
}

void Chunk::updateBlockRight(int x, int y, int z)
{
	if (x >= CHUNK_WIDTH)
	{
		return;
	}

	int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	unsigned int facesExposed = m_blocks[i].getExposedFaces();

	if (!m_blocks[i].getRender())
	{
		m_blocks[i].setExposedFaces(0);
		return;
	}

	if (!m_blocks[i - 1].getRender())
	{
		facesExposed |= LEFT_FACE;
	}
	else
	{
		facesExposed &= ~LEFT_FACE;
	}

	//std::cout << "blocks[i] " << i << " " << std::endl;
	m_blocks[i].setExposedFaces(facesExposed);//
}

void Chunk::updateBlockTop(int x, int y, int z)
{
	if (y < 0)
	{
		return;
	}
	//std::cout << "updateBlockTop" << std::endl;

	int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	unsigned int facesExposed = m_blocks[i].getExposedFaces();

	if (!m_blocks[i].getRender())
	{
		m_blocks[i].setExposedFaces(0);
		return;
	}

	if (!m_blocks[i + CHUNK_WIDTH * CHUNK_DEPTH].getRender())
	{
		facesExposed |= BOTTOM_FACE;
	}
	else
	{
		facesExposed &= ~BOTTOM_FACE;
	}

	m_blocks[i].setExposedFaces(facesExposed);//
}

void Chunk::updateBlockBottom(int x, int y, int z)
{

	if (y >= CHUNK_HEIGHT)
	{
		return;
	}

	int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	unsigned int facesExposed = m_blocks[i].getExposedFaces();

	if (!m_blocks[i].getRender())
	{
		m_blocks[i].setExposedFaces(0);
		return;
	}

	if (!m_blocks[i - CHUNK_WIDTH * CHUNK_DEPTH].getRender())
	{
		facesExposed |= TOP_FACE;
	}
	else
	{
		facesExposed &= ~TOP_FACE;
	}

	m_blocks[i].setExposedFaces(facesExposed);//
}

bool Chunk::rayCastBlock(glm::vec3 hitBlock, int* blockHitPosition) const
{

	hitBlock.x = hitBlock.x - (float)(m_ChunkX * CHUNK_WIDTH);
	hitBlock.z = hitBlock.z - (float)(m_ChunkZ * CHUNK_DEPTH);

	int x, y, z;
	x = (int)round(hitBlock.x);
	z = (int)round(hitBlock.z);
	y = (int)round(-hitBlock.y);

	//std::cout << "hitBlock " << hitBlock. x << " " << hitBlock.y << " " << hitBlock.z << std::endl;
	//std::cout << "block " << x << " " << y << " " << z << std::endl;

	if (x >= CHUNK_WIDTH)
	{
		return false;
	}

	if (x < 0)
	{
		return false;
	}

	if (y < 0)
	{
		return false;
	}

	if (z >= CHUNK_DEPTH)
	{
		return false;
	}

	if (z < 0)
	{
		return false;
	}

	int i = x + (z * CHUNK_WIDTH) + (y * CHUNK_WIDTH * CHUNK_DEPTH);

	// We hit a block
	if (m_blocks[i].getRender())
	{
		int j = blockHitPosition[0] + (blockHitPosition[2] * CHUNK_WIDTH) + (blockHitPosition[1] * CHUNK_WIDTH * CHUNK_DEPTH);

		//std::cout << "hit " << hitBlock.x - (float)x << " " << hitBlock.y + (float)y << " " << hitBlock.z - (float)z << std::endl;

		bool right = false;
		bool back = false;
		bool top = false;

		if (hitBlock.x - (float)x > 0.0f)
		{
			right = true;
		}

		if (hitBlock.z - (float)z > 0.0f)
		{
			back = true;
		}

		if (hitBlock.y + (float)y > 0.0f)
		{
			top = true;
		}


		float relativeX = abs(hitBlock.x - (float)x);
		float relativeY = abs(hitBlock.y + (float)y);
		float relativeZ = abs(hitBlock.z - (float)z);
	
		//could be x or y or z
		if (relativeX > relativeZ)
		{
			//could be x or y
			if (relativeX > relativeY)
			{
				// its x
				if (right)
				{
					x++;
				}
				else
				{
					x--;
				}
			}
			else
			{
				// its y
				if (top)
				{
					y--;
				}
				else
				{
					y++;
				}
			} 
		}
		else
		{
			//could be y or z
			if (relativeY > relativeZ)
			{
				// its y
				if (top)
				{
					y--;
				}
				else
				{
					y++;
				}
			}
			else
			{
				//its z
				if (back)
				{
					z++;
				}
				else
				{
					z--;
				}
			}
		}
		std::cout << "rayCastBlock " << x << " " << y << " " << z << std::endl;
		blockHitPosition[0] = x;
		blockHitPosition[1] = y;
		blockHitPosition[2] = z;
		return true;
		
	}
	//*/
	//blocks[i].setRender(true);
	//updateBlock(x, y, z);
	return false;
}

bool Chunk::rayCastBlockRemove(glm::vec3 hitBlock, int* blockHitPosition)
{
	hitBlock.x = hitBlock.x - (float)(m_ChunkX * CHUNK_WIDTH);
	hitBlock.z = hitBlock.z - (float)(m_ChunkZ * CHUNK_DEPTH);

	int x, y, z;
	x = (int)round(hitBlock.x);
	z = (int)round(hitBlock.z);
	y = (int)round(-hitBlock.y);

	//std::cout << "block " << x << " " << y << " " << z << std::endl;

	// do we need this?
	//should we stop early here?
	if (x >= CHUNK_WIDTH)
	{
		return false;
	}

	if (x < 0)
	{
		return false;
	}

	if (y < 0)
	{
		return false;
	}

	if (z >= CHUNK_DEPTH)
	{
		return false;
	}

	if (z < 0)
	{
		return false;
	}

	int i = x + (z * CHUNK_WIDTH) + (y * CHUNK_WIDTH * CHUNK_DEPTH);

	// We hit a block
	if (m_blocks[i].getRender())
	{
		int j = blockHitPosition[0] + (blockHitPosition[2] * CHUNK_WIDTH) + (blockHitPosition[1] * CHUNK_WIDTH * CHUNK_DEPTH);

		std::cout << "hit " << hitBlock.x - (float)x << " " << hitBlock.y + (float)y << " " << hitBlock.z - (float)z << std::endl;

		blockHitPosition[0] = x;
		blockHitPosition[1] = y;
		blockHitPosition[2] = z;
		return true;

	}
	//*/
	//blocks[i].setRender(true);
	//updateBlock(x, y, z);
	return false;
}

void Chunk::setRender(bool value, int* blockHitPosition)
{

	updateBlock(value, blockHitPosition[0], blockHitPosition[1], blockHitPosition[2]);
	// Update block faces around this block
	updateSurroundingBlockFaces(blockHitPosition[0], blockHitPosition[1], blockHitPosition[2]);
}

void Chunk::updateSurroundingBlockFaces(int x, int y, int z)
{
	updateBlockLeft(x - 1, y, z);
	updateBlockRight(x + 1, y, z);

	updateBlockFront(x, y, z - 1);
	updateBlockBack(x, y, z + 1);

	updateBlockTop(x, y - 1, z);
	updateBlockBottom(x, y + 1, z);
}

int Chunk::getChunkX()
{
	return m_ChunkX;
}

int Chunk::getChunkZ()
{
	return m_ChunkZ;
}
