#include "raylib.h"
#include "box2d.h"
#include <cstdio>
#include <cmath>
#include <vector>
#include <unordered_map>
#include "helpers.hh"
#include "box.hh"
#include "suika.hh"
#include "contactlistener.hh"
#include "game.hh"
#include "gui.hh"

using namespace std;
constexpr int dead_zone_begin = 80;

unordered_map<Fruits::GE_Type, int> pointstable = {
	{Fruits::BOX, 0},
	{Fruits::GRAPE, 1},
	{Fruits::CHERRY, 2},
	{Fruits::STRAWBERRY, 6},
	{Fruits::TANGERINE, 12},
	{Fruits::ORANGE, 24},
	{Fruits::GRAPEFRUIT, 48},
	{Fruits::MELON, 96},
	{Fruits::SUIKA, 128},
};

void DrawGuideLines()
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

void DrawPoints(int points)
{
	char string[100];
	snprintf(string, 100, "Points:\t%i", points);
	DrawText(string, 20, 20, 48, BLACK);
}

typedef struct GameOverStruct
{
	bool gameover = false;
	float percent_to_game_over = 0.0f;
} GameOverStruct;

GameOverStruct IsGameOver(int highest, int delta)
{
	constexpr int time_to_game_over = 6000;
	static int time_in_dead_zone = 0;

	if (highest <= dead_zone_begin)
		time_in_dead_zone += delta;
	else
		time_in_dead_zone = 0;

	if (time_in_dead_zone > time_to_game_over)
		return {true, 1.0f};
	return {false, (float(time_in_dead_zone) / float(time_to_game_over))};
}

int main(void)
{
	InitWindow(window_width, window_height, "Suika Game");
	InitAudioDevice();
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	Game game;
	b2Vec2 gravity(0.0f, -40.0f);
	shared_ptr<b2World> world(new b2World(gravity));

	SuikaFactory sf;
	shared_ptr<GEManager> game_manager = shared_ptr<GEManager>(new GEManager);
	// contactlistener rlistener = contactlistener(p);
	shared_ptr<contactlistener> listener = make_shared<contactlistener>(contactlistener(game_manager));

	vector<shared_ptr<Box>> walls;
	constexpr int w = 20;
	walls.push_back(shared_ptr<Box>(new Box(0, 0, w, 820, true, world)));
	walls.push_back(shared_ptr<Box>(new Box(game_area_x - w, 0, w, 820, true, world)));
	walls.push_back(shared_ptr<Box>(new Box(0, 780, 800, 40, true, world)));

	game.init(world, game_manager);
	world->SetContactListener((b2ContactListener *)listener.get());

	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	int fps = 120;
	SetTargetFPS(fps);
	int points = 0;
	bool is_game_over = false;

	// Calc points
	auto getPointsForType = [&](Fruits::GE_Type type)
	{if (pointstable.find(type) != pointstable.end()) return pointstable[type];
		return 0; };

	while (!WindowShouldClose() && !is_game_over)
	{

		BeginDrawing();
		ClearBackground(RAYWHITE);
		world->Step(1 / float(fps), velocityIterations, positionIterations);

		Vector2 mouse = GetMousePosition();
		if (!(mouse.x < offset_x + w || mouse.x > offset_x + game_area_x - w))
			DrawMousePosition(mouse.x);
		for (auto wall : walls) // Update walls
		{
			wall->update();
			wall->draw();
		}
		int highest = 800;
		for (auto pair : game_manager->geMap) // Update all game elements
		{
			pair.second->update();
			pair.second->draw();
			points += getPointsForType(pair.second->id()->type);
			highest = min(highest, pair.second->getPosition().y);
		}

		game.update(listener, game_manager, world);

		static float GROWTH = 0.0;
		static bool LIMIT = true;
		static float DELTA = 0.0;
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			int x = int(mouse.x);
			if (DELTA > 0.7 || !LIMIT) // You can spawn a new fruit
			{
				if (!(x < offset_x + w || x > offset_x + game_area_x - w)) // Guard case to ensure click is inside area.
				{
					DELTA = 0.0;
					shared_ptr<Suika> melon =
						SuikaFactory::create(getNextMelon(), x - offset_x, 20, world);
					game_manager->insertGE(melon);
				}
			}
			else
			{
				DELTA += GetFrameTime();
			}
		}
		// DrawGuideLines();
		DrawDeadZone(dead_zone_begin);
		DrawPoints(points);
		DrawNextMelon(previewNext());
		DrawEvolution();
		GameOverStruct gos = IsGameOver(highest, int(GetFrameTime() * 1000));
		is_game_over = gos.gameover;
		DrawGameOverProgress(gos.percent_to_game_over);
		
		points = 0;
		EndDrawing();
	}
	CloseAudioDevice(); // Close audio device
	CloseWindow();

	world->SetContactListener((b2ContactListener *)nullptr); //
	return 0;
}
