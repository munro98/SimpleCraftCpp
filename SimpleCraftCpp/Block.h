#pragma once

#define FRONT_FACE  00000001
#define BACK_FACE   00000010
#define LEFT_FACE   00000100
#define RIGHT_FACE  00001000
#define BOTTOM_FACE 00010000
#define TOP_FACE    00100000

class Block
{
public:
	Block();
	~Block();

	void Update();

	void updateBlock(Block * frontBlock, Block * backBlock, Block * leftBlock, Block * rightBlock);

	void updateFront(Block * frontBlock);

	void updateBack(Block * backBlock);

	void updateLeft(Block * leftBlock);

	void updateRight(Block * rightBlock);

	void setExposedFaces(unsigned int facesExposed);
	unsigned int getExposedFaces();
	void setRender(bool render);

	bool getRender();
private:
	unsigned int m_ExposedFaces;
	bool m_Render;

};
