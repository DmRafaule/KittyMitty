#include "GameLayer.h"

World::World(std::string world_file_path,void* currentLayer){
   level = cocos2d::TMXTiledMap::create(world_file_path);
   level->setScale(2);
   level_layer_midleground = level->getLayer("midleground");
   this->currentLayer = currentLayer;
   static_cast<GameLayer*>(currentLayer)->getChildByName("gamesession")->addChild(level,1);

   auto group = level->getObjectGroup("obj1");
   auto& objects = group->getObjects();
   for (auto& obj : objects){
      cocos2d::ValueMap& dict = obj.asValueMap();
      float x = dict["x"].asFloat() *2;
      float y = dict["y"].asFloat()* 2;
      float width = dict["width"].asFloat()*2;
      float height = dict["height"].asFloat()*2;
      
      cocos2d::Node* ground = cocos2d::Node::create();
      auto ground_body = cocos2d::PhysicsBody::createBox(cocos2d::Size(width,height));
      ground_body->setDynamic(false);
      ground_body->setGravityEnable(false);
      ground_body->setPositionOffset(cocos2d::Vec2(width/2,height/2));
      ground_body->setCategoryBitmask(0x01);
      ground_body->setCollisionBitmask(0x02);
      ground->setPosition(x,y);
      ground->setPhysicsBody(ground_body);
      static_cast<GameLayer*>(currentLayer)->getChildByName("gamesession")->addChild(ground);
   }
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
      if (static_cast<GameLayer*>(currentLayer)->getPlayer()->getCreatureSprite()->getBoundingBox().intersectsRect(cocos2d::Rect(x,y,width,height))){
         //static_cast<GameLayer*>(currentLayer)->getPlayer()->getCreatureSprite()->getPhysicsBody()->setDynamic(false);
         //static_cast<GameLayer*>(currentLayer)->getPlayer()->getCreatureSprite()->runAction(cocos2d::MoveBy::create(0.1,cocos2d::Vec2(0,1)));
      }
   }
}