#include "DynamicObject.h"
#include "Bullet.h"

DynamicObject::DynamicObject(int x, int y, int xsize, int ysize) : Object(x, y, xsize, ysize)
{
	velocity = { 0,0 };
}

void DynamicObject::update(int elapsed)
{
	if (isFloating)
		velocity += {0, Math::Ceil(GRAVITY * elapsed / 1000)};
	else if(velocity.y > 0)
		velocity.y= 0;

	velocity = { min(velocity.x, LIMIT_VELOCITY), min(velocity.y, LIMIT_VELOCITY) };

	Vec2 delta = { velocity * elapsed / 1000 };
	moveBy(Math::Ceil(delta));
}

shared_ptr<DynamicObject> DynamicObject::genObject()
{
	return nullptr;
}

void DynamicObject::colision(const vector<shared_ptr<Object>>& stat, const vector<shared_ptr<DynamicObject>>& dynamic)
{
	while (isGrounded(stat)) {
		if (velocity.y >= 0) {
			moveBy({ 0, -1 });
			isFloating = false;
		}
		if (velocity.y < 0) {
			moveBy({ 0, 1 });
		}
	}

	for (auto& d : dynamic) {
		if (intersects(*d)) {
			if (d->isBullet()) {
				auto p = static_pointer_cast<Bullet>(d);
				if(p->isPlayer == isEnemy())
					dieFlag = true;
			}
		}
	}
}

bool DynamicObject::isGrounded(const vector<shared_ptr<Object>>& block)
{
	for (auto& b : block) {
		if (intersects(*b))
			return true;
	}

	return false;
}
