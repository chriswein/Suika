#pragma once
#include "suika.hh"
#include "raylib.h"
#include "textures.hh"
#include <math.h>

void DrawNextMelon(Fruits::GE_Type melon);
void DrawEvolution();
void DrawMousePosition(int x);
void DrawGameOverProgress(float percent_to_game_over);
void DrawDeadZone(int end);
void DrawGruideLines();
void DrawPoints(int points);