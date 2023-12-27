#include "raylib.h"
#include "box2d.h"
#include <cstdio>
#include <cmath>
#include <vector>
#include <unordered_map>
#include "./src/helpers.hh"
#include "./src/box.hh"
#include "./src/suika.hh"
#include "./src/contactlistener.hh"
#include "./src/game.hh"
#include "./src/gui.hh"
#include "./src/textures.hh"

using namespace std;

unordered_map<Fruits::GE_Type, int> pointstable = {
	{Fruits::BOX, 0},
	{Fruits::GRAPE, 1},
	{Fruits::CHERRY, 2},
	{Fruits::STRAWBERRY, 6},
	{Fruits::TANGERINE, 12},
	{Fruits::ORANGE, 24},
	{Fruits::GRAPEFRUIT, 48},
	{Fruits::MELON, 96},
	{Fruits::SUIKA, 300},
};

int main(void)
{
	InitWindow(window_width, window_height, "Suika Game");
	InitAudioDevice();
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// game variables
	Game game;
	SuikaFactory sf;
	Wave d = LoadWave("./data/spawn.wav");
	auto spawn = LoadSoundFromWave(d);
	vector<shared_ptr<Box>> walls;

	// Calc points function
	auto GetPointsForTypeLambda = [&](Fruits::GE_Type type)
	{	
		if (pointstable.find(type) != pointstable.end()) return pointstable[type];
		return 0; 
	};

	// box2d parameters
	constexpr int32 velocityIterations = 6;
	constexpr int32 positionIterations = 2;
	constexpr int fps = 120;
	b2Vec2 gravity(0.0f, -40.0f);
	shared_ptr<b2World> world(new b2World(gravity));

	/*
		LOAD TEXTURES
	*/

	Textures *t = new Textures();
	t->load("./data/melonen_11.png", Fruits::GE_Type::GRAPE);
	t->load("./data/melonen_01.png", Fruits::GE_Type::CHERRY);
	t->load("./data/melonen_02.png", Fruits::GE_Type::STRAWBERRY);
	t->load("./data/melonen_06.png", Fruits::GE_Type::TANGERINE);
	t->load("./data/melonen_08.png", Fruits::GE_Type::ORANGE);
	t->load("./data/melonen_09.png", Fruits::GE_Type::GRAPEFRUIT);
	t->load("./data/melonen_10.png", Fruits::GE_Type::MELON);
	t->load("./data/melonen_11.png", Fruits::GE_Type::SUIKA);

	// This manages the game elements that need drawing and updating.
	shared_ptr<GEManager> game_manager = shared_ptr<GEManager>(new GEManager);

	// Interface to box2d for collision detection.
	shared_ptr<contactlistener> listener = make_shared<contactlistener>(contactlistener(game_manager));

	// Initialize walls
	constexpr int w = 20;
	walls.push_back(shared_ptr<Box>(new Box(0, 0, w, 820, true, world)));
	walls.push_back(shared_ptr<Box>(new Box(game_area_x - w, 0, w, 820, true, world)));
	walls.push_back(shared_ptr<Box>(new Box(0, 780, 800, 40, true, world)));

	// Game init
	game.init(world, game_manager);
	world->SetContactListener((b2ContactListener *)listener.get());
	int points = 0;
	bool is_game_over = false;

	// Raylib init
	SetTargetFPS(fps);
	RenderTexture2D texture = LoadRenderTexture(window_width, window_height);

	while (!WindowShouldClose() && !is_game_over)
	{
		// We draw to a canvas that we can scale to any size.
		BeginTextureMode(texture);
		ClearBackground(RAYWHITE);

		world->Step(1 / float(fps), velocityIterations, positionIterations);

		Vector2 mouse = GetMousePosition();
		if (!(mouse.x < offset_x + w || mouse.x > offset_x + game_area_x - w))
			DrawMousePosition(mouse.x);
		
		// Update walls
		for (auto wall : walls) 
		{
			wall->update();
			wall->draw();
		}
		int highest = 800;

		// Update all game elements
		for (auto pair : game_manager->geMap) 
		{
			pair.second->update();
			pair.second->draw();
			points += GetPointsForTypeLambda(pair.second->id()->type);

			// Calculate which fruit is highest on screen.
			highest = min(highest, pair.second->getPosition().y);
		}

		// Update the game logic 
		game.update(listener, game_manager, world);


		static float GROWTH = 0.0;
		static bool LIMIT = true;
		static float DELTA_REFACTORY_SPAWN_TIME = 0.0;
		constexpr float DELTA_NEW_SPAWN_OK = 0.7f;

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			int x = int(mouse.x);
			if (DELTA_REFACTORY_SPAWN_TIME > DELTA_NEW_SPAWN_OK || !LIMIT) // You can spawn a new fruit
			{
				if (!(x < offset_x + w || x > offset_x + game_area_x - w)) // Guard case to ensure click is inside area.
				{
					DELTA_REFACTORY_SPAWN_TIME = 0.0;
					shared_ptr<Suika> melon =
						SuikaFactory::create(getNextMelon(), x - offset_x, 20, world);
					game_manager->insertGE(melon);
					PlaySound(spawn);
				}
			}
			else
			{
				DELTA_REFACTORY_SPAWN_TIME += GetFrameTime();
			}
		}
		#ifdef DEBUG
			DrawGuideLines();
		#endif
		DrawDeadZone(dead_zone_begin);
		DrawPoints(points);
		DrawNextMelon(previewNext());
		DrawEvolution();
		GameOverStruct gos = IsGameOver(highest, int(GetFrameTime() * 1000));
		is_game_over = gos.gameover;
		DrawGameOverProgress(gos.percent_to_game_over);

		points = 0;
		EndTextureMode();

		// Draw the frame
		BeginDrawing();
		DrawTexturePro(
			texture.texture,
			Rectangle{0, 0, static_cast<float>(texture.texture.width), static_cast<float>(-texture.texture.height)},
			Rectangle{0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())},
			Vector2{0, 0},
			0,
			WHITE);
		EndDrawing();
	}
	UnloadSound(spawn);
	CloseAudioDevice(); // Close audio device
	CloseWindow();

	world->SetContactListener((b2ContactListener *)nullptr); // fix for: box2d trys to call decontructor of already deleted class.
	return 0;
}
