#include "game.hh"
#define dpc dynamic_pointer_cast

void Game::update(shared_ptr<contactlistener> contacts, shared_ptr<GEManager> gm, shared_ptr<b2World> world)
{
    vector<FruitsPairToMerge> mergables = contacts->GetMergables();
    auto calcMiddlePosition = [](struct FruitsPairChild a, struct FruitsPairChild b)
    {
        auto left = min(a.x, b.x);
        auto top = min(a.y, b.y);
        auto diffw = abs(a.x - b.x);
        auto diffh = abs(a.y - b.y);
        printf("%i_L %i_T %i_DW %i_DH \n", left, top, diffw, diffh);
        struct pos
        {
            int x;
            int y;
        } p;
        p.x = -offset_x + left + diffw / 2;
        p.y = -offset_y + top + diffh / 2;
        return p;
    };
    auto calcNewType = [](Fruits::GE_Type type)
    {
        int i = type;
        if (++i > GE_Type_Max)
            i = GE_Type_Max;
        return (Fruits::GE_Type)i;
    };

    for (auto key : contacts->GetDeletables()) // Remove all elements that where selected
    {
        gm->deleteGE(key);
    }

    for (auto ele : mergables) // Add a new element for the pair that was deleted
    {
        auto new_position = calcMiddlePosition(ele.A, ele.B);
        auto new_type = calcNewType(ele.A.type);
        auto l = (new_position.x); 
        // TODO this does not fix the problem
        auto t = (new_position.y);
        if (l<0){ // TODO only for debugging
            ;
        }
        auto s =SuikaFactory::create(
                    new_type, l, t, world);
        s->update();
        gm->insertGE(
            dpc<GE>(s
                ));
        printf("Inserted new Suika %i at %ix %iy \n", new_type, l, t);
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
