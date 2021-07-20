#include "GameLayer.h"

World::World(std::string world_file_path,void* currentLayer){
   level = cocos2d::TMXTiledMap::create(world_file_path);
   level->setScale(2);
   level_layer_midleground = level->getLayer("midleground");
   this->currentLayer = currentLayer;
   static_cast<GameLayer*>(currentLayer)->getChildByName("gamesession")->addChild(level,1);
}
World::~World(){}
void World::update(float dt){
   auto group = level->getObjectGroup("obj1");
   auto& objects = group->getObjects();
   for (auto& obj : objects){
      cocos2d::ValueMap& dict = obj.asValueMap();
      float x = dict["x"].asFloat() *2;
      float y = dict["y"].asFloat()* 2;
      float width = dict["width"].asFloat()*2;
      float height = dict["height"].asFloat()*2;
      if (static_cast<GameLayer*>(currentLayer)->getPlayer()->getCreatureSprite()->getBoundingBox().intersectsRect(cocos2d::Rect(x,y,width,height)))
         static_cast<GameLayer*>(currentLayer)->getPlayer()->getCreatureSprite()->runAction(cocos2d::MoveBy::create(0,cocos2d::Vec2(0.1,8)));
   }
}