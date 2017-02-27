#pragma once
#include <functional>
#include <cmath>

#include "Hasher.h"

class ChunkPosition
{
public:
	ChunkPosition(int x, int z);
	~ChunkPosition();
	bool operator==(const ChunkPosition & other) const;
	bool operator!=(const ChunkPosition & other) const;

	int x, z;
private:
	
};

namespace std
{
	template<>
	struct hash<ChunkPosition>
	{
		size_t operator() (ChunkPosition const& v) const
		{
			return Hasher::hash(v.x, v.z);
		}
	};
}