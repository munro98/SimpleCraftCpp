#include "ChunkPosition.h"

ChunkPosition::ChunkPosition(int x, int z): x(x), z(z)
{
}

ChunkPosition::~ChunkPosition()
{
}

bool ChunkPosition::operator== (const ChunkPosition& other) const
{
	return (x == other.x) && (z == other.z);
}


bool ChunkPosition::operator!= (const ChunkPosition& other) const
{
	return !((x == other.x) && (z == other.z));
}