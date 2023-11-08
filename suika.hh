#include "box.hh"
#include "box2d.h"
#include "raylib.h"

class Suika : public Box
{
private:
    /* data */
    float radius;
public:
    Suika(int x, int y, int radius);
    ~Suika();
    void init(shared_ptr<b2World> world) override;
    void draw() override;
};
