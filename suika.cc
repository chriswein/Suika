#include "suika.hh"
#undef ORANGE // Defined in raylib

Suika::Suika(int x, int y, int radius, shared_ptr<b2World> world) : Box(x, y, radius / 2, radius / 2)
{
	this->x = x;
	this->y = y;
	this->radius = radius;
	this->init(world);
}

Suika::~Suika() {}

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

typedef struct parameters
{
	int radius;
	Color color;
	float weight;
} parameters;

const float baseweight = 200.0;
const int baseradius = 10;
unordered_map<Fruits::GE_Type, parameters> values =
	{{Fruits::TANGERINE, {2*baseradius, PURPLE, 1.0f*baseweight}},
	 {Fruits::ORANGE, {4*baseradius, YELLOW, 1.5f*baseweight}},
	 {Fruits::GRAPEFRUIT, {8*baseradius, BLUE, 2.0f*baseweight}},
	 {Fruits::MELON, {12*baseradius, BLACK, 2.5f*baseweight}},
	 {Fruits::SUIKA, {16*baseradius, GREEN, 3.0f*baseweight}}};

void Suika::changeType(Fruits::GE_Type type)
{
	using namespace Fruits;

	auto apply = [](parameters p) {};

	this->gid->type = type;
	if (values.find(type) == values.end())
		return;
	this->radius = values[type].radius;
	this->color = values[type].color;
	this->massdata = 
	{
		values[type].weight, this->bodyref->GetLocalCenter(), this->bodyref->GetInertia()
	};
	const b2MassData *p = &massdata;
	this->bodyref->SetMassData(p);
}

shared_ptr<Suika> SuikaFactory::create(Melon melon, int x, int y, shared_ptr<b2World> world)
{
	using namespace Fruits;
	Color color = PURPLE;
	GE_Type type = TANGERINE;

	switch (melon)
	{
	case Small:
		type = TANGERINE;
		break;
	case Middle:
		type = ORANGE;
		break;
	case Large:
		type = GRAPEFRUIT;
		break;
	case Giant:
		type = SUIKA;
		break;
	default:
		type = TANGERINE;
		break;
	}
	shared_ptr<Suika> a = shared_ptr<Suika>(new Suika(x, y, values[type].radius, world));
	a->color = color;
	a->changeType(type);
	return a;
}

shared_ptr<Suika> SuikaFactory::create(Fruits::GE_Type type, int x, int y, shared_ptr<b2World> world)
{
	using namespace Fruits;
	shared_ptr<Suika> a = shared_ptr<Suika>(new Suika(x, y, values[type].radius, world));
	a->changeType(type);
	return a;
}

Melon getNextMelon()
{
	using namespace std;
	random_device rd;								  // obtain a random number from hardware
	mt19937 gen(rd());								  // seed the generator
	uniform_int_distribution<> distr(0, MELONLENGTH); // define the range
	return (Melon)distr(gen);
}
