#include <iostream>
#include <glm/mat4x2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <istream>
#include <sstream>

#include "Chunk.h"
#include "HeightGenerator.h"



Chunk::Chunk(int chunkX, int chunkZ): m_blocks(new Block[CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT]), m_savedBlocks(new Block[CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT]), m_chunkX(chunkX), m_chunkZ(chunkZ), m_vertices(nullptr), m_VBO(0), m_VAO(0), m_meshUpdateCalled(false), m_modified(false)
{

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
					if (y > 28 - height)
					{
						m_blocks[i].setRender(true);
					}

					if (y > 40)
					{
						m_blocks[i].setType(2);
					}

					if (y > 43)
					{
						m_blocks[i].setType(3);
					}
				}
			}
	}

	std::ifstream inFile;
	std::string line;
	
	inFile.open("chunkData/" + std::string(std::to_string(m_chunkX) + " " + std::to_string(m_chunkZ) + ".txt"));
	
	if (inFile.is_open()) {
		while (inFile.good()) {
			getline(inFile, line);

			std::stringstream lineSS(line);

			int index;
			int blockType;
			lineSS >> index;
			lineSS >> blockType;

			//std::cout << index << " " << blockType << std::endl;

			if (index < 0 || index > CHUNK_BLOCK_COUNT)
			{
				std::cout << "Corrupt file" << std::endl;
				break;
			}

			m_savedBlocks[index].setRender(true);
			m_savedBlocks[index].setType(blockType);

			if(blockType == 0)
			{
				m_blocks[index].setRender(false);
			} else
			{
				m_blocks[index].setRender(true);
			}
			m_blocks[index].setType(blockType);
			//m_modified |= true;
		}

	}

	inFile.close();
	//std::this_thread::sleep_for(std::chrono::milliseconds(500));

	auto endTime = std::chrono::high_resolution_clock::now();
	auto deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
	//std::cout << deltaTime.count() << std::endl;
	/*
	
	*/


}

Chunk::~Chunk()
{
	//std::cout << mVertices << "\n";
	if (m_modified) {
		std::ofstream outFile("chunkData/" + std::string(std::to_string(m_chunkX) + " " + std::to_string(m_chunkZ) + ".txt"));

		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_DEPTH; z++)
			{
				for (int x = 0; x < CHUNK_WIDTH; x++)
				{
					int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;

					int type = m_savedBlocks[i].getType();
					if (m_savedBlocks[i].getRender())
						outFile << i << " " << type << "\n";
				}
			}
		}
		outFile.close();
	}

	delete[] m_blocks;
	delete[] m_savedBlocks;
	if (m_meshUpdateCalled) {
		glDeleteBuffers(1, &m_VBO);
		glDeleteVertexArrays(1, &m_VAO);
	}

}

void Chunk::render()
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6 * m_faceCount);
	glBindVertexArray(0);

}

void Chunk::updateBlockFaces(Chunk* frontChunk, Chunk* backChunk, Chunk* leftChunk, Chunk* rightChunk)
{
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
				m_blocks[i].setExposedFaces(facesExposed);//

			}
		}
	}
}

void Chunk::updateMesh()
{
	m_faceCount = 0;

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

				if (facesExposed & FRONT_FACE)
				{
					m_faceCount++;
				}

				if (facesExposed & BACK_FACE)
				{
					m_faceCount++;
				}

				if (facesExposed & LEFT_FACE)
				{
					m_faceCount++;
				}

				if (facesExposed & RIGHT_FACE)
				{
					m_faceCount++;
				}

				if (facesExposed & TOP_FACE)
				{
					m_faceCount++;
				}

				if (facesExposed & BOTTOM_FACE)
				{
					m_faceCount++;
				}
				//std::cout << "blocks[i] " << i << " " << std::endl;
				//blocks[i].SetExposedFaces(facesExposed);//
			}
		}
	}

	//std::cout << mFaceCount << std::endl;
	m_vertices = new GLfloat[8 * 6 * m_faceCount];

	int counter = 0;

	
	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;


				unsigned int facesExposed = m_blocks[i].getExposedFaces();
				unsigned char blockType = m_blocks[i].getType();

				if (facesExposed & FRONT_FACE)
				{
					int vertexIndex = (counter++) * 48;
					addFace(vertexIndex, x, y, z, m_chunkX, m_chunkZ, blockType, frontVertices);
				}

				if (facesExposed & BACK_FACE)
				{
					int vertexIndex = (counter++) * 48;
					addFace(vertexIndex, x, y, z, m_chunkX, m_chunkZ, blockType, backVertices);
				}

				if (facesExposed & LEFT_FACE)
				{
					int vertexIndex = (counter++) * 48;
					addFace(vertexIndex, x, y, z, m_chunkX, m_chunkZ, blockType, leftVertices);
				}

				if (facesExposed & RIGHT_FACE)
				{
					int vertexIndex = (counter++) * 48;
					addFace(vertexIndex, x, y, z, m_chunkX, m_chunkZ, blockType, rightVertices);
				}

				if (facesExposed & TOP_FACE)
				{
					int vertexIndex = (counter++) * 48;
					addFace(vertexIndex, x, y, z, m_chunkX, m_chunkZ, blockType, topVertices);
				}

				if (facesExposed & BOTTOM_FACE)
				{
					int vertexIndex = (counter++) * 48;
					addFace(vertexIndex, x, y, z, m_chunkX, m_chunkZ, blockType, bottomVertices);
				}

			}
		}
	}

	if (!m_meshUpdateCalled) {
		m_meshUpdateCalled = true;
	} 
	else
	{
		glDeleteBuffers(1, &m_VBO);
		glDeleteVertexArrays(1, &m_VAO);
	}

	createVAO();
}
/* Unoptimised
void Chunk::addFace(int vertrexIndex, int x, int y, int z, int chunkX, int chunkZ, int blockType, const GLfloat vertices[])
{
	y = -y;
	memcpy(m_vertices + vertrexIndex, vertices, 192);

	//int blockType = 3;
	int textureX = blockType % 2;
	int textureY = blockType / 2;

	GLfloat textureXOffset = textureX * 0.5f;
	GLfloat textureYOffset = textureY * 0.5f;

	//Offset vertex values by position of block in the chunk + chunks position
	m_vertices[vertrexIndex] += (GLfloat)(x + chunkX * CHUNK_WIDTH);
	m_vertices[vertrexIndex + 8] += (GLfloat)(x + chunkX * CHUNK_WIDTH);
	m_vertices[vertrexIndex + 16] += (GLfloat)(x + chunkX * CHUNK_WIDTH);
	m_vertices[vertrexIndex + 24] += (GLfloat)(x + chunkX * CHUNK_WIDTH);
	m_vertices[vertrexIndex + 32] += (GLfloat)(x + chunkX * CHUNK_WIDTH);
	m_vertices[vertrexIndex + 40] += (GLfloat)(x + chunkX * CHUNK_WIDTH);

	m_vertices[vertrexIndex + 1] += (GLfloat)(y);
	m_vertices[vertrexIndex + 8 + 1] += (GLfloat)(y);
	m_vertices[vertrexIndex + 16 + 1] += (GLfloat)(y);
	m_vertices[vertrexIndex + 24 + 1] += (GLfloat)(y);
	m_vertices[vertrexIndex + 32 + 1] += (GLfloat)(y);
	m_vertices[vertrexIndex + 40 + 1] += (GLfloat)(y);

	m_vertices[vertrexIndex + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	m_vertices[vertrexIndex + 8 + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	m_vertices[vertrexIndex + 16 + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	m_vertices[vertrexIndex + 24 + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	m_vertices[vertrexIndex + 32 + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	m_vertices[vertrexIndex + 40 + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);


	
	m_vertices[vertrexIndex + 6] = m_vertices[vertrexIndex + 6] / TEXTURE_MAP_ROWS + textureXOffset;
	m_vertices[vertrexIndex + 7] = m_vertices[vertrexIndex + 7] / TEXTURE_MAP_ROWS + textureYOffset;

	m_vertices[vertrexIndex + 8 + 6] = m_vertices[vertrexIndex + 8 + 6] / TEXTURE_MAP_ROWS + textureXOffset;
	m_vertices[vertrexIndex + 8 + 7] = m_vertices[vertrexIndex + 8 + 7] / TEXTURE_MAP_ROWS + textureYOffset;

	m_vertices[vertrexIndex + 16 + 6] = m_vertices[vertrexIndex + 16 + 6] / TEXTURE_MAP_ROWS + textureXOffset;
	m_vertices[vertrexIndex + 16 + 7] = m_vertices[vertrexIndex + 16 + 7] / TEXTURE_MAP_ROWS + textureYOffset;

	m_vertices[vertrexIndex + 24 + 6] = m_vertices[vertrexIndex + 24 + 6] / TEXTURE_MAP_ROWS + textureXOffset;
	m_vertices[vertrexIndex + 24 + 7] = m_vertices[vertrexIndex + 24 + 7] / TEXTURE_MAP_ROWS + textureYOffset;

	m_vertices[vertrexIndex + 32 + 6] = m_vertices[vertrexIndex + 32 + 6] / TEXTURE_MAP_ROWS + textureXOffset;
	m_vertices[vertrexIndex + 32 + 7] = m_vertices[vertrexIndex + 32 + 7] / TEXTURE_MAP_ROWS + textureYOffset;

	m_vertices[vertrexIndex + 40 + 6] = m_vertices[vertrexIndex + 40 + 6] / TEXTURE_MAP_ROWS + textureXOffset;
	m_vertices[vertrexIndex + 40 + 7] = m_vertices[vertrexIndex + 40 + 7] / TEXTURE_MAP_ROWS + textureYOffset;
}
*/

void Chunk::addFace(int vertrexIndex, int x, int y, int z, int chunkX, int chunkZ, int blockType, const GLfloat vertices[])
{
	y = -y;
	memcpy(m_vertices + vertrexIndex, vertices, 192);

	int textureX = blockType % 2;
	int textureY = blockType / 2;

	GLfloat textureXOffset = textureX * 1.0f / TEXTURE_MAP_ROWS;
	GLfloat textureYOffset = textureY * 1.0f / TEXTURE_MAP_ROWS;

	GLfloat blockXPosition = x + chunkX * CHUNK_WIDTH;
	GLfloat blockZPosition = z + chunkZ * CHUNK_DEPTH;

	//Offset vertex values by position of block in the chunk + chunks position
	m_vertices[vertrexIndex] += (GLfloat)(x + chunkX * CHUNK_WIDTH);
	m_vertices[vertrexIndex = vertrexIndex + 1] += (GLfloat)(y);
	m_vertices[vertrexIndex = vertrexIndex + 1] += blockZPosition;
	m_vertices[vertrexIndex = vertrexIndex + 4] = m_vertices[vertrexIndex] / TEXTURE_MAP_ROWS + textureXOffset;
	m_vertices[vertrexIndex = vertrexIndex + 1] = m_vertices[vertrexIndex] / TEXTURE_MAP_ROWS + textureYOffset;
	m_vertices[vertrexIndex = vertrexIndex + 1] += blockXPosition;
	m_vertices[vertrexIndex = vertrexIndex + 1] += (GLfloat)(y);
	m_vertices[vertrexIndex = vertrexIndex + 1] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	m_vertices[vertrexIndex = vertrexIndex + 4] = m_vertices[vertrexIndex] / TEXTURE_MAP_ROWS + textureXOffset;
	m_vertices[vertrexIndex = vertrexIndex + 1] = m_vertices[vertrexIndex] / TEXTURE_MAP_ROWS + textureYOffset;
	m_vertices[vertrexIndex = vertrexIndex + 1] += blockXPosition;
	m_vertices[vertrexIndex = vertrexIndex + 1] += (GLfloat)(y);
	m_vertices[vertrexIndex = vertrexIndex + 1] += blockZPosition;
	m_vertices[vertrexIndex = vertrexIndex + 4] = m_vertices[vertrexIndex] / TEXTURE_MAP_ROWS + textureXOffset;
	m_vertices[vertrexIndex = vertrexIndex + 1] = m_vertices[vertrexIndex] / TEXTURE_MAP_ROWS + textureYOffset;
	m_vertices[vertrexIndex = vertrexIndex + 1] += blockXPosition;
	m_vertices[vertrexIndex = vertrexIndex + 1] += (GLfloat)(y);
	m_vertices[vertrexIndex = vertrexIndex + 1] += blockZPosition;
	m_vertices[vertrexIndex = vertrexIndex + 4] = m_vertices[vertrexIndex] / TEXTURE_MAP_ROWS + textureXOffset;
	m_vertices[vertrexIndex = vertrexIndex + 1] = m_vertices[vertrexIndex] / TEXTURE_MAP_ROWS + textureYOffset;
	m_vertices[vertrexIndex = vertrexIndex + 1] += blockXPosition;
	m_vertices[vertrexIndex = vertrexIndex + 1] += (GLfloat)(y);
	m_vertices[vertrexIndex = vertrexIndex + 1] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	m_vertices[vertrexIndex = vertrexIndex + 4] = m_vertices[vertrexIndex] / TEXTURE_MAP_ROWS + textureXOffset;
	m_vertices[vertrexIndex = vertrexIndex + 1] = m_vertices[vertrexIndex] / TEXTURE_MAP_ROWS + textureYOffset;
	m_vertices[vertrexIndex = vertrexIndex + 1] += blockXPosition;
	m_vertices[vertrexIndex = vertrexIndex + 1] += (GLfloat)(y);
	m_vertices[vertrexIndex = vertrexIndex + 1] += blockZPosition;
	m_vertices[vertrexIndex = vertrexIndex + 4] = m_vertices[vertrexIndex] / TEXTURE_MAP_ROWS + textureXOffset;
	m_vertices[vertrexIndex = vertrexIndex + 1] = m_vertices[vertrexIndex] / TEXTURE_MAP_ROWS + textureYOffset;
}

void Chunk::createVAO()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//Copy Chunk data to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * 6 * m_faceCount, m_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(3);

	delete[] m_vertices;
}

void Chunk::updateFront(const Chunk *frontChunk)
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

void Chunk::updateBack(const Chunk *backChunk)
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

void Chunk::updateLeft(const Chunk *leftChunk)
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

void Chunk::updateRight(const Chunk *rightChunk)
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

void Chunk::updateBlockFront(const Chunk *frontChunk, int x, int y)
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

void Chunk::updateBlockBack(const Chunk *backChunk, int x, int y)
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

void Chunk::updateBlockLeft(const Chunk *leftChunk, int y, int z)
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

void Chunk::updateBlockRight(const Chunk *rightChunk, int y, int z)
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

void Chunk::addBlock(unsigned char type, int x, int y, int z)
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

	m_blocks[i].setRender(true);
	m_blocks[i].setType(type);

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

	updateSurroundingBlockFaces(x, y, z);
}

void Chunk::removeBlock(int x, int y, int z)
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

	m_blocks[i].setRender(false);
	m_blocks[i].setType(0);
	m_blocks[i].setExposedFaces(0);

	updateSurroundingBlockFaces(x, y, z);
}

void Chunk::updateBlockFront(int x, int y, int z)
{
	if (z < 0)
	{
		return;
	}

	int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	assert(i >= 0);
	assert(i < CHUNK_BLOCK_COUNT);
	unsigned int facesExposed = m_blocks[i].getExposedFaces();

	if (!m_blocks[i].getRender())
	{
		m_blocks[i].setExposedFaces(0);
		return;
	}
	assert(i + CHUNK_WIDTH >= 0);
	assert(i + CHUNK_WIDTH < CHUNK_BLOCK_COUNT);
	if (!m_blocks[i + CHUNK_WIDTH].getRender())
	{
		facesExposed |= BACK_FACE;
	}
	else
	{
		facesExposed &= ~BACK_FACE;
	}

	m_blocks[i].setExposedFaces(facesExposed);//
}

void Chunk::updateBlockBack(int x, int y, int z)
{
	if (z >= CHUNK_DEPTH)
	{
		return;
	}

	int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	assert(i >= 0);
	assert(i < CHUNK_BLOCK_COUNT);
	unsigned int facesExposed = m_blocks[i].getExposedFaces();

	if (!m_blocks[i].getRender())
	{
		m_blocks[i].setExposedFaces(0);
		return;
	}
	assert(i - 1 >= 0);
	assert(i - 1 < CHUNK_BLOCK_COUNT);
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
	assert(i >= 0);
	assert(i < CHUNK_BLOCK_COUNT);
	unsigned int facesExposed = m_blocks[i].getExposedFaces();

	if (!m_blocks[i].getRender())
	{
		m_blocks[i].setExposedFaces(0);
		return;
	}
	assert(i + 1 >= 0);
	assert(i+1 < CHUNK_BLOCK_COUNT);
	if (!m_blocks[i + 1].getRender())
	{
		facesExposed |= RIGHT_FACE;
	}
	else
	{
		facesExposed &= ~RIGHT_FACE;
	}

	m_blocks[i].setExposedFaces(facesExposed);
}

void Chunk::updateBlockRight(int x, int y, int z)
{
	if (x >= CHUNK_WIDTH)
	{
		return;
	}

	int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	assert(i >= 0);
	assert(i < CHUNK_BLOCK_COUNT);
	unsigned int facesExposed = m_blocks[i].getExposedFaces();

	if (!m_blocks[i].getRender())
	{
		m_blocks[i].setExposedFaces(0);
		return;
	}
	assert(i - 1 >= 0);
	assert(i-1 < CHUNK_BLOCK_COUNT);
	if (!m_blocks[i - 1].getRender())
	{
		facesExposed |= LEFT_FACE;
	}
	else
	{
		facesExposed &= ~LEFT_FACE;
	}

	m_blocks[i].setExposedFaces(facesExposed);
}

void Chunk::updateBlockTop(int x, int y, int z)
{
	assert(!(y >= CHUNK_HEIGHT - 1));

	if (y < 0)
	{
		return;
	}
	//std::cout << "updateBlockTop" << std::endl;

	int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	assert(i >= 0);
	assert(i < CHUNK_BLOCK_COUNT);
	unsigned int facesExposed = m_blocks[i].getExposedFaces();

	if (!m_blocks[i].getRender())
	{
		m_blocks[i].setExposedFaces(0);
		return;
	}
	assert(i + CHUNK_WIDTH * CHUNK_DEPTH >= 0);
	assert(i + CHUNK_WIDTH * CHUNK_DEPTH < CHUNK_BLOCK_COUNT);
	if (!m_blocks[i + CHUNK_WIDTH * CHUNK_DEPTH].getRender())
	{
		facesExposed |= BOTTOM_FACE;
	}
	else
	{
		facesExposed &= ~BOTTOM_FACE;
	}

	m_blocks[i].setExposedFaces(facesExposed);
}

void Chunk::updateBlockBottom(int x, int y, int z)
{
	if (y >= CHUNK_HEIGHT)
	{
		return;
	}

	//std::cout << "updateBlock " << x << " " << y << " " << z << std::endl;

	int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
	assert(i >= 0);
	assert(i < CHUNK_BLOCK_COUNT);
	unsigned int facesExposed = m_blocks[i].getExposedFaces();

	if (!m_blocks[i].getRender())
	{
		m_blocks[i].setExposedFaces(0);
		return;
	}
	assert(i >= 0);
	assert(i < CHUNK_BLOCK_COUNT);
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

	hitBlock.x = hitBlock.x - (float)(m_chunkX * CHUNK_WIDTH);
	hitBlock.z = hitBlock.z - (float)(m_chunkZ * CHUNK_DEPTH);

	int x, y, z;
	x = (int)std::floor(hitBlock.x);
	z = (int)std::floor(hitBlock.z);
	y = (int)std::floor(hitBlock.y);
	y = -y;

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
	//checkBounds(i);
	// We hit a block
	if (m_blocks[i].getRender())
	{
		int j = blockHitPosition[0] + (blockHitPosition[2] * CHUNK_WIDTH) + (blockHitPosition[1] * CHUNK_WIDTH * CHUNK_DEPTH);
		//checkBounds(j);
		//std::cout << "hit " << hitBlock.x - (float)x << " " << hitBlock.y + (float)y << " " << hitBlock.z - (float)z << std::endl;

		//Depending on what face of the block hit
		//We need to find what block lies adjacent to it
		bool right = false;
		bool back = false;
		bool top = false;

		if (hitBlock.x - (float)x > 0.5f)
		{
			right = true;
		}

		if (hitBlock.z - (float)z > 0.5f)
		{
			back = true;
		}

		if (hitBlock.y + (float)y > 0.5f)
		{
			top = true;
		}

		float relativeX = std::abs((hitBlock.x - 0.5f) - (float)x);
		float relativeY = std::abs((hitBlock.y - 0.5f) + (float)y);
		float relativeZ = std::abs((hitBlock.z - 0.5f) - (float)z);

		//std::cout << "relative " << relativeX << " " << relativeY << " " << relativeZ << std::endl;
	
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
		//std::cout << "hitBlock " << hitBlock.x << " " << hitBlock.y << " " << hitBlock.z << std::endl;
		//std::cout << "rayCastBlock " << x << " " << y << " " << z << std::endl;
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
	hitBlock.x = hitBlock.x - (float)(m_chunkX * CHUNK_WIDTH);
	hitBlock.z = hitBlock.z - (float)(m_chunkZ * CHUNK_DEPTH);

	int x, y, z;
	x = (int)std::floor(hitBlock.x);
	z = (int)std::floor(hitBlock.z);
	y = (int)std::floor(hitBlock.y);
	y = -y;

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
	assert(i >= 0);
	assert(i < CHUNK_BLOCK_COUNT);
	// We hit a block
	if (m_blocks[i].getRender())
	{
		//std::cout << "hit " << hitBlock.x - (float)x << " " << hitBlock.y + (float)y << " " << hitBlock.z - (float)z << std::endl;

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

	//updateBlock(value, blockHitPosition[0], blockHitPosition[1], blockHitPosition[2]);
	// Update block faces around this block
	updateSurroundingBlockFaces(blockHitPosition[0], blockHitPosition[1], blockHitPosition[2]);
}

void Chunk::updateSurroundingBlockFaces(int x, int y, int z)
{
	if (x >= CHUNK_WIDTH)
	{
		return;
	}

	if (x < 0)
	{
		return;
	}

	if (y >= CHUNK_HEIGHT)
	{
		return;
	}

	if (y < 0)
	{
		return;
	}

	if (z >= CHUNK_DEPTH)
	{
		return;
	}

	if (z < 0)
	{
		return;
	}
	updateBlockLeft(x - 1, y, z);
	updateBlockRight(x + 1, y, z);

	updateBlockFront(x, y, z - 1);
	updateBlockBack(x, y, z + 1);

	updateBlockTop(x, y - 1, z);
	//std::cout << x << " " << y+1 << " " << z << std::endl;
	updateBlockBottom(x, y + 1, z);
}

bool Chunk::hitBlock(glm::vec3& position)
{
	glm::vec3 blockPosition;
	blockPosition.x = position.x - (float)(m_chunkX * CHUNK_WIDTH);
	blockPosition.z = position.z - (float)(m_chunkZ * CHUNK_DEPTH);

	//std::cout << "block " <<  << std::endl;

	int x, y, z;
	x = (int)std::floor(blockPosition.x);
	z = (int)std::floor(blockPosition.z);
	y = (int)std::floor(position.y);
	y = -y;
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

	if (m_blocks[i].getRender())
	{
		return true;
	}
 
	return false;
}

void Chunk::saveBlock(int x, int y, int z)
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
	
	m_savedBlocks[i].setRender(true);
	m_savedBlocks[i].setType(m_blocks[i].getType());
	m_modified |= true;
	
}

int Chunk::getChunkX()
{
	return m_chunkX;
}

int Chunk::getChunkZ()
{
	return m_chunkZ;
}
