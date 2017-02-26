#include "Block.h"
#include <glm/gtc/matrix_transform.inl>

Block::Block() : mExposedFaces(0), mRender(false)
{
	///*
	int random = rand() % 100;
	if (random > 50)
	{
		mRender = true;
	}
	//*/
	//mRender = true;
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
		mExposedFaces |= FRONT_FACE;
	}
	else
	{
		mExposedFaces &= ~FRONT_FACE;
	}
}

void Block::updateBack(Block* backBlock)
{
	if (backBlock->getRender())
	{
		mExposedFaces |= BACK_FACE;
	}
	else
	{
		mExposedFaces &= ~BACK_FACE;
	}
}

void Block::updateLeft(Block* leftBlock)
{
	if (leftBlock->getRender())
	{
		mExposedFaces |= LEFT_FACE;
	}
	else
	{
		mExposedFaces &= ~LEFT_FACE;
	}
}

void Block::updateRight(Block* rightBlock)
{
	if (rightBlock->getRender())
	{
		mExposedFaces |= RIGHT_FACE;
	}
	else
	{
		mExposedFaces &= ~RIGHT_FACE;
	}
}

void Block::setExposedFaces(unsigned int exposedFaces)
{
	mExposedFaces = exposedFaces;
}

unsigned int Block::getExposedFaces()
{
	return mExposedFaces;
}

void Block::setRender(bool render)
{
	mRender = render;
}

bool Block::getRender()
{
	return mRender;
}