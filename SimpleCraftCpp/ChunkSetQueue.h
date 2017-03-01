#pragma once
#include <queue>
#include <unordered_set>

#include "ChunkPosition.h"

class ChunkSetQueue
{
public:
	ChunkSetQueue();
	~ChunkSetQueue();
	void add(ChunkPosition chunk);
	ChunkPosition pop();
	bool empty();
private:
	std::queue<ChunkPosition> m_chunkPositionQueue;
	std::unordered_set<ChunkPosition> m_chunkPositionSet;
};

