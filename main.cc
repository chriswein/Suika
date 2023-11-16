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

int main(void)
{
	InitWindow(window_width, window_height, "Suika Game");

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
	walls.push_back(shared_ptr<Box>(new Box(20, 40, 20, 720, true, world)));
	walls.push_back(shared_ptr<Box>(new Box(window_width - 40, 40, 20, 720, true, world)));

	game.init(world, gm);
	world->SetContactListener((b2ContactListener *)listener.get());

	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	int fps = 120;
	SetTargetFPS(fps);

	while (!WindowShouldClose())
	{

		BeginDrawing();
		ClearBackground(RAYWHITE);
		world->Step(1 / float(fps), velocityIterations, positionIterations);

		for (auto key : listener->GetDeletables()) // Remove all elements that where selected
		{
			gm->deleteGE(key);
		}

		game.update(listener, gm, world);

		for (auto wall : walls)
		{
			wall->update();
			wall->draw();
		}

		for (auto pair : gm->geMap)
		{
			pair.second->update();
			pair.second->draw();
		}

		Vector2 mouse = GetMousePosition();
		static float growth = 0;
		static bool limit = true;
		static float delta = 0;
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			if (delta > 1.0 || !limit)
			{
				delta = 0.0;
				// char str[100];
				// snprintf(str, 100, "%4.2f %4.2f", mouse.x, mouse.y);
				// DrawText(str, 20, 20, 20, BLACK);
				// auto element = dynamic_pointer_cast<GE>(shared_ptr<Box>(new Box(
				// 	int(mouse.x - 10 - int(growth / 2)),
				// 	int(mouse.y - 10 - int(growth / 2)),
				// 	20 + int(growth),
				// 	20 + int(growth),
				// 	world)));
				// gm->insertGE(element);

				// growth += 0.0;
				auto melon = SuikaFactory::create(getNextMelon(), mouse.x, 20, world);
				gm->insertGE(melon);
			}
			else
			{
				delta += GetFrameTime();
			}
		}
		drawGuideLines();
		EndDrawing();
	}

	CloseWindow();

	world->SetContactListener((b2ContactListener*)nullptr); // 
	return 0;
}
