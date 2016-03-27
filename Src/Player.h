#pragma once

# include "DynamicObject.h"

class Player : public DynamicObject{
public:
	Player(int x, int y, int xsize, int ysize);

	void inputMove(int elapsed, const vector<shared_ptr<Object>>& stat);
	void update(int elapsed);
	shared_ptr<DynamicObject> genObject();
	void colision(const vector<shared_ptr<Object>>& stat, const vector<shared_ptr<DynamicObject>>& dynamic);

	void draw(const s3d::Rect& view){
	if (area.intersects(view)) {
			auto& tex = 
				attackCount ? attack :
				isFloating ? jump : 
				walking ? walk[prevodd / 250] :
				stand;
			auto& mtex = tex.mirror();
			area.movedBy(-view.pos)(left ? tex : mtex).draw();
			if (attackCount > 0)attackCount--;
		}
	}
	
	bool isEnemy() const { return false; }
	bool isBullet() const { return false; }

	static Texture stand;
	static Texture jump;
	static Texture walk[2];
	static Texture attack;
private:
	bool secondJump;
	bool left;
	int attackCount;
	int prevodd;
	bool walking;
};