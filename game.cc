#include "game.hh"
#define dpc dynamic_pointer_cast

void Game::update(shared_ptr<contactlistener> contacts, shared_ptr<GEManager> gm,shared_ptr<b2World> world)
{
    vector<FruitsPairToMerge> mergables = contacts->GetMergables();
    auto compareFruitsByHeightAndReturnLower = [](struct FruitsPairChild a, struct FruitsPairChild b)
    {
        if (a.y < b.y)
            return a;
        return b;
    };
    auto calcMiddlePosition = [](struct FruitsPairChild a, struct FruitsPairChild b)
    {
        auto left = min(a.x, b.x);
        auto top = min(a.y, b.y);
        auto diffw = abs(a.x - b.x);
        auto diffh = abs(a.y - b.y);
        struct pos
        {
            int x;
            int y;
        } p;
        p.x = left + diffw / 2;
        p.y = top + diffh / 2;
        return p;
    };
    auto calcNewType = [](Fruits::GE_Type type){
        int i = type;
        if(++i > GE_Type_Max) i = GE_Type_Max;
        return (Fruits::GE_Type)i;
    };
    
    for (auto key : contacts->GetDeletables()) // Remove all elements that where selected
	{
			gm->deleteGE(key);
	}

    for (auto ele : mergables) // Add a new element for the pair that was deleted
    {
        auto lower = compareFruitsByHeightAndReturnLower(ele.A, ele.B);
        auto newpos = calcMiddlePosition(ele.A, ele.B);
        auto new_type = calcNewType(ele.A.type);
        gm->insertGE(
            dpc<GE>(
                SuikaFactory::create(
                    new_type, newpos.x, newpos.y, world
                )
            )
        );
    }

   

}
void Game::init(shared_ptr<b2World> world, shared_ptr<GEManager> gm)
{
    // gm->insertGE(dpc<GE>(shared_ptr<Box>((new Box(300, 100, 20, 20, world)))));
    // gm->insertGE(dpc<GE>(shared_ptr<Box>((new Box(100, 100, 20, 20, world)))));
    // gm->insertGE(dpc<GE>(shared_ptr<Box>((new Box(300, 300, 20, 20, world)))));
    // gm->insertGE(dpc<GE>(shared_ptr<Box>((new Box(250, 100, 40, 40, world)))));
    gm->insertGE(dpc<GE>(
        SuikaFactory::create(Large, 200, 10, world)));
    gm->insertGE(dpc<GE>(
        SuikaFactory::create(Large, 600, 60, world)));
}

Game::Game(/* args */)
{
}

Game::~Game()
{
}
