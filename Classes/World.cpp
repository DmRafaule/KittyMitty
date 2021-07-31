#include "GameLayer.h"
#include "engEnums.hpp"
#include "engMacros.hpp"

cocos2d::Size               WorldProperties::screenSize = cocos2d::Size();
cocos2d::Size               WorldProperties::mapSize = cocos2d::Size();
cocos2d::Vec2               WorldProperties::playerSpawnPoint = cocos2d::Vec2();
std::vector<cocos2d::Vec2>  WorldProperties::enemySpawnPoint(0);
std::vector<cocos2d::Rect>  WorldProperties::levelEnd(0);
std::vector<cocos2d::Rect>  WorldProperties::levelDeathZone(0);
std::vector<std::pair<std::string,cocos2d::Rect>>  WorldProperties::levelItems(0);

World::World(std::string world_file_path,cocos2d::Node* currentLayer){
   scaleOffset = 2;
   level = cocos2d::TMXTiledMap::create(world_file_path);
   level->setScale(scaleOffset);
   level_layer_midleground = level->getLayer("midleground");
   this->currentLayer = currentLayer;
   this->currentLayer->addChild(level,1);
   /*Define level size*/
   WorldProperties::mapSize.setSize(level->getMapSize().width  * level->getTileSize().width  * scaleOffset,
                                    level->getMapSize().height * level->getTileSize().height * scaleOffset);
   

   auto group = level->getObjectGroup("objectsLayer");
   auto& objects = group->getObjects();
   for (auto& obj : objects){
      cocos2d::ValueMap& dict = obj.asValueMap();
      float x = dict["x"].asFloat()           * scaleOffset;
      float y = dict["y"].asFloat()           * scaleOffset;
      float width = dict["width"].asFloat()   * scaleOffset;
      float height = dict["height"].asFloat() * scaleOffset;
      cocos2d::Node* ground = cocos2d::Node::create();
      auto ground_body = cocos2d::PhysicsBody::createBox(cocos2d::Size(width,height));
      ground_body->setDynamic(false);
      ground_body->setGravityEnable(false);
      ground_body->setPositionOffset(cocos2d::Vec2(width/2,height/2));
      
      ground_body->setContactTestBitmask(0xFF);
      if (dict["name"].asString() == "wall")
         ground_body->setCollisionBitmask(0x03);
      else if (dict["name"].asString() == "floor")
         ground_body->setCollisionBitmask(0x01);
      else if (dict["name"].asString() == "door"){
         WorldProperties::levelItems.push_back(std::pair<std::string,cocos2d::Rect>("door",cocos2d::Rect(dict["x"].asFloat()      * scaleOffset,
                                                                                                  dict["y"].asFloat()      * scaleOffset,
                                                                                                  dict["width"].asFloat()  * scaleOffset,
                                                                                                  dict["height"].asFloat() * scaleOffset)));
      }
      else if (dict["name"].asString() == "stair"){
         WorldProperties::levelItems.push_back(std::pair<std::string,cocos2d::Rect>("stair",cocos2d::Rect(dict["x"].asFloat()      * scaleOffset,
                                                                                                  dict["y"].asFloat()      * scaleOffset,
                                                                                                  dict["width"].asFloat()  * scaleOffset,
                                                                                                  dict["height"].asFloat() * scaleOffset)));
      }

      ground->setPosition(x,y);
      ground->setPhysicsBody(ground_body);
      currentLayer->addChild(ground);
      /*Define where will be appear player*/
      if (dict["name"].asString() == "PlayerSpawnPoint")
         WorldProperties::playerSpawnPoint.setPoint(dict["x"].asFloat() * scaleOffset, dict["y"].asFloat() * scaleOffset);
      /*Define where is new locations*/
      if (dict["name"].asString() == "NewLocation")//HERE fix collision detection for this rect
         WorldProperties::levelEnd.push_back(cocos2d::Rect(dict["x"].asFloat()      * scaleOffset,
                                                           dict["y"].asFloat()      * scaleOffset,
                                                           dict["width"].asFloat()  * scaleOffset,
                                                           dict["height"].asFloat() * scaleOffset));
      /*Define where will appears enemies*/
      if (dict["name"].asString() == "EnemySpawnPoint")
         WorldProperties::enemySpawnPoint.push_back(cocos2d::Vec2(dict["x"].asFloat() * scaleOffset, dict["y"].asFloat() * scaleOffset));
      /*Define where death will be emidiatly*/
      if (dict["name"].asString() == "DeathZone")
         WorldProperties::levelDeathZone.push_back(cocos2d::Rect(dict["x"].asFloat()      * scaleOffset,
                                                                 dict["y"].asFloat()      * scaleOffset,
                                                                 dict["width"].asFloat()  * scaleOffset,
                                                                 dict["height"].asFloat() * scaleOffset));
   }
   
}
World::~World(){}
void World::update(float dt){
   
}