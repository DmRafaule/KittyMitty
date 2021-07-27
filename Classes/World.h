#pragma once 

#include <cocos2d.h>

class World {
public:
   World(std::string world_file_path,cocos2d::Node* currentLayer);
   ~World();
   void update(float dt);
private:
private:
   cocos2d::TMXTiledMap* level;
   cocos2d::TMXLayer* level_layer_midleground;
   static std::vector<std::string> level_obj_groupes;
   cocos2d::Node* currentLayer;
};