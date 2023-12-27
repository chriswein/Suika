#include "gui.hh"

void DrawMelon(int x, int y, Fruits::GE_Type next, float scale)
{
    using namespace Fruits;
    int base = 10 * scale;
    float r = 0;
    Color c;
    Textures t;

    switch (next)
    {
    case GRAPE:
        r = 1 * base;
        c = PINK;
        break;
    case CHERRY:
        r = 1.5 * base;
        c = GRAY;
        break;
    case STRAWBERRY:
        r = 2 * base;
        c = LIGHTGRAY;
        break;
    case TANGERINE:
        r = 2.5 * base;
        c = PURPLE;
        break;
    case ORANGE:
        r = 3 * base;
        c = YELLOW;
        break;
    case GRAPEFRUIT:
        r = 3.5 * base;
        c = BLUE;
        break;
    case MELON:
        r = 4 * base;
        c = BLACK;
        break;
    case SUIKA:
        r = 4.5 * base;
        c = GREEN;
        break;
    default:
        break;
    }
    auto texture = Textures().get((int)next);
    if (texture != nullptr)
    {
        DrawTexturePro(
            *texture,
            {0.0, 0.0, float(texture->width), float(texture->height)},
            {float(x), float(y), float(r * 2), float(r * 2)},
            {float(r), float(r)},
            0.0,
            WHITE);
    }
    else
    {
        DrawCircle(x, y, r, c);
    }
}

void DrawNextMelon(Fruits::GE_Type next)
{
    constexpr int x = 1100;
    constexpr int y = 100;
    DrawText("Next: ", x - 80, y - 70, 24, BLACK);
    DrawMelon(x, y, next, 1.0f);
}

void DrawEvolution()
{

    const int steps = GE_Type_Max - 1;
    constexpr int width = 60;
    constexpr int height = 60;
    constexpr int x = 1100;
    constexpr int y = 600;
    constexpr float angle_step = 360.0f / steps+1;
    constexpr float angle = -(angle_step) * (PI / 180.0f);
    constexpr float turn_by = 90.0 * (PI / 180.0f);
    constexpr float dynamic_angle = 0.98f;
    DrawText("Evolution: ", x - 80, y - 120, 24, BLACK);
    for (size_t i = 0; i <= steps; i++)
    {
        DrawMelon(
            x + int(width * sin(turn_by + angle * (i + 1) * pow(dynamic_angle, i))),
            y + int(height * cos(turn_by + angle * (i + 1) * pow(dynamic_angle, i))),
            (Fruits::GE_Type)(i + 1),
            0.75f);
    }
}

/**
 * Draw a vertical line to indicate where a fruit will spawn.
*/
void DrawMousePosition(int x)
{
    Vector2 a{float(x), float(offset_y)};
    Vector2 b{float(x), float(game_area_y)};
    DrawLineV(a, b, LIGHTGRAY);
    a.x -= 1;
    b.x -= 1;
    DrawLineV(a, b, LIGHTGRAY);
    a.x += 2;
    b.x += 2;
    DrawLineV(a, b, LIGHTGRAY);
}

void DrawGameOverProgress(float percent_to_game_over)
{
    constexpr int width = 50;
    constexpr int x = 60;
    constexpr int y = 750;
    DrawText("GameOver", x / 2, y - width - 30, 20, BLACK);
    DrawCircleSector({x, y}, width, 0.0 - 90.0, 360.0 * percent_to_game_over - 90.0, 40, RED);
}

void DrawDeadZone(int end)
{
    DrawRectangle(offset_x, offset_y, game_area_x, end, {255, 255, 255, 123});
}

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
