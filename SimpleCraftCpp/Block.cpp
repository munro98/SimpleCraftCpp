#include "Block.h"
#include <glm/gtc/matrix_transform.inl>

Block::Block() : m_exposedFaces(0), m_render(false), m_type(1)
{
}

Block::~Block()
{
}

void Block::setExposedFaces(unsigned int exposedFaces)
{
	m_exposedFaces = exposedFaces;
}

unsigned int Block::getExposedFaces()
{
	return m_exposedFaces;
}

void Block::setRender(bool render)
{
	m_render = render;
}

void Block::setType(unsigned char type)
{
	m_type = type;
}

bool Block::getRender()
{
	return m_render;
}

unsigned char Block::getType()
{
	return m_type;
}