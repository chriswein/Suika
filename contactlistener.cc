#include "contactlistener.hh"

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

    if (one->type == two->type && one->type == SUIKA)
    {
        this->deletables[one->id] = 0;
        this->deletables[two->id] = 0;
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