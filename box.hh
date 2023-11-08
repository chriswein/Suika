#pragma once
#include "helpers.hh"
#include "box2d.h"
#include "raylib.h"
#include <memory>
#include <random>
using namespace std;

class Box : public GE
{

public:	
	b2Body* bodyref;
	shared_ptr<b2World> world;
	int x;
	int y;
	int w;
	int h;
	float angle;
	bool stat;
	Box(int x, int y, int w, int h);
	Box(int x, int y, int w, int h, bool staticBox);
	~Box();
	void init(shared_ptr<b2World> world) override;
	b2Body* body() override;
	void draw() override;
	void update() override;
};