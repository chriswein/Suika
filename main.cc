#include "raylib.h"
#include "box2d.h"
#include <cstdio>
#include <cmath>
#include <vector>
#include "helpers.hh"
#include "box.hh"
#include "suika.hh"
#include "contactlistener.hh"
#include "game.hh"

using namespace std;

void drawGuideLines()
{
	for (int i = 0; i < window_width; i = i + 20)
	{
		DrawLine(i, 0, i, window_height, GRAY);
	}
	for (int i = 0; i < window_height; i = i + 20)
	{
		DrawLine(0, i, window_width, i, LIGHTGRAY);
	}
}

void drawPoints(int points)
{
	char str[100];
	snprintf(str, 100, "Points:\t%i", points);
	DrawText(str, 20, 20, 48, BLACK);
}

int main(void)
{
	InitWindow(game_area_x, game_area_y, "Suika Game");

	Game game;
	b2Vec2 gravity(0.0f, -40.0f);
	shared_ptr<b2World> world(new b2World(gravity));
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	b2Body *groundBody = world->CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 1.0f);

	groundBody->CreateFixture(&groundBox, 0.0f);

	SuikaFactory sf;
	auto gm = shared_ptr<GEManager>(new GEManager);
	shared_ptr<GEManager> p(gm);
	// contactlistener rlistener = contactlistener(p);
	auto listener = make_shared<contactlistener>(contactlistener(p));

	vector<shared_ptr<Box>> walls;
	walls.push_back(shared_ptr<Box>(new Box(20+offset_x-100, 40+offset_y, 20, 720, true, world)));
	walls.push_back(shared_ptr<Box>(new Box(window_width+offset_x/2-100, 40+offset_y, 20, 720, true, world)));

	game.init(world, gm);
	world->SetContactListener((b2ContactListener *)listener.get());

	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	int fps = 60;
	SetTargetFPS(fps);
	int points = 0;

	unordered_map<Fruits::GE_Type, int> pointstable = {
		{Fruits::BOX, 0},
		{Fruits::TANGERINE, 1},
		{Fruits::ORANGE, 2},
		{Fruits::GRAPEFRUIT, 4},
		{Fruits::MELON, 8},
		{Fruits::SUIKA, 32},
	};
	auto calcPoints = [&pointstable](Fruits::GE_Type type)
	{
		if (pointstable.find(type) != pointstable.end())
		{
			return pointstable[type];
		}
		return 0;
	};

	while (!WindowShouldClose())
	{

		BeginDrawing();
		ClearBackground(RAYWHITE);
		world->Step(1 / float(fps), velocityIterations, positionIterations);

		for (auto wall : walls)
		{
			wall->update();
			wall->draw();
		}

		for (auto pair : gm->geMap)
		{
			pair.second->update();
			pair.second->draw();
			points += calcPoints(pair.second->id()->type);
		}

		game.update(listener, gm, world);

		Vector2 mouse = GetMousePosition();
		static float growth = 0;
		static bool limit = true;
		static float delta = 0;
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			if (delta > 1.0 || !limit)
			{
				delta = 0.0;
				 auto melon = SuikaFactory::create(getNextMelon(), mouse.x-offset_x, 20, world);
				// auto melon = SuikaFactory::create(Fruits::TANGERINE, mouse.x, 20, world);
				gm->insertGE(melon);
			}
			else
			{
				delta += GetFrameTime();
			}
		}
		// drawGuideLines();
		drawPoints(points);
		points = 0;
		EndDrawing();
	}

	CloseWindow();

	world->SetContactListener((b2ContactListener *)nullptr); //
	return 0;
}
