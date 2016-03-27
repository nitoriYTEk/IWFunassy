#pragma once

# include <vector>
# include <memory>

#include "Object.h"

constexpr double GRAVITY = 1200;
constexpr double LIMIT_VELOCITY = 800;

using namespace std;

class DynamicObject : public Object {
public:
	DynamicObject(int x, int y, int xsize, int ysize);

	void checkFloating(const vector<shared_ptr<Object>>& block) {
		isFloating = true;
		for (auto& b : block) {
			if (!intersects(*b) && area.movedBy(0, 1).intersects(b->getArea()))
				isFloating = false;
		}
	}

	virtual void update(int elapsed);
	virtual shared_ptr<DynamicObject> genObject();
	virtual void colision(const vector<shared_ptr<Object>>& stat, const vector<shared_ptr<DynamicObject>>& dynamic);

	bool isGrounded(const vector<shared_ptr<Object>>& block);
protected:
	Vec2 velocity;

	void moveBy(const s3d::Vec2& v) { area.moveBy(v.x, v.y); }
	bool isFloating;
};