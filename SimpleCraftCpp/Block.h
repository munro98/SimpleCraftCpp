#pragma once

#define FRONT_FACE  00000001
#define BACK_FACE   00000010
#define LEFT_FACE   00000100
#define RIGHT_FACE  00001000
#define BOTTOM_FACE 00010000
#define TOP_FACE    00100000
#define TRANSPARENT 01000000

typedef struct {
	bool saved;
	unsigned char type;
} SavedBlock;

class Block
{
public:
	Block();
	~Block();

	void setExposedFaces(unsigned short facesExposed);
	unsigned short getExposedFaces();
	void setRender(bool render);
	void setType(unsigned short type);
	bool getRender();
	unsigned char getType();
private:
	bool m_render;
	unsigned short m_exposedFaces;
	unsigned char m_type;

};
