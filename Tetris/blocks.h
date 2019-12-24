#ifndef __BLOCKS_H__
#define __BLOCKS_H__
#include "baseblock.h"

class ExtendedBlock : public Block
{
private:
	int blockType;
	int color;
public:
	ExtendedBlock(int);
	virtual void Rotate();
	int GetBlockType() { return blockType; }
	int GetBlockColor() { return color; }
	void InitSpace();
	~ExtendedBlock();
};




#endif