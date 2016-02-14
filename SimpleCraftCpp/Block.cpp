#include "Block.h"
#include <glm/gtc/matrix_transform.inl>

Block::Block() : mExposedFaces(0), mRender(false), mIsExposed(false)
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

void Block::setExposedFaces(unsigned int exposedFaces)
{
	mExposedFaces = exposedFaces;
	if (mExposedFaces & FRONT_FACE || mExposedFaces & BACK_FACE || mExposedFaces & LEFT_FACE || mExposedFaces & RIGHT_FACE || mExposedFaces & BOTTOM_FACE || mExposedFaces & TOP_FACE)
		mIsExposed = true;
	else
		mIsExposed = false;
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