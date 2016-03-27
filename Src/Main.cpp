# include <Siv3D.hpp>
# include <vector>
# include <memory>

#include "Object.h"
#include "DynamicObject.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

using namespace std;

vector<shared_ptr<Object>> object;
vector<shared_ptr<Object>> staticObject;
vector<shared_ptr<DynamicObject>> dynamicObject;
shared_ptr<Player> player;

shared_ptr<Rect> viewRect;

Image CreateSkyImage()
{
	PerlinNoise noise;
	Image image(640, 960);

	for (auto p : step(image.size))
		image[p.y][p.x] = HSV(220, 0.9 * noise.octaveNoise0_1(p.x / 160.0, Abs(p.y / 120.0 - 4.0), 8), 0.8);

	return image;
}

void initTexture() {
	Bullet::texture = Texture(L"Assets/bullet.png");
	Needle::texture = Texture(L"Assets/needle.png");
	FallingPeer::texture = Texture(L"Assets/peer.png");

	Player::stand = Texture(L"Assets/funassyi_stand.png");
	Player::jump = Texture(L"Assets/funassyi_jump.png");
	Player::walk[0] = Texture(L"Assets/funassyi_move1.png");
	Player::walk[1] = Texture(L"Assets/funassyi_move2.png");
	Player::attack = Texture(L"Assets/funassyi_attack.png");

	Block::stone = Texture(L"Assets/stone.png");
	Block::soil = Texture(L"Assets/soil.png");
	Block::grass = Texture(L"Assets/grass.png");
}

void loadStage() {

	object.clear();
	staticObject.clear();
	dynamicObject.clear();

	TextReader reader(L"Mapdata/stage1.txt");

	String line;

	int j = 0;
	while (reader.readLine(line))
	{
		int i = 0;
		for (auto& c : line) {
			shared_ptr<Object> o;
			shared_ptr<Enemy> e;

			switch (c)
			{
			case L'.':
				break;
			case L'#':
				o = make_shared<Block>(&Block::stone, i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				staticObject.push_back(o);
				object.push_back(o);
				break;
			case L'=':
				o = make_shared<Block>(&Block::soil, i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				staticObject.push_back(o);
				object.push_back(o);
				break;
			case L'w':
				o = make_shared<Block>(&Block::grass,i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				staticObject.push_back(o);
				object.push_back(o);
				break;
			case L'@':
				player = make_shared<Player>(i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				dynamicObject.push_back(player);
				object.push_back(player);
				break;
			case L'u':
				e = make_shared<Needle>(0, i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				dynamicObject.push_back(e);
				object.push_back(e);
				break;
			case L'd':
				e = make_shared<Needle>(2, i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				dynamicObject.push_back(e);
				object.push_back(e);
				break;
			case L'r':
				e = make_shared<Needle>(1, i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				dynamicObject.push_back(e);
				object.push_back(e);
				break;
			case L'l':
				e = make_shared<Needle>(3, i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				dynamicObject.push_back(e);
				object.push_back(e);
				break;
			case L'U':
				e = make_shared<FlyingNeedle>(0, i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				dynamicObject.push_back(e);
				object.push_back(e);
				break;
			case L'D':
				e = make_shared<FlyingNeedle>(2, i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				dynamicObject.push_back(e);
				object.push_back(e);
				break;
			case L'R':
				e = make_shared<FlyingNeedle>(1, i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				dynamicObject.push_back(e);
				object.push_back(e);
				break;
			case L'L':
				e = make_shared<FlyingNeedle>(3, i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				dynamicObject.push_back(e);
				object.push_back(e);
				break;
			case L'$':
				e = make_shared<FallingPeer>(i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE / 2, BLOCK_SIZE / 2);
				dynamicObject.push_back(e);
				object.push_back(e);
				break;
			default:
				break;
			}
			i++;
		}
		j++;
	}

	for (auto& d : dynamicObject) {
		d->checkFloating(staticObject);
	}
}

void Main()
{
	Window::SetTitle(L"I wanna be the FUNASSY");

	bool isTitle = true;
	Texture title(L"Assets/title.png");
	Texture title2(L"Assets/title2.png");

	viewRect = make_shared<Rect>(0, 0, 640, 480);

	const Texture texture(CreateSkyImage());
	const Font font(10);

	Stopwatch s(true);
	int lasttime;

	initTexture();

	loadStage();

	lasttime = s.ms();

	Texture over(L"Assets/over.png");

	while (System::Update())
	{
		int nowtime = s.ms();
		int elapsed = nowtime - lasttime;
		lasttime = nowtime;

		if (isTitle) {
			title.draw();
			if (nowtime % 800 < 400)
				title2.drawAt(320, 400);

			if (Input::AnyKeyClicked()) {
				isTitle = false;
			}
		}
		else {
			if (!player->dieFlag) {
				player->inputMove(elapsed, staticObject);

				{
					vector<shared_ptr<DynamicObject>> buff1;
					vector<shared_ptr<Object>> buff2;
					for (auto& d : dynamicObject) {
						d->update(elapsed);
						auto p = d->genObject();
						if (p != nullptr) {
							buff1.push_back(p);
							buff2.push_back(p);
						}
					}

					dynamicObject.insert(dynamicObject.end(), buff1.begin(), buff1.end());
					object.insert(object.end(), buff2.begin(), buff2.end());
				}

				for (auto& d : dynamicObject) {
					d->colision(staticObject, dynamicObject);
				}

				object.erase(remove_if(object.begin(), object.end(), [&](const shared_ptr<Object>& v) {return v->dieFlag; }), object.end());
				dynamicObject.erase(remove_if(dynamicObject.begin(), dynamicObject.end(), [&](const shared_ptr<DynamicObject>& v) {return v->dieFlag; }), dynamicObject.end());

				auto ppos = player->getCenter();
				if (ppos.x < viewRect->pos.x + viewRect->size.x / 4)
					viewRect->pos.x = ppos.x - viewRect->size.x / 4;
				if (viewRect->pos.x + viewRect->size.x * 3 / 4 < ppos.x)
					viewRect->pos.x = ppos.x - viewRect->size.x * 3 / 4;

				if (ppos.y < viewRect->pos.y + viewRect->size.y / 4)
					viewRect->pos.y = ppos.y - viewRect->size.y / 4;
				if (viewRect->pos.y + viewRect->size.y * 3 / 4 < ppos.y)
					viewRect->pos.y = ppos.y - viewRect->size.y * 3 / 4;
			}

			texture.draw();

			for (auto& o : object) {
				o->draw(*viewRect);
			}

			if (player->dieFlag) {
				if (Input::KeyR.clicked) {
					loadStage();
				}

				over.draw();
			}

			font(L"fps:", Profiler::FPS()).draw();
		}
	}
}