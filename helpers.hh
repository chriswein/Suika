#pragma once
#include "box2d.h"
#include <memory>
#include <map>
using namespace std;

const int game_area_x = 800;
const int game_area_y = 800;
const int window_width = 1200;
const int window_height = 820;
const int offset_x = (window_width - game_area_x) / 2;
const int offset_y = (window_height - game_area_y) / 2;
const float aspect = float(game_area_x) / float(game_area_y);
const float b2_world_width = 20.0;
const float b2_world_height = 20.0;
const float b2_w_conv = b2_world_width / game_area_x;
const float b2_h_conv = b2_world_height / game_area_y;
static int GE_last_id = 0;

float widthInBox2d(int w);

float heightInBox2d(int h);

float coordinateXInBox2d(int x);
float coordinateYInBox2d(int y);

int pixelXFromBox2d(float x);
int pixelYFromBox2d(float y);

#undef ORANGE // Defined by Raylib

namespace Fruits
{
	enum GE_Type
	{
		BOX,
		GRAPE,
		CHERRY,
		STRAWBERRY,
		TANGERINE,
		ORANGE,
		GRAPEFRUIT,
		MELON,
		SUIKA
	};
}
const int GE_Type_Max = Fruits::GE_Type::SUIKA;

typedef struct GE_Id
{
	int id;
	Fruits::GE_Type type;
} GE_Id;

class GE
{
public:
	virtual ~GE(){};
	virtual void init(shared_ptr<b2World> world){};
	virtual b2Body *body() { return nullptr; };
	virtual void draw(){};
	virtual void update(){};
	virtual shared_ptr<GE_Id> id() { return shared_ptr<GE_Id>(nullptr); };
};

class GEManager
{

public:
	std::unordered_map<int, shared_ptr<GE>> geMap;
	GEManager();

	void insertGE(shared_ptr<GE> ge);
	void deleteGE(int id);
	void printAllGEs() const;
};