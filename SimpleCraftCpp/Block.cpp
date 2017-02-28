#include "Block.h"
#include <glm/gtc/matrix_transform.inl>

Block::Block() : m_ExposedFaces(0), m_Render(false)
{
}

Block::~Block()
{
}

void Block::Update()
{
}

void Block::updateBlock(Block* frontBlock, Block* backBlock, Block* leftBlock, Block* rightBlock)
{
	updateFront(frontBlock);
	updateBack(backBlock);
	updateLeft(leftBlock);
	updateRight(rightBlock);
}

void Block::updateFront(Block* frontBlock)
{
	if (frontBlock->getRender())
	{
		m_ExposedFaces |= FRONT_FACE;
	}
	else
	{
		m_ExposedFaces &= ~FRONT_FACE;
	}
}

void Block::updateBack(Block* backBlock)
{
	if (backBlock->getRender())
	{
		m_ExposedFaces |= BACK_FACE;
	}
	else
	{
		m_ExposedFaces &= ~BACK_FACE;
	}
}

void Block::updateLeft(Block* leftBlock)
{
	if (leftBlock->getRender())
	{
		m_ExposedFaces |= LEFT_FACE;
	}
	else
	{
		m_ExposedFaces &= ~LEFT_FACE;
	}
}

void Block::updateRight(Block* rightBlock)
{
	if (rightBlock->getRender())
	{
		m_ExposedFaces |= RIGHT_FACE;
	}
	else
	{
		m_ExposedFaces &= ~RIGHT_FACE;
	}
}

void Block::setExposedFaces(unsigned int exposedFaces)
{
	m_ExposedFaces = exposedFaces;
}

unsigned int Block::getExposedFaces()
{
	return m_ExposedFaces;
}

void Block::setRender(bool render)
{
	m_Render = render;
}

bool Block::getRender()
{
	return m_Render;
}