#pragma once
#include <raylib.h>
#include <memory>
#include <unordered_map>
#include <string>
using namespace std;



class Textures
{

public:
    static unordered_map<int, shared_ptr<Texture2D>> texturemap;
    Textures(/* args */);
    ~Textures();
    void load(string name, int id);
    shared_ptr<Texture2D> get(int id);
};

