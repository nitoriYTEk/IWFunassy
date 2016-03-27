#include "Player.h"
#include "Bullet.h"

Texture Player::stand;
Texture Player::jump;
Texture Player::walk[2];
Texture Player::attack;

Player::Player(int x, int y, int xsize, int ysize) : DynamicObject(x, y, xsize, ysize)
{
	left = false;
	attackCount = 0;
	walking = 0;
	prevodd = 0;

	secondJump = false;
}

void Player::inputMove(int elapsed, const vector<shared_ptr<Object>>& stat){
 	if (Input::KeyZ.clicked) {
		if (!isFloating) {
			secondJump = true;
			velocity.y = -500;
			isFloating = true;
		}
		else if (secondJump) {
			secondJump = false;
			velocity.y = -500;
			isFloating = true;
		}
	}


	if (Input::KeyRight.pressed) {
		left = false;
		moveBy({ BLOCK_SIZE * 5 * elapsed / 1000, 0 });
		while (isGrounded(stat)) {
			moveBy({ -1, 0 });
		}
		checkFloating(stat);

		prevodd = (prevodd + elapsed) % 500;
		walking = true;
	}
	else if (Input::KeyLeft.pressed) {
		left = true;
		moveBy({ -BLOCK_SIZE * 5 * elapsed / 1000, 0 });
		while (isGrounded(stat)) {
			moveBy({ 1, 0 });
		}
		checkFloating(stat);

		prevodd = (prevodd + elapsed) % 500;
		walking = true;
	}
	else {
		walking = false;
	}
}

void Player::update(int elapsed)
{
	DynamicObject::update(elapsed);
}

shared_ptr<DynamicObject> Player::genObject()
{
	if (Input::KeyX.clicked) {
		attackCount = 20;
		return make_shared<Bullet>(true, left ? Vec2(-320, 0) : Vec2(320, 0), getCenter().x, getCenter().y, 8, 8);
	}
	else
		return nullptr;
}

void Player::colision(const vector<shared_ptr<Object>>& stat, const vector<shared_ptr<DynamicObject>>& dynamic)
{
	DynamicObject::colision(stat, dynamic);

	for (auto& d : dynamic) {
		if (intersects(*d)) {
			if (d->isEnemy()) {
				dieFlag = true;
			}
		}
	}

}
