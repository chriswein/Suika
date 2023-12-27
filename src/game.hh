#include "contactlistener.hh"
#include "suika.hh"
#include "helpers.hh"
#include "raylib.h"
#include "box2d.h"
#include <algorithm>

class Game
{
private:
    static int points;
   Sound sound;  
public:
    Game(/* args */);
    ~Game();
    void update(shared_ptr<contactlistener> contacts, shared_ptr<GEManager> gm,shared_ptr<b2World> world);
    void init(shared_ptr<b2World> world,shared_ptr<GEManager> gm);
};

typedef struct GameOverStruct
{
	bool gameover = false;
	float percent_to_game_over = 0.0f;
} GameOverStruct;

GameOverStruct IsGameOver(int highest, int delta);

constexpr int dead_zone_begin = 80;