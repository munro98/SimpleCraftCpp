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

	void setExposedFaces(unsigned int facesExposed);
	unsigned int getExposedFaces();
	void setRender(bool render);

	bool getRender();
private:
	bool mIsExposed;
	unsigned int mExposedFaces;
	bool mRender;

};
