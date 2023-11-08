#include "suika.hh"

Suika::Suika(int x, int y, int radius) : Box(x,y,radius,radius)
{
   this->radius = radius;
}

Suika::~Suika()
{
}
void Suika::init(shared_ptr<b2World> world){
	float x = coordinateXInBox2d(this->x+this->w/2);
	float y = coordinateYInBox2d(this->y+this->h/2);
	float w = widthInBox2d(this->w);
	float h = heightInBox2d(this->h);
	
	this->world = world;
	b2BodyDef bodyDef;
	if (!this->stat)
		bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);

	this->bodyref = world->CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(w / 2, h / 2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	if (!this->stat)
	{
		fixtureDef.density = 100.0f;
		fixtureDef.friction = 0.3f;
	}

	this->bodyref->CreateFixture(&fixtureDef);
}

void Suika::draw(){
	Rectangle rec{float(this->x), float(this->y), float(this->w), float(this->h)};
	Vector2 origin{float(this->w / 2), float(this->h / 2)};
	DrawRectanglePro(rec, origin, this->angle, PURPLE);
}