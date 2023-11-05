#pragma once
#include "helpers.hh"
#include "box2d.h"
#include "raylib.h"
#include <memory>
using namespace std;

class Box : public GE
{
	b2Body* bodyref;
	shared_ptr<b2World> world;
	int x;
	int y;
	int w;
	int h;
	float angle;
	bool stat;

public:
	Box(int x, int y, int w, int h);
	Box(int x, int y, int w, int h, bool staticBox);
	~Box();
	void init(shared_ptr<b2World> world);

	b2Body* body();
	void draw();
	void update();
};