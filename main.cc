#include "raylib.h"
#include "box2d.h"
#include <cstdio>
#include <cmath>
#include <vector>
#include "helpers.hh"
#include "box.hh"
#include "suika.hh"
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

	b2Vec2 gravity(0.0f, -40.0f);
	shared_ptr<b2World> world(new b2World(gravity));
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	b2Body *groundBody = world->CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 1.0f);

	groundBody->CreateFixture(&groundBox, 0.0f);

	SuikaFactory sf;
	vector<shared_ptr<GE>> boxes;
	vector<shared_ptr<Box>> walls;
	walls.push_back(shared_ptr<Box>(new Box(20, 40, 20, 720, true)));
	walls.back()->init(world);
	walls.push_back(shared_ptr<Box>(new Box(window_width - 40, 40, 20, 720, true)));
	walls.back()->init(world);
	boxes.push_back(dynamic_pointer_cast<GE>(shared_ptr<Box>((new Box(300, 100, 20, 20)))));
	boxes.push_back(dynamic_pointer_cast<GE>(shared_ptr<Box>((new Box(100, 100, 20, 20)))));
	boxes.push_back(dynamic_pointer_cast<GE>(shared_ptr<Box>((new Box(300, 300, 20, 20)))));
	boxes.push_back(dynamic_pointer_cast<GE>(shared_ptr<Box>((new Box(250, 100, 40, 40)))));
	boxes.push_back(dynamic_pointer_cast<GE>(
		sf.create(Small,250,60)
	));
	boxes.push_back(dynamic_pointer_cast<GE>(
		sf.create(Large,200,60)
	));

	for (int i = 0; i < boxes.size(); i++)
	{
		boxes[i]->init(world);
	}

	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	int fps = 120;
	SetTargetFPS(fps);
	while (!WindowShouldClose())
	{

		BeginDrawing();
		ClearBackground(RAYWHITE);
		world->Step(1/float(fps), velocityIterations, positionIterations);

		for (auto wall : walls)
		{
			wall->update();
			wall->draw();
		}
		for (int i = 0; i < boxes.size(); i++)
		{
			boxes[i]->update();
			boxes[i]->draw();
		}

		Vector2 mouse = GetMousePosition();
		static float growth = 0;
		static bool limit = false;
		static int delta = 0;
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			if (delta > 0 || !limit)
			{
				delta = 0;
				char str[100];
				snprintf(str, 100, "%4.2f %4.2f", mouse.x, mouse.y);
				DrawText(str, 20, 20, 20, BLACK);
				boxes.insert(boxes.begin(), dynamic_pointer_cast<GE>(shared_ptr<Box>(new Box(
												int(mouse.x - 10 - int(growth / 2)),
												int(mouse.y - 10 - int(growth / 2)),
												20 + int(growth),
												20 + int(growth)))));
				boxes.front()->init(world);
				growth += 0.0;
				if (boxes.size() > 1000)
				{
					boxes.pop_back();
				}
			}
			else
			{
				delta += 1;
			}
		}
		drawGuideLines();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
