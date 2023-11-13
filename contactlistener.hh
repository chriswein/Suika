#include "box2d.h"
#include "helpers.hh"
#include "raylib.h"

class contactlistener : public b2ContactListener
{
private:
    /* data */
    shared_ptr<GEManager> gm;
public:
    contactlistener(shared_ptr<GEManager> gm);
    ~contactlistener();
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;
};

