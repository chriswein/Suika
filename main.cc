#include "raylib.h"
#include "box2d.h"
#include <cstdio>  

class GE{
	public:
		void init(b2World * world);
		b2Body* body();
		void draw();
		void update();
};

class Box : public GE {
	    b2Body* bodyref; 
	public: 
		void init(b2World * world){
		    b2BodyDef bodyDef;
		    bodyDef.type = b2_dynamicBody;
		    bodyDef.position.Set(0.0f, 4.0f);
		    bodyref = world->CreateBody(&bodyDef);
		    b2PolygonShape dynamicBox;
		    dynamicBox.SetAsBox(1.0f, 1.0f);
		    b2FixtureDef fixtureDef;
		    fixtureDef.shape = &dynamicBox;
		    fixtureDef.density = 1.0f;
		    fixtureDef.friction = 0.3f;
		    bodyref->CreateFixture(&fixtureDef);
		}

		b2Body* body(){
			return this->bodyref;
		}
		void draw(){
		}
		void update(){}

};

int main(void)
{
    InitWindow(450,800, "Suika Game");

    b2Vec2 gravity(0.0f, -10.0f);
    b2World world(gravity);
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    
    groundBody->CreateFixture(&groundBox, 0.0f);
    
    Box box;
    box.init(&world);
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
	    b2Body * body = box.body();
	    world.Step(GetFrameTime(), velocityIterations, positionIterations);
	    b2Vec2 position = body->GetPosition();
	    float angle = body->GetAngle();
	    printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
