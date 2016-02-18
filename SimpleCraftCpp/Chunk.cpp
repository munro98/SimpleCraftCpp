#include <iostream>
#include <glm/mat4x2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Chunk.h"


Chunk::Chunk(): blocks(new Block[CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT]), mChunkX(0), mChunkZ(0), mFaceCount(0), mVertices(nullptr), mVBO(0), mVAO(0)
{

}

Chunk::~Chunk()
{
	delete[] blocks;
}

void Chunk::render(GLuint modelLocation, GLuint blockPositionLocation)
{
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mFaceCount);
	glBindVertexArray(0);

}

void Chunk::update( int chunkX, int chunkZ, Chunk* frontChunk, Chunk* backChunk, Chunk* leftChunk, Chunk* rightChunk)
{
	mChunkX = chunkX;
	mChunkZ = chunkZ;

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
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
					*/
			}
		}
	}

	mFaceCount = 0;

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
				if (!blocks[i].getRender())
					continue;

				unsigned int facesExposed = 0;
				
				if (y == 0)
				{
					facesExposed |= TOP_FACE;
				}

				if (i - CHUNK_WIDTH * CHUNK_DEPTH > -1 && !blocks[i - CHUNK_WIDTH * CHUNK_DEPTH].getRender())
				{
					facesExposed |= TOP_FACE;
				}

				if (i + CHUNK_WIDTH * CHUNK_DEPTH < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT && !blocks[i + CHUNK_WIDTH * CHUNK_DEPTH].getRender())
				{
					facesExposed |= BOTTOM_FACE;
				}

				if (leftChunk != nullptr && x == 0)
				{
					int j = CHUNK_WIDTH - 1 + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
					if (!leftChunk->blocks[j].getRender())
					{
						facesExposed |= LEFT_FACE;
					}
	
				}
				else if (x != 0 && i - 1 > -1 && !blocks[i - 1].getRender())
				{
					facesExposed |= LEFT_FACE;
				}

				///*

				if (rightChunk != nullptr && x == CHUNK_WIDTH - 1)
				{
					int j = 0 + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
					if (!rightChunk->blocks[j].getRender())
					{
						facesExposed |= RIGHT_FACE;
					}

				}
				//*/
				else if (x != CHUNK_WIDTH - 1 && i + 1 < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT && !blocks[i + 1].getRender())
				{
					facesExposed |= RIGHT_FACE;
				}
				//

				///*
				if (frontChunk != nullptr && z == 0)
				{
					int j = x + (CHUNK_DEPTH - 1) * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
					if (!frontChunk->blocks[j].getRender())
					{
						facesExposed |= FRONT_FACE;
					}

				}
				else if (z != 0 && i - CHUNK_WIDTH > -1 && !blocks[i - CHUNK_WIDTH].getRender())
				{
					facesExposed |= FRONT_FACE;
				}
				//*/

				if (backChunk != nullptr && z == CHUNK_DEPTH - 1)
				{
					//int j = x + 0 * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
					int j = x + 0 * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
					if (!backChunk->blocks[j].getRender())
					{
						facesExposed |= BACK_FACE;
					}

				}
				else if (z != CHUNK_DEPTH - 1 && z < CHUNK_DEPTH - 1 && i + CHUNK_WIDTH < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT && !blocks[i + CHUNK_WIDTH].getRender())
				{
					facesExposed |= BACK_FACE;
				}

				/////////////////////////////

				if (facesExposed & FRONT_FACE)
				{
					mFaceCount++;
				}

				if (facesExposed & BACK_FACE)
				{
					mFaceCount++;
				}

				if (facesExposed & LEFT_FACE)
				{
					mFaceCount++;
				}

				if (facesExposed & RIGHT_FACE)
				{
					mFaceCount++;
				}

				if (facesExposed & TOP_FACE)
				{
					mFaceCount++;
				}

				if (facesExposed & BOTTOM_FACE)
				{
					mFaceCount++;
				}
				//std::cout << "blocks[i] " << i << " " << std::endl;
				blocks[i].setExposedFaces(facesExposed);//
				
			}
		}
	}
	
	//std::cout << mFaceCount << std::endl;
	mVertices = new GLfloat[8 * 6 * mFaceCount];
	//mVertices = new GLfloat[48];
	//mVertices = new GLfloat[sizeof(topVertices) / sizeof(topVertices[0])];
	int counter = 0;

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;


				unsigned int facesExposed = blocks[i].getExposedFaces();

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

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	//Copy Vertex array to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * 6 * mFaceCount, mVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	delete[] mVertices;
}

void Chunk::update2()
{
	mFaceCount = 0;

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
				if (!blocks[i].getRender())
					continue;

				unsigned int facesExposed = blocks[i].getExposedFaces();


				/////////////////////////////

				if (facesExposed & FRONT_FACE)
				{
					mFaceCount++;
				}

				if (facesExposed & BACK_FACE)
				{
					mFaceCount++;
				}

				if (facesExposed & LEFT_FACE)
				{
					mFaceCount++;
				}

				if (facesExposed & RIGHT_FACE)
				{
					mFaceCount++;
				}

				if (facesExposed & TOP_FACE)
				{
					mFaceCount++;
				}

				if (facesExposed & BOTTOM_FACE)
				{
					mFaceCount++;
				}
				//std::cout << "blocks[i] " << i << " " << std::endl;
				//blocks[i].SetExposedFaces(facesExposed);//
			}
		}
	}

	std::cout << mFaceCount << std::endl;
	mVertices = new GLfloat[8 * 6 * mFaceCount];

	int counter = 0;

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				int i = x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;


				unsigned int facesExposed = blocks[i].getExposedFaces();

				if (facesExposed & FRONT_FACE)
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, mChunkX, mChunkZ, frontVertices);
				}

				if (facesExposed & BACK_FACE)
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, mChunkX, mChunkZ, backVertices);
				}

				if (facesExposed & LEFT_FACE)
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, mChunkX, mChunkZ, leftVertices);
				}

				if (facesExposed & RIGHT_FACE)
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, mChunkX, mChunkZ, rightVertices);
				}

				if (facesExposed & TOP_FACE) //  && z == 0
				{
					//int test = (x + z * CHUNK_WIDTH)* 48;
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, mChunkX, mChunkZ, topVertices);
				}

				if (facesExposed & BOTTOM_FACE)
				{
					int vertrexIndex = (counter++) * 48;
					addFace(vertrexIndex, x, y, z, mChunkX, mChunkZ, bottomVertices);
				}

			}
		}
	}

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	//Copy Chunk data to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * 6 * mFaceCount, mVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	delete[] mVertices;
}

void Chunk::updateFront(Chunk* frontChunk)
{
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			int i = x + 0 * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;

			if (!blocks[i].getRender())
			{
				continue;
			}

			unsigned int facesExposed = blocks[i].getExposedFaces();
			int j = x + (CHUNK_DEPTH - 1) * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
			if (!frontChunk->blocks[j].getRender())
			{
				facesExposed |= FRONT_FACE;
			}
			else
			{
				facesExposed &= ~FRONT_FACE;
			}
			blocks[i].setExposedFaces(facesExposed);
		}
	}
	update2();
}

void Chunk::updateBack(Chunk* backChunk)
{
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				int i = x + (CHUNK_DEPTH - 1) * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;

				if (!blocks[i].getRender())
				{
					continue;
				}

				unsigned int facesExposed = blocks[i].getExposedFaces();
				int j = x + 0 * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
				if (!backChunk->blocks[j].getRender())
				{
					facesExposed |= BACK_FACE;
				} else
				{
					facesExposed &= ~BACK_FACE;
				}

				blocks[i].setExposedFaces(facesExposed);
			}
	}
	update2();
}

void Chunk::updateLeft(Chunk* leftChunk)
{
	for (int z = 0; z < CHUNK_DEPTH; z++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			int i = 0 + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;

			if (!blocks[i].getRender())
			{
				continue;
			}

			unsigned int facesExposed = blocks[i].getExposedFaces();
			int j = (CHUNK_WIDTH - 1) + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
			if (!leftChunk->blocks[j].getRender())
			{
				facesExposed |= LEFT_FACE;
			}
			else
			{
				facesExposed &= ~LEFT_FACE;
			}
			blocks[i].setExposedFaces(facesExposed);
		}
	}
	update2();
}

void Chunk::updateRight(Chunk* rightChunk)
{
	for (int z = 0; z < CHUNK_DEPTH; z++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			int i = (CHUNK_WIDTH - 1) + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;

			if (!blocks[i].getRender())
			{
				continue;
			}

			unsigned int facesExposed = blocks[i].getExposedFaces();
			int j = 0 + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
			if (!rightChunk->blocks[j].getRender())
			{
				facesExposed |= RIGHT_FACE;
			}
			else
			{
				facesExposed &= ~RIGHT_FACE;
			}
			blocks[i].setExposedFaces(facesExposed);
		}
	}
	update2();
}

void Chunk::addFace(int vertrexIndex ,int x, int y, int z,int chunkX, int chunkZ, const GLfloat vertices[])
{
	y = -y;
	memcpy(mVertices + vertrexIndex, vertices, 192);
	//std::cout << "hit " << sizeof(vertices) << " " << std::endl;
	mVertices[vertrexIndex] += (GLfloat)(x + chunkX * CHUNK_WIDTH);
	mVertices[vertrexIndex + 8] += (GLfloat)(x + chunkX * CHUNK_WIDTH);
	mVertices[vertrexIndex + 16] += (GLfloat)(x + chunkX * CHUNK_WIDTH);
	mVertices[vertrexIndex + 24] += (GLfloat)(x + chunkX * CHUNK_WIDTH);
	mVertices[vertrexIndex + 32] += (GLfloat)(x + chunkX * CHUNK_WIDTH);
	mVertices[vertrexIndex + 40] += (GLfloat)(x + chunkX * CHUNK_WIDTH);

	mVertices[vertrexIndex + 1] += (GLfloat)(y);
	mVertices[vertrexIndex + 8 + 1] += (GLfloat)(y);
	mVertices[vertrexIndex + 16 + 1] += (GLfloat)(y);
	mVertices[vertrexIndex + 24 + 1] += (GLfloat)(y);
	mVertices[vertrexIndex + 32 + 1] += (GLfloat)(y);
	mVertices[vertrexIndex + 40 + 1] += (GLfloat)(y);

	mVertices[vertrexIndex + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	mVertices[vertrexIndex + 8 + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	mVertices[vertrexIndex + 16 + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	mVertices[vertrexIndex + 24 + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	mVertices[vertrexIndex + 32 + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
	mVertices[vertrexIndex + 40 + 2] += (GLfloat)(z + chunkZ * CHUNK_DEPTH);
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

	if (!blocks[i].getRender())
	{
		blocks[i].setExposedFaces(facesExposed);
		return;
	}
		
	if (y == 0)
	{
		facesExposed |= TOP_FACE;
	}

	if (i - CHUNK_WIDTH * CHUNK_DEPTH > -1 && !blocks[i - CHUNK_WIDTH * CHUNK_DEPTH].getRender())
	{
		facesExposed |= TOP_FACE;
	}

	if (i + CHUNK_WIDTH * CHUNK_DEPTH < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT && !blocks[i + CHUNK_WIDTH * CHUNK_DEPTH].getRender())
	{
		facesExposed |= BOTTOM_FACE;
	}

	if (x > 0 && i - 1 > -1 && !blocks[i - 1].getRender())
	{
		facesExposed |= LEFT_FACE;
	}

	if (x < CHUNK_WIDTH - 1 && i + 1 < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT && !blocks[i + 1].getRender())
	{
		facesExposed |= RIGHT_FACE;
	}

	if (z > 0 && i - CHUNK_WIDTH > -1 && !blocks[i - CHUNK_WIDTH].getRender())
	{
		facesExposed |= FRONT_FACE;
	}

	if (z < CHUNK_DEPTH - 1 && i + CHUNK_WIDTH < CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT && !blocks[i + CHUNK_WIDTH].getRender())
	{
		facesExposed |= BACK_FACE;
	}

	//std::cout << "blocks[i] " << i << " " << std::endl;
	blocks[i].setExposedFaces(facesExposed);//
}

void Chunk::rayCastBlock(RayCast& rayCast, glm::vec3 start, glm::vec3 forward)
{
	glm::vec3 vecIntersection;
	float fraction = 1.0f;
	static int count = 0;

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				if (blocks[x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH].getRender() && rayCast.lineBlockIntersection((float)(x + mChunkX * CHUNK_WIDTH), (float)-y, (float)(z + mChunkZ * CHUNK_DEPTH), start, start + forward  * 10.0f, vecIntersection, fraction)) {
					//std::cout << "hit " << count++ <<  " "  << std::endl;
				}
			}
		}

	}

	if (fraction < 1.0f) {

		vecIntersection = start + forward  * 10.0f * fraction - forward  * 0.01f;
		int x, y, z;
		x = (int)round(vecIntersection.x);
		z = (int)round(vecIntersection.z);
		y = (int)round(-vecIntersection.y);

		x -= mChunkX * CHUNK_WIDTH;
		z -= mChunkZ * CHUNK_DEPTH;

		if (x >=  CHUNK_WIDTH)
		{
			return;
		}

		if (x < 0)
		{
			return;
		}

		if (y < 0)
		{
			return;
		}

		if (z >=  CHUNK_DEPTH)
		{
			return;
		}

		if (z < 0)
		{
			return;
		}
		
		int i = x  + (z * CHUNK_WIDTH) + (y * CHUNK_WIDTH * CHUNK_DEPTH);

		//std::cout << x << " " << z << " " << y << " " << std::endl;

		blocks[i].setRender(true);

		updateBlock(x, y, z);

		updateBlock(x - 1, y, z);
		updateBlock(x + 1, y, z);

		updateBlock(x, y, z - 1);
		updateBlock(x, y, z + 1);

		updateBlock(x, y - 1, z);
		updateBlock(x, y + 1, z);

		update2();
	}
}

void Chunk::rayCastBlock2(RayCast& rayCast, glm::vec3 start, glm::vec3 forward, int* blockHitPosition)
{
	glm::vec3 vecIntersection;
	float fraction = 1.0f;
	static int count = 0;

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				if (blocks[x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH].getRender() && rayCast.lineBlockIntersection((float)(x + mChunkX * CHUNK_WIDTH), (float)-y, (float)(z + mChunkZ * CHUNK_DEPTH), start, start + forward  * 10.0f, vecIntersection, fraction)) {
					//std::cout << "hit " << count++ <<  " "  << std::endl;
				}
			}
		}

	}

	if (fraction < 1.0f) {

		vecIntersection = start + forward  * 10.0f * fraction - forward  * 0.01f;
		int x, y, z;
		x = (int)round(vecIntersection.x);
		z = (int)round(vecIntersection.z);
		y = (int)round(-vecIntersection.y);

		x -= mChunkX * CHUNK_WIDTH;
		z -= mChunkZ * CHUNK_DEPTH;

		if (x >= CHUNK_WIDTH)
		{
			return;
		}

		if (x < 0)
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

		blockHitPosition[0] = x;
		blockHitPosition[1] = y;
		blockHitPosition[2] = z;
		int i = x + (z * CHUNK_WIDTH) + (y * CHUNK_WIDTH * CHUNK_DEPTH);

		//std::cout << x << " " << z << " " << y << " " << std::endl;

		blocks[i].setRender(true);

		updateBlock(x, y, z);

		updateBlock(x - 1, y, z);
		updateBlock(x + 1, y, z);

		updateBlock(x, y, z - 1);
		updateBlock(x, y, z + 1);

		updateBlock(x, y - 1, z);
		updateBlock(x, y + 1, z);

		update2();
	}
}

void Chunk::rayCastBlockRemove(RayCast& rayCast, glm::vec3 start, glm::vec3 forward)
{
	glm::vec3 vecIntersection;
	float fraction = 1.0f;
	static int count = 0;
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				if (blocks[x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH].getRender() && rayCast.lineBlockIntersection((float)(x + mChunkX * CHUNK_WIDTH), (float)-y, (float)(z + mChunkZ * CHUNK_DEPTH), start, start + forward  * 10.0f, vecIntersection, fraction)) {
					//std::cout << "hit " << count++ << " " << std::endl;
				}
			}
		}

	}
	if (fraction < 1.0f) {
		vecIntersection = start + forward  * 10.0f * fraction + forward  * 0.01f;
		int x, y, z;
		x = (int)round(vecIntersection.x);
		z = (int)round(vecIntersection.z);
		y = (int)round(-vecIntersection.y);

		x -= mChunkX * CHUNK_WIDTH;
		z -= mChunkZ * CHUNK_DEPTH;

		///*
		if (x < 0)
		{
			return;
		}

		if (x >= CHUNK_WIDTH)
		{
			return;
		}

		if (y < 0)
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

		int i = x + (z * CHUNK_WIDTH) + (y * CHUNK_WIDTH * CHUNK_DEPTH);

		//std::cout << x << " " << z << " " << y << " " << i << std::endl;

		blocks[i].setRender(false);

		updateBlock(x, y, z);
		
		updateBlock(x - 1, y, z);
		updateBlock(x + 1, y, z);

		updateBlock(x, y, z - 1);
		updateBlock(x, y, z + 1);

		updateBlock(x, y - 1, z);
		updateBlock(x, y + 1, z);
		
		update2();
	}
}

void Chunk::rayCastBlockRemove2(RayCast& rayCast, glm::vec3 start, glm::vec3 forward, int* blockHitPosition)
{
	glm::vec3 vecIntersection;
	float fraction = 1.0f;
	static int count = 0;
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				if (blocks[x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH].getRender() && rayCast.lineBlockIntersection((float)(x + mChunkX * CHUNK_WIDTH), (float)-y, (float)(z + mChunkZ * CHUNK_DEPTH), start, start + forward  * 10.0f, vecIntersection, fraction)) {
					//std::cout << "hit " << count++ << " " << std::endl;
				}
			}
		}

	}
	if (fraction < 1.0f) {
		vecIntersection = start + forward  * 10.0f * fraction + forward  * 0.01f;
		int x, y, z;
		x = (int)round(vecIntersection.x);
		z = (int)round(vecIntersection.z);
		y = (int)round(-vecIntersection.y);

		x -= mChunkX * CHUNK_WIDTH;
		z -= mChunkZ * CHUNK_DEPTH;

		///*
		if (x < 0)
		{
			return;
		}

		if (x >= CHUNK_WIDTH)
		{
			return;
		}

		if (y < 0)
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

		blockHitPosition[0] = x;
		blockHitPosition[1] = y;
		blockHitPosition[2] = z;
		int i = x + (z * CHUNK_WIDTH) + (y * CHUNK_WIDTH * CHUNK_DEPTH);

		//std::cout << x << " " << z << " " << y << " " << i << std::endl;

		blocks[i].setRender(false);

		updateBlock(x, y, z);

		updateBlock(x - 1, y, z);
		updateBlock(x + 1, y, z);

		updateBlock(x, y, z - 1);
		updateBlock(x, y, z + 1);

		updateBlock(x, y - 1, z);
		updateBlock(x, y + 1, z);

		update2();
	}
}