#include "gui.hh"

void DrawMelon(int x, int y, Fruits::GE_Type next, float scale){
    using namespace Fruits;
    int base = 10*scale;
    float r = 0;
    Color c;

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
     DrawCircle(x, y, r, c);
}

void DrawNextMelon(Fruits::GE_Type next)
{
    constexpr int x = 1100;
    constexpr int y = 100;
    DrawText("Next: ",x-80,y-70,24,BLACK);
    DrawMelon(x,y,next,1.0f);
}

void DrawEvolution(){
    
    const int steps = GE_Type_Max-1;
    constexpr int width = 60;
    constexpr int height = 60;
    constexpr int x = 1100;
    constexpr int y = 600;
    constexpr float angle = -(360.0f/steps)*(PI/180.0f); 
    DrawText("Evolution: ",x-80,y-120,24,BLACK);
    for (size_t i = 0; i <= steps; i++)
    {
        DrawMelon(
            x+int(width*sin(angle*i)),
            y+int(height*cos(angle*i)),
            (Fruits::GE_Type)(i+1),
            0.75f
        );
    }
}

void DrawMousePosition(int x){
    Vector2 a{float(x),float(offset_y)};
    Vector2 b{float(x),float(game_area_y)};
    DrawLineV(a,b, LIGHTGRAY);
    a.x-=1;
    b.x-=1;
    DrawLineV(a,b,LIGHTGRAY);
    a.x+=2;
    b.x+=2;
    DrawLineV(a,b,LIGHTGRAY);
}

void DrawGameOverProgress(float percent_to_game_over){
    constexpr int width = 50;
    constexpr int x = 60;
    constexpr int y = 750;
    DrawText("GameOver",x/2,y-width-30, 20,BLACK);
    DrawCircleSector({x,y},width,0.0-90.0,360.0*percent_to_game_over-90.0,40, RED);
}

void DrawDeadZone(int end){
    DrawRectangle(offset_x,offset_y, game_area_x,end, {255,255,255,123});
}
