#include "contactlistener.hh"
#define dpc dynamic_pointer_cast

contactlistener::contactlistener(shared_ptr<GEManager> gm)
{
    this->gm = gm;
}

contactlistener::~contactlistener()
{
}

void contactlistener::BeginContact(b2Contact *contact)
{
    GE_Id *one = (GE_Id *)(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    GE_Id *two = (GE_Id *)(contact->GetFixtureB()->GetBody()->GetUserData().pointer);
    if (one == nullptr || two == nullptr)
        return;

    if (one->type == two->type && one->type != Fruits::BOX)
    {
        this->deletables[one->id] = 0;
        this->deletables[two->id] = 0;
        auto addToMergable = [this](GE_Id *ele, GE_Id *ele2) // TODO this Lambda has become obsolete
        {
            auto oneobji = this->gm->geMap.find(ele->id);
            auto oneobj = dpc<Suika>((*oneobji).second);
            auto twoobji = this->gm->geMap.find(ele2->id);
            auto twoobj = dpc<Suika>((*oneobji).second);

            this->mergables.push_back(
                {{.x = oneobj->x,
                  .y = oneobj->y,
                  .radius = oneobj->radius,
                  .type = ele->type},

                 {.x = twoobj->x,
                  .y = twoobj->y,
                  .radius = twoobj->radius,
                  .type = ele2->type}});
        };
        addToMergable(one, two);
    }
}

void contactlistener::EndContact(b2Contact *contact)
{
}

vector<int> contactlistener::GetDeletables()
{
    vector<int> keys;
    for (auto pair : this->deletables)
        keys.push_back(pair.first);
    this->deletables.clear();
    return keys;
}

vector<FruitsPairToMerge> contactlistener::GetMergables()
{
    vector<FruitsPairToMerge> copy(this->mergables);
    this->mergables.clear();
    return copy;
}