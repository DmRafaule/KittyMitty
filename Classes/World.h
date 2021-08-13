#pragma once 

#include <cocos2d.h>

using namespace cocos2d;

class World {
public:
   World(std::string world_file_path,cocos2d::Vec2 world_offset,cocos2d::Node* currentLayer);
   ~World();
   void update(float dt);
private:
   void initLevelObjects();
   void initBackground();
private:
   cocos2d::TMXTiledMap* level;
   cocos2d::TMXLayer* level_layer_midleground;
   cocos2d::Node* currentLayer;
   std::vector<cocos2d::Node*> ground;
   cocos2d::Sprite* backgroundSprite;
   float scaleOffset;
};