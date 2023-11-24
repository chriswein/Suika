#pragma once
#include "suika.hh"
// #include "helpers.hh"
#include "raylib.h"
#include <math.h>

void DrawNextMelon(Fruits::GE_Type melon);
void DrawEvolution();
void DrawMousePosition(int x);
void DrawGameOverProgress(float percent_to_game_over);
void DrawDeadZone(int end);