#include "raylib.h"
#include "box2d.h"
#include <cstdio>  
#include <cmath>

const int window_width = 450;
const int window_height = 800;
const float b2_world_width = 20.0;
const float b2_world_height = 20.0;
const float b2_w_conv = b2_world_width / window_width; 
const float b2_h_conv = b2_world_height / window_height; 

class GE{
	public:
		void init(b2World * world);
		b2Body* body();
		void draw();
		void update();
};

float b2W(int w){
	return w*b2_w_conv;
}

float b2H(int h){
	return h*b2_h_conv;
}

float b2X(int x){
	float middle = b2W(x);
		middle = middle - b2_world_width/2;
	return middle;
}
float b2Y(int y){
	float middle = b2W(y);
		middle = (middle-b2_world_height/2);	
	return middle*-1;
}

int X2b(float x){
	if(x<0){
		float q = (b2_world_width/2)-abs(x);
		float a = (abs(q)/(b2_world_width/2))*(window_width/2);

	return a;
	}else{
		float q = x;
		float a = (abs(q)/(b2_world_width/2))*(window_width/2);
		a = (window_width/2)+a;
	return a; 
	}
}
int Y2b(float y){
	if(y>0){
		float q = (b2_world_height/2)-abs(y);
		float a = (abs(q)/(b2_world_height/2))*(window_height/2);

	return a; 
	}else{
		float q = y;
		float a = (abs(q)/(b2_world_height/2))*(window_height/2);
		a = (window_height/2)+a;
	return a; 
	}
}

class Box : public GE {
	    b2Body* bodyref; 
	    b2World* world;
		int x; int y; int w; int h;
	public: 
	    	Box(int x, int y, int w, int h){
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
		~Box(){
			if(world != nullptr){
				world->DestroyBody(this->bodyref);
			}
		}
		void init(b2World * world){
		    float x = b2X(this->x);
		    float y = b2Y(this->y);
		    b2BodyDef bodyDef;
		    bodyDef.type = b2_dynamicBody;
		    bodyDef.position.Set(x,y);
		    bodyref = world->CreateBody(&bodyDef);
		    b2PolygonShape dynamicBox;
		    dynamicBox.SetAsBox(b2W(this->w)/2, b2H(this->h)/2);
		    b2FixtureDef fixtureDef;
		    fixtureDef.shape = &dynamicBox;
		    fixtureDef.density = 1.0f;
		    fixtureDef.friction = 0.3f;
		    bodyref->CreateFixture(&fixtureDef);
		    this->world = world;
		}

		b2Body* body(){
			return this->bodyref;
		}
		void draw(){
			 DrawRectangle(this->x, this->y, this->w, this->h, MAROON);
		}
		void update(){
		    b2Vec2 position = this->bodyref->GetPosition();
		    this->x = X2b(position.x-b2W(this->w));
		    this->y = Y2b(position.y+b2H(this->h));
		    char str[100];
		    sprintf(str, "%i %i %4.2f %4.2f\n %4.2f %4.2f", this->x,this->y, 
				    position.x, position.y, b2W(this->w)/2 ,b2H(this->h)/2);
		     DrawText(str, 10+this->x, 10+this->y, 20, BLACK);
		}

};


int main(void)
{
    InitWindow(window_width,window_height, "Suika Game");

    b2Vec2 gravity(0.0f, -20.0f);
    b2World world(gravity);
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 1.0f);
    
    groundBody->CreateFixture(&groundBox, 0.0f);
    
    const int max = 4;
	Box a(100,100,20,20);
	a.init(&world);
	Box b(300,100,20,20);
	b.init(&world);
	Box c(300,300,20,20);
	c.init(&world);
	Box d(280,100,20,20);
	d.init(&world);

    int32 velocityIterations = 6;
    int32 positionIterations = 2;
	SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
	    world.Step(GetFrameTime(), velocityIterations, positionIterations);
	    a.update();a.draw();
	    b.update();b.draw();
	    c.update();c.draw();
	    d.update();d.draw();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
