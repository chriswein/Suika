#pragma once
#include "helpers.hh"
#include "box2d.h"
#include "raylib.h"
#include <memory>
#include <random>
using namespace std;

class Box : public GE
{
protected:
	shared_ptr<GE_Id> gid;
public:	
	b2Body* bodyref;
	shared_ptr<b2World> world;
	int x;
	int y;
	int w;
	int h;
	float angle;
	bool stat;
	Box(int x, int y, int w, int h, shared_ptr<b2World> world);
	Box(int x, int y, int w, int h, bool staticBox, shared_ptr<b2World> world);
	~Box();
	void init(shared_ptr<b2World> world) override;
	b2Body* body() override;
	void draw() override;
	void update() override;
	shared_ptr<GE_Id> id() override;
};