
#include "box.hh"

Box::Box(int x, int y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->stat = false;
}
Box::Box(int x, int y, int w, int h, bool staticBox)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->stat = staticBox;
}
Box::~Box()
{
	if (world != nullptr && this->bodyref != nullptr)
	{
		world->DestroyBody(this->bodyref);
	}
}
void Box::init(shared_ptr<b2World> world)
{
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

b2Body *Box::body()
{
	return this->bodyref;
}
void Box::draw()
{
	Rectangle rec{float(this->x), float(this->y), float(this->w), float(this->h)};
	Vector2 origin{float(this->w / 2), float(this->h / 2)};
	DrawRectanglePro(rec, origin, this->angle, MAROON);
}
void Box::update()
{
	b2Vec2 position = this->bodyref->GetPosition();
	this->angle = this->bodyref->GetAngle() * (180.0 / M_PI);
	this->x = pixelXFromBox2d(position.x);
	this->y = pixelYFromBox2d(position.y);
	char str[100];
	// snprintf(str, 100, "%i %i %4.2f %4.2f\n %4.2f %4.2f", this->x, this->y,
	// 		 position.x, position.y, widthInBox2d(this->w) / 2, heightInBox2d(this->h) / 2);
	// DrawText(str, 20 + this->x, 20 + this->y, 10, BLACK);
}
