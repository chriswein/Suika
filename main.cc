#include "raylib.h"
#include "box2d.h"
#include <cstdio>  
#include <cmath>

const int window_width = 450;
const int window_height = 800;
const float b2_world_width = 2*4.0;
const float b2_world_height = 2*10.0;
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
	if (middle > b2_world_width/2){
		middle = -1*(middle-b2_world_width/2);
	}else{
		middle = middle - b2_world_width/2;
	}

	return middle;
}
float b2Y(int y){
	float middle = b2W(y);
	if (middle > b2_world_height/2){
		middle = -1*(middle - b2_world_height/2);
	}else{
		middle = (middle-b2_world_height/2);	
	}
	return middle;
}

int X2b(float x){
	float a = (abs(x)/(b2_world_width/2))*(window_width/2);
	if (x < 0) {
		a = window_width/2-a;
	}else{
		a = window_width+a;
	}
	return a;//int(middle+middle*x); 
}
int Y2b(float y){
	float a = (10+y)/b2_world_height;
	
	return a*window_height;//int(middle+middle*x); 
}

class Box : public GE {
	    b2Body* bodyref; 
		int x; int y; int w; int h;
	public: 
	    	Box(int x, int y, int w, int h){
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
		void init(b2World * world){
		    float x = b2X(this->x)-(b2W(this->w)/2);
		    float y = b2Y(this->y)-(b2H(this->h)/2);
		    b2BodyDef bodyDef;
		    bodyDef.type = b2_dynamicBody;
		    bodyDef.position.Set(x,y);
		    bodyref = world->CreateBody(&bodyDef);
		    b2PolygonShape dynamicBox;
		    dynamicBox.SetAsBox(this->w*b2_w_conv/2, this->h*b2_h_conv/2);
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
			 DrawRectangle(this->x, this->y, this->w, this->h, MAROON);
		}
		void update(){
		    b2Vec2 position = this->bodyref->GetPosition();
		    this->x = X2b(position.x);
		    this->y = Y2b(position.y);
		    char str[100];
		    sprintf(str, "%i %i %4.2f %4.2f", this->x,this->y, position.x, position.y);
		     DrawText(str, 10+this->x, 10+this->y, 40, BLACK);
		    printf("%i %i",this->x,this->y);
		}

};


int main(void)
{
    InitWindow(window_width,window_height, "Suika Game");

    b2Vec2 gravity(0.0f, -10.0f);
    b2World world(gravity);
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    
    groundBody->CreateFixture(&groundBox, 0.0f);
    
    Box box(20,20,40,40);
    box.init(&world);
    const int max = 4;
     //for (int i = 0; i<max; i++) {
    	 //Box b(20*i, 20*i, 40,40);
	 //b.init(&world);
	 //boxes[i] = &b;
     //}
	Box a(200,200,20,20);
	a.init(&world);
	Box b(400,200,20,20);
	b.init(&world);
	Box c(200,600,20,20);
	c.init(&world);
	Box d(400,600,20,20);
	d.init(&world);

    int32 velocityIterations = 6;
    int32 positionIterations = 2;
	SetTargetFPS(1);
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
	    b2Body * body = box.body();
	    //world.Step(GetFrameTime(), velocityIterations, positionIterations);
	    b2Vec2 position = body->GetPosition();
	     //float angle = body->GetAngle();
	     //printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	    box.update();
	    box.draw();
	    a.update();a.draw();
	    b.update();b.draw();
	    c.update();c.draw();
	    d.update();d.draw();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
