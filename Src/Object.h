#pragma once
# include <Siv3D.hpp>

constexpr int BLOCK_SIZE = 32;

constexpr int X_BLOCK_NUM = 640 / BLOCK_SIZE;
constexpr int Y_BLOCK_NUM = 480 / BLOCK_SIZE;

class Object {
public:
	Object(int x, int y, int xsize, int ysize);

	bool intersects(const Object& right) const {
		return id != right.getId() && area.intersects(right.getArea());
	}

	int getId() const { return id; }
	const Rect& getArea() const { return area; }
	Point getCenter() const { return area.pos + area.size / 2; }

	virtual void draw(const s3d::Rect& view){ 
		if (area.intersects(view))
			area.movedBy(-view.pos)(*ptex).draw();
	}

	virtual bool isEnemy() const = 0;
	virtual bool isBullet() const = 0;

	bool dieFlag;
protected:
	int id;
	Texture* ptex;
	Rect area;
};

class Block : public Object{
public:
	Block(Texture* tex, int x, int y, int xsize, int ysize);

	virtual bool isEnemy() const { return false; }
	virtual bool isBullet() const { return false; }

	static Texture stone;
	static Texture soil;
	static Texture grass;
};