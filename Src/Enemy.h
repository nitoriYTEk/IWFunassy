#pragma once

# include "DynamicObject.h"

class Enemy : public DynamicObject {
public:
	Enemy(int x, int y, int xsize, int ysize);

	bool isEnemy() const { return true; }
	bool isBullet() const { return false; }
};

class Needle : public Enemy {
public:
	Needle(int dir, int x, int y, int xsize, int ysize);

	virtual void update(int elapsed);
	void colision(const vector<shared_ptr<Object>>& stat, const vector<shared_ptr<DynamicObject>>& dynamic);

	void draw(const s3d::Rect& view) {
		if (area.intersects(view))
			ptex->rotate((direction * Math::HalfPi)).draw(area.movedBy(-view.pos).pos);
	}

	static Texture texture;
protected:
	int direction;
};

class FlyingNeedle : public Needle {
public:
	FlyingNeedle(int dir, int x, int y, int xsize, int ysize);

	void update(int elapsed);
private:
	bool standby;
};

class FallingPeer : public Enemy {
public:
	FallingPeer(int x, int y, int xsize, int ysize);

	void update(int elapsed);
	void colision(const vector<shared_ptr<Object>>& stat, const vector<shared_ptr<DynamicObject>>& dynamic);

	static Texture texture;
private:
	bool standby;
};