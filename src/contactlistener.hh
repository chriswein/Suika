#pragma once
#include "box2d.h"
#include "helpers.hh"
#include "raylib.h"
#include "suika.hh"
#include <map>
#include <unordered_map>
#include <vector>
using namespace std;

typedef struct FruitsPairChild
{
    int x;
    int y;
    float radius;
    Fruits::GE_Type type;
} FruitsPairChild;

typedef struct FruitsPairToMerge
{
    FruitsPairChild A{};
    FruitsPairChild B{};
} FruitsPairToMerge;

class contactlistener : public b2ContactListener
{
private:
    /* data */
    shared_ptr<GEManager> gm;
    unordered_map<int, int> deletables;
    unordered_map<int, FruitsPairToMerge>mergables;
    
public:
    contactlistener(shared_ptr<GEManager> gm);
    ~contactlistener();
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;
    vector<int> GetDeletables();
    vector<FruitsPairToMerge> GetMergables();
};
