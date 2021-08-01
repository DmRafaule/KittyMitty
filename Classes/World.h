#pragma once 

#include <cocos2d.h>

class World {
public:
   World(std::string world_file_path,cocos2d::Node* currentLayer);
   ~World();
   void update(float dt);

private:
   cocos2d::TMXTiledMap* level;
   cocos2d::TMXLayer* level_layer_midleground;
   cocos2d::Node* currentLayer;
   cocos2d::Sprite* backgroundSprite;
   float scaleOffset;
};