#include "contactlistener.hh"

contactlistener::contactlistener(shared_ptr<GEManager> gm)
{
    this->gm = gm;
}

contactlistener::~contactlistener()
{
}

void contactlistener::BeginContact(b2Contact* contact){
    try
    {
        /* code */
        auto pr = [](GE_Id* one){
            if (one == nullptr) return;
            printf("%i ", one->id);
            char tmp[100];
            snprintf(tmp, 100, "%i", one->id);
            DrawText(tmp,100,100,30,BLACK); 
        };

        GE_Id* one = (GE_Id*)(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
        GE_Id* two = (GE_Id*)(contact->GetFixtureB()->GetBody()->GetUserData().pointer);
        if(one == nullptr || two == nullptr) return;

        if(one->type == two->type && one->type == BOX){
            printf("Explosion \n");
             this->gm->deleteGE(one->id);
            
        }
        
    }
    catch(const std::exception& e)
    {
       
    }
    
    
}

void contactlistener::EndContact(b2Contact* contact){

}