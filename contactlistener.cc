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

    auto checkConsumed = [this](int id){
        bool res = false;
        if(this->mergables.find(id) != this->mergables.end()) res = true;
        if(this->deletables.find(id) != this->deletables.end()) res = true;
        return res;
    };

    if (one == nullptr || two == nullptr) 
        return;

    if (one->type == two->type && one->type != Fruits::BOX)
    {
        if (one->type == GE_Type_Max)
            return; // If maximum size do nothing

        if (checkConsumed(one->id)) return; // One of the two has already be marked. Abort.
        if (checkConsumed(two->id)) return; // One of the two has already be marked. Abort.

        this->deletables[one->id] = 0;
        this->deletables[two->id] = 0;

        auto addToMergable = [this, &one, &two](GE_Id *ele, GE_Id *ele2) // TODO this Lambda has become obsolete
        {
            auto oneiterator = this->gm->geMap.find(ele->id);
            auto oneobj = dpc<Suika>((*oneiterator).second);
            auto twoiterator = this->gm->geMap.find(ele2->id);
            auto twoobj = dpc<Suika>((*twoiterator).second);
            int id_to_store = one->id;
            if ((this->mergables.find(one->id) != this->mergables.end()))
            {
                id_to_store = one->id;
            }
            else if (this->mergables.find(two->id) != this->mergables.end())
            {
                id_to_store = two->id;
            }

            this->mergables[id_to_store] = // TODO change to unique_map to avoid doubles
                {{.x = oneobj->x,
                  .y = oneobj->y,
                  .radius = oneobj->radius,
                  .type = ele->type},

                 {.x = twoobj->x,
                  .y = twoobj->y,
                  .radius = twoobj->radius,
                  .type = ele2->type}};
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
    vector<FruitsPairToMerge> res;
    for(auto pair : this->mergables)
        res.push_back(pair.second);
    this->mergables.clear();
    return res;
}