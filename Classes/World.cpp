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

   for (int i = 0; i < level_obj_groupes.size(); ++i){
      auto group = level->getObjectGroup(level_obj_groupes.at(i));
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
         if (i == 0){
            ground_body->setContactTestBitmask(true);
            ground_body->setCollisionBitmask(ContactMasks::floorMask);
         }
         else if (i == 1){
            ground_body->setContactTestBitmask(true);
            ground_body->setCollisionBitmask(ContactMasks::wallMask);
         }
         else{
            ground_body->setContactTestBitmask(true);
            ground_body->setCollisionBitmask(ContactMasks::roofMask);
         }
         //ground_body->setCollisionBitmask(ContactMasks::creatureMask);
         ground->setPosition(x,y);
         ground->setPhysicsBody(ground_body);
         currentLayer->addChild(ground);
      }
   }
}
World::~World(){}
void World::update(float dt){
   
}