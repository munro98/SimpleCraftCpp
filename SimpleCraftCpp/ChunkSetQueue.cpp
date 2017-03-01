#include "ChunkSetQueue.h"



ChunkSetQueue::ChunkSetQueue()
{
}


ChunkSetQueue::~ChunkSetQueue()
{
}

void ChunkSetQueue::add(ChunkPosition chunk)
{
	if (m_chunkPositionSet.find(chunk) == m_chunkPositionSet.end())
	{
		m_chunkPositionQueue.push(chunk);
		m_chunkPositionSet.insert(chunk);
	}
}

ChunkPosition ChunkSetQueue::pop()
{
	ChunkPosition result = m_chunkPositionQueue.front();
	m_chunkPositionSet.erase(m_chunkPositionQueue.front());
	m_chunkPositionQueue.pop();
	return result;
}

bool ChunkSetQueue::empty()
{
	return m_chunkPositionQueue.empty();
}
