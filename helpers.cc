#include "helpers.hh"

float widthInBox2d(int w)
{
	return w * b2_w_conv;
}

float heightInBox2d(int h)
{
	return h * b2_h_conv;
}

float coordinateXInBox2d(int x)
{
	float middle = widthInBox2d(x);
	middle = middle - b2_world_width / 2;
	return middle;
}
float coordinateYInBox2d(int y)
{
	float middle = heightInBox2d(y);
	middle = (middle - b2_world_height / 2);
	return middle * -1;
}

int pixelXFromBox2d(float x)
{
	if (x < 0)
	{
		float q = (b2_world_width / 2) - abs(x);
		float a = (abs(q) / (b2_world_width / 2)) * (window_width / 2);
		return a;
	}
	else
	{
		float q = x;
		float a = (abs(q) / (b2_world_width / 2)) * (window_width / 2);
		a = (window_width / 2) + a;
		return a;
	}
}
int pixelYFromBox2d(float y)
{
	if (y > 0)
	{
		float q = (b2_world_height / 2) - abs(y);
		float a = (abs(q) / (b2_world_height / 2)) * (window_height / 2);
		return a;
	}
	else
	{
		float q = y;
		float a = (abs(q) / (b2_world_height / 2)) * (window_height / 2);
		a = (window_height / 2) + a;
		return a;
	}
}

GEManager::GEManager() {}

void GEManager::insertGE(shared_ptr<GE>  ge) {
    this->geMap[ge->id()->id] = ge;
}

void GEManager::deleteGE(int id) {
    int key = id;
    auto it = this->geMap.find(key);
    if (it != this->geMap.end()) {
        this->geMap.erase(it);
    } 
}

void GEManager::printAllGEs() const {
    for (const auto& pair : this->geMap) {
        int key = pair.first;
        shared_ptr<GE> ge = pair.second;
    }
}
