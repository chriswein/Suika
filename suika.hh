#pragma once
#include "box.hh"
#include "box2d.h"
#include "raylib.h"
#include <random>

class Suika : public Box
{
private:
    /* data */
    
    
public:
    float radius;
    Color color = RED;
    Suika(int x, int y, int radius, shared_ptr<b2World> world);
    ~Suika();
    void init(shared_ptr<b2World> world) override;
    void draw() override;
    void changeType(Fruits::GE_Type type);
};

enum Melon{
    Small,
    Middle,
    Large,
    Giant
}; 
static int MELONLENGTH = Melon::Giant;

Melon getNextMelon();

class SuikaFactory {
    public: 
        static shared_ptr<Suika> create(Melon melon, int x , int y, shared_ptr<b2World> world);
};