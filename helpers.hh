#pragma once
#include "box2d.h"
#include <memory>
#define DEBUG true
using namespace std;

const int window_width = 800;
const int window_height = 800;
const float aspect = float(window_width) / float(window_height);
const float b2_world_width = 20.0;
const float b2_world_height = 20.0;
const float b2_w_conv = b2_world_width / window_width;
const float b2_h_conv = b2_world_height / window_height;


float widthInBox2d(int w);

float heightInBox2d(int h);

float coordinateXInBox2d(int x);
float coordinateYInBox2d(int y);

int pixelXFromBox2d(float x);
int pixelYFromBox2d(float y);

class GE
{
public:
	 virtual void init(shared_ptr<b2World> world){};
	 virtual b2Body* body(){return nullptr;};
	 virtual void draw(){};
	 virtual void update(){};
};
