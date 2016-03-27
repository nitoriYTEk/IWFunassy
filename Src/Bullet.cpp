#include "Bullet.h"

extern shared_ptr<Rect> viewRect;

Texture Bullet::texture;

Bullet::Bullet(bool player, const Vec2& v, int x, int y, int xsize, int ysize) : DynamicObject(x, y, xsize, ysize)
{
	ptex = &texture;
	velocity = v;
	isPlayer = player;
}

void Bullet::update(int elapsed)
{
	moveBy({ velocity * elapsed / 1000 });
	if (!viewRect->intersects(getArea())) {
		dieFlag = true;
	}
}

void Bullet::colision(const vector<shared_ptr<Object>>& stat, const vector<shared_ptr<DynamicObject>>& dynamic)
{
	for (auto& d : dynamic) {
		if (intersects(*d)) {
			if (!d->isBullet()) {
				if (isPlayer == d->isEnemy())
					dieFlag = true;
			}
		}
	}
}
