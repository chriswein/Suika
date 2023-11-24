#include "suika.hh"
#undef ORANGE // Defined in raylib
typedef struct parameters
{
    int radius;
    Color color;
    float weight;
} parameters;

constexpr float baseweight = 200.0;
constexpr int baseradius = 10;



unordered_map<Fruits::GE_Type, parameters> values =
    {
        {Fruits::GRAPE, {1 * baseradius, PINK, 1.0f * baseweight}},
        {Fruits::CHERRY, {2 * baseradius, GRAY, 1.1f * baseweight}},
        {Fruits::STRAWBERRY, {3 * baseradius, LIGHTGRAY, 1.2f * baseweight}},
        {Fruits::TANGERINE, {4 * baseradius, PURPLE, 2.3f * baseweight}},
        {Fruits::ORANGE, {6 * baseradius, YELLOW, 2.5f * baseweight}},
        {Fruits::GRAPEFRUIT, {8 * baseradius, BLUE, 3.0f * baseweight}},
        {Fruits::MELON, {12 * baseradius, BLACK, 4.5f * baseweight}},
        {Fruits::SUIKA, {16 * baseradius, GREEN, 5.0f * baseweight}}};

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
	DrawText(str, 50, 40, 20, BLACK);
#endif
}


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
			values[type].weight, this->bodyref->GetLocalCenter(), this->bodyref->GetInertia()};
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
	int x_ = x<0?values[type].radius:x;
	shared_ptr<Suika> a = shared_ptr<Suika>(new Suika(x_, y, values[type].radius, world));
	a->changeType(type);
	return a;
}


Fruits::GE_Type getNextMelon()
{	
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distr(1, 1 + (GE_Type_Max / 2));

	if (next_melons.empty())
	{
		for (size_t i = 0; i < 5; i++)
		{
			next_melons.push((Fruits::GE_Type)distr(rd));
		}
	}
	else
	{
		next_melons.push((Fruits::GE_Type)distr(rd));
	}
	auto res = next_melons.front();
	next_melons.pop();
	return res;
}

Fruits::GE_Type previewNext(){
	if (next_melons.empty()){
		getNextMelon();
	}
	return next_melons.front();
}