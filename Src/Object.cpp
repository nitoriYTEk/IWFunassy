#include "Object.h"

Texture Block::stone;
Texture Block::soil;
Texture Block::grass;

Object::Object(int x, int y, int xsize, int ysize)
{
	dieFlag = false;
	id = Random(1, 1000000);
	area = Rect(x - xsize / 2, y - ysize / 2, xsize, ysize);
}

Block::Block(Texture* tex, int x, int y, int xsize, int ysize) : Object(x, y, xsize, ysize)
{
	ptex = tex;
}