#pragma once

# include "DynamicObject.h"

class Bullet : public DynamicObject{
public:
	Bullet(bool player, const Vec2& v, int x, int y, int xsize, int ysize);

	void update(int elapsed);
	void colision(const vector<shared_ptr<Object>>& stat, const vector<shared_ptr<DynamicObject>>& dynamic);

	bool isEnemy() const { return false; }
	bool isBullet() const { return true; }

	bool isPlayer;

	static Texture texture;
private:
};