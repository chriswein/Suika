#include "suika.hh"

Suika::Suika(int x, int y, int radius, shared_ptr<b2World> world) : Box(x, y, radius / 2, radius / 2)
{
	this->x = x;
	this->y = y;
	this->radius = radius;
	this->init(world);
}

Suika::~Suika()
{
}

void Suika::init(shared_ptr<b2World> world)
{
	this->gid->type = Fruits::SUIKA;
	float x = coordinateXInBox2d(this->x); // + this->w / 2);
	float y = coordinateYInBox2d(this->y); // + this->h / 2);

	this->world = world;
	b2BodyDef bodyDef;
	if (!this->stat)
		bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	b2BodyUserData data;
	data.pointer = (uintptr_t)(this->gid.get());
	bodyDef.userData = data;
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
	snprintf(str, 100, "%i %i \n id: %i type: %i", this->x, this->y, this->gid->id, this->gid->type);
	DrawText(str, 50, 40, 20, BLACK);
#endif
}

void Suika::changeType(Fruits::GE_Type type){
	this->gid->type = type;
}

shared_ptr<Suika> SuikaFactory::create(Melon melon, int x, int y, shared_ptr<b2World> world)
{
	#undef ORANGE // Defined in raylib
	using namespace Fruits;
	int radius = 10;
	Color color = PURPLE;
	GE_Type type = TANGERINE;

	switch (melon)
	{
	case Small:
		radius = 10;
		color = PURPLE;
		type = TANGERINE;
		break;
	case Middle:
		radius = 50;
		color = YELLOW;
		type = ORANGE;
		break;
	case Large:
		radius = 150;
		color = GREEN;
		type = GRAPEFRUIT;
		break;
	case Giant:
		radius = 225;
		color = GREEN;
		type = SUIKA;
		break;
	default:
		radius = 10;
		color = PURPLE;
		type = TANGERINE;
		break;
	}
	shared_ptr<Suika> a = shared_ptr<Suika>(new Suika(x, y, radius, world));
	a->color = color;
	a->changeType(type);
	return a;
}
