#pragma once 

#include <cocos2d.h>

class World {
public:
   World(std::string world_file_path,void* currentLayer);
   ~World();
   void update(float dt);
private:
private:
   cocos2d::TMXTiledMap* level;
   cocos2d::TMXLayer* level_layer_midleground;
   void* currentLayer;
};