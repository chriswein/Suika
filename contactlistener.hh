#include "box2d.h"
#include "helpers.hh"
#include "raylib.h"
#include <map>
#include <vector>
using namespace std;

class contactlistener : public b2ContactListener
{
private:
    /* data */
    shared_ptr<GEManager> gm;
    unordered_map<int,int> deletables;
public:
    contactlistener(shared_ptr<GEManager> gm);
    ~contactlistener();
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;
    vector<int> GetDeletables();
};

