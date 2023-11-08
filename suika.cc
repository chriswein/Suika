#include "suika.hh"

Suika::Suika(int x, int y, int radius) : Box(x, y, radius / 2, radius / 2)
{
	this->radius = radius;
}

Suika::~Suika(){

}

void Suika::init(shared_ptr<b2World> world)
{
	float x = coordinateXInBox2d(this->x);// + this->w / 2);
	float y = coordinateYInBox2d(this->y);// + this->h / 2);

	this->world = world;
	b2BodyDef bodyDef;
	if (!this->stat)
		bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);

	this->bodyref = world->CreateBody(&bodyDef);

	b2CircleShape circle;
	circle.m_radius = heightInBox2d(this->radius);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	if (!this->stat)
	{
		fixtureDef.density = 200.0f;
		fixtureDef.friction = 0.6f;
	}

	this->bodyref->CreateFixture(&fixtureDef);
}

void Suika::draw()
{
	DrawCircle(this->x, this->y, this->radius, this->color);
	#ifdef DEBUG
		char str[100];
		snprintf(str, 100, "%i %i", this->x, this->y);
		DrawText(str, 40, 40, 20, BLACK);
	#endif
}

shared_ptr<Suika> SuikaFactory::create(Melon melon, int x, int y)
{
	shared_ptr<Suika> a = shared_ptr<Suika>(new Suika(x,y,10));
	switch (melon)
	{
	case Small:
		a->radius = 10;
		a->color = PURPLE;
		break;
	case Middle:
		a->radius = 50;
		a->color = YELLOW;
		break;
	case Large:
		a->radius = 150;
		a->color = GREEN;
		break;
	default:
		a->radius = 10;
		a->color = PURPLE;
		break;
	}
	return a;
}
