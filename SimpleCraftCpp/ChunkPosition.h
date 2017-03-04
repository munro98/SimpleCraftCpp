#pragma once
#include <functional>
#include <cmath>

#include "Hash.h"

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
			return Hash::hash(v.x, v.z);
		}
	};
}