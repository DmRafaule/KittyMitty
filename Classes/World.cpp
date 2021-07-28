#include "GameLayer.h"
#include "engEnums.hpp"
#include "engMacros.hpp"

std::vector<std::string> World::level_obj_groupes({"floors","walls","roofs"});

World::World(std::string world_file_path,cocos2d::Node* currentLayer){
   level = cocos2d::TMXTiledMap::create(world_file_path);
   level->setScale(2);
   level_layer_midleground = level->getLayer("midleground");
   this->currentLayer = currentLayer;
   currentLayer->addChild(level,1);
   /*Define level size*/
   this->mapSize.setSize(level->getMapSize().width  * level->getTileSize().width  * 2,
                         level->getMapSize().height * level->getTileSize().height * 2);
   

   auto group = level->getObjectGroup("objectsLayer");
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
      
      ground_body->setContactTestBitmask(0xFF);
      if (dict["name"].asString() == "wall")
         ground_body->setCollisionBitmask(0x03);
      else
         ground_body->setCollisionBitmask(0x01);

      ground->setPosition(x,y);
      ground->setPhysicsBody(ground_body);
      currentLayer->addChild(ground);
      /*Define where is new locations*/
      if (dict["name"].asString() == "NewLocation")
         this->levelEnd.setRect(dict["x"].asFloat()*2,dict["y"].asFloat()*2, dict["width"].asFloat()*2,dict["height"].asFloat()*2);
      /*Define where will be appear player*/
      if (dict["name"].asString() == "PlayerSpawnPoint"){
         this->playerSpawnPoint.setPoint(dict["x"].asFloat()*2,dict["y"].asFloat()*2);
      }
      /*Define where will appears enemies*/
      if (dict["name"].asString() == "EnemySpawnPoint")
         this->enemySpawnPoint.push_back(cocos2d::Vec2(dict["x"].asFloat()*2,dict["y"].asFloat()*2));
   }
   
}
World::~World(){}
void World::update(float dt){
   
}