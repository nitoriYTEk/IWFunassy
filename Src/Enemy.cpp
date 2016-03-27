#include "Enemy.h"
#include "Player.h"

extern shared_ptr<Player> player;
extern shared_ptr<Rect> viewRect;

Texture Needle::texture;
Texture FallingPeer::texture;

namespace {
	vector<Point> delta = {
		{0, -1},
		{1, 0},
		{0, 1},
		{-1, 0}
	};
}

Enemy::Enemy(int x, int y, int xsize, int ysize) : DynamicObject(x, y, xsize, ysize)
{

}

Needle::Needle(int dir, int x, int y, int xsize, int ysize) : Enemy(x, y, xsize, ysize)
{
	direction = dir;
	ptex = &texture;
}

void Needle::update(int elapsed)
{

}

void Needle::colision(const vector<shared_ptr<Object>>& stat, const vector<shared_ptr<DynamicObject>>& dynamic)
{

}

FlyingNeedle::FlyingNeedle(int dir, int x, int y, int xsize, int ysize) : Needle(dir, x, y, xsize, ysize)
{
	standby = true;
}

void FlyingNeedle::update(int elapsed)
{
	if (standby) {
		if (direction == 0 || direction == 2) {
			if (abs(getCenter().x - player->getCenter().x) <= BLOCK_SIZE * 1.5)
				standby = false;
		}
		if (direction == 1 || direction == 3) {
			if (abs(getCenter().y - player->getCenter().y) <= BLOCK_SIZE * 1.5)
				standby = false;
		}
	}
	else {
		moveBy(delta[direction] * 240 * elapsed / 1000);
		if (!viewRect->intersects(getArea())) {
			dieFlag = true;
		}
	}
}

FallingPeer::FallingPeer(int x, int y, int xsize, int ysize) : Enemy(x, y, xsize, ysize)
{
	ptex = &texture;
	standby = true;
}

void FallingPeer::update(int elapsed)
{
	if (!standby) {
		DynamicObject::update(elapsed);
	}
	else {
		if (abs(getCenter().x - player->getCenter().x) <= BLOCK_SIZE)
			standby = false;
	}
}

void FallingPeer::colision(const vector<shared_ptr<Object>>& stat, const vector<shared_ptr<DynamicObject>>& dynamic)
{
	if (!standby) {
		DynamicObject::colision(stat, dynamic);
	}
}
