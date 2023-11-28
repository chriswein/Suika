#include "textures.hh"

// Shared map for all instances 
unordered_map<int, shared_ptr<Texture2D>> Textures::texturemap;


/*
    Loads texture into map if it does not already exist under this id.
    Possible double loading issue here. 
*/
void Textures::load(string name, int id)
{
    if (Textures::texturemap.find(id) != Textures::texturemap.end())
        return;
    Textures::texturemap[id] = make_shared<Texture2D>(LoadTexture(name.c_str()));
};


/*
    Returns texture pointer for id or null_ptr
*/
shared_ptr<Texture2D> Textures::get(int id)
{
    if (Textures::texturemap.find(id) == Textures::texturemap.end())
        return shared_ptr<Texture2D>(nullptr);
    else
        return Textures::texturemap.find(id)->second;
};


Textures::Textures(/* args */)
{
}

Textures::~Textures()
{
}