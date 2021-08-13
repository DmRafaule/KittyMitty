#include "GameLayer.h"
#include "engEnums.hpp"
#include "engMacros.hpp"

cocos2d::Size               WorldProperties::screenSize = cocos2d::Size();
cocos2d::Size               WorldProperties::mapSize = cocos2d::Size();
cocos2d::Vec2               WorldProperties::playerSpawnPoint = cocos2d::Vec2();
std::vector<cocos2d::Vec2>  WorldProperties::enemySpawnPoint(0);
std::vector<std::pair<cocos2d::Rect,std::pair<std::string,cocos2d::Vec2>>>  WorldProperties::chunks_transitions(0);
std::vector<cocos2d::Rect>  WorldProperties::levelDeathZone(0);
std::vector<std::pair<std::string,cocos2d::Rect>>  WorldProperties::levelItems(0);

World::World(){};
World::World(uint levelNum,cocos2d::Node* currentLayer){
   level = new Level(levelNum,currentLayer);
}
World::~World(){
   delete level;
}
void World::update(float dt){
   level->update(dt);
}

Level::Level(){};
Level::Level(uint level,cocos2d::Node* currentLayer){
   this->scaleOffset = 2;
   this->level_offset = cocos2d::Vec2(0,0);
   this->currentLayer = currentLayer;
   switch (level){
   case 0:{
      loadChunk("world/area0/level0.tmx",level_offset);
      break;
   }
   case 1:{
      break;
   
   }
   }
}
Level::~Level(){
   
}
void Level::update(float dt){
   if (!isNewChunk)
      for (const auto& chunk : WorldProperties::chunks_transitions){
         /*intersection player body with chunks_transitions*/
         if (currentLayer->getChildByName(SceneEntities::gamesession)->getChildByTag(2)->getBoundingBox().intersectsRect(chunk.first)){
            unloadChunk();
            loadChunk(chunk.second.first, chunk.second.second);
            this->currentLayer->getChildByName(SceneEntities::gamesession)->getChildByTag(2)->setPosition(3200,
                                                                                          this->currentLayer->getChildByName(SceneEntities::gamesession)->getChildByTag(2)->getPosition().y);
            this->currentLayer->getChildByName(SceneEntities::gamesession)->stopAllActions();
            this->currentLayer->getChildByName(SceneEntities::gamesession)->runAction(
                 cocos2d::Follow::createWithOffset(
                     this->currentLayer->getChildByName(SceneEntities::gamesession)->getChildByTag(2),
                     -100,-100,
                     cocos2d::Rect(
                        ,
                        ,
                        WorldProperties::mapSize.width - WorldProperties::screenSize.width,
                        WorldProperties::mapSize.height - WorldProperties::screenSize.height 
                     )
                 )
             );
            isNewChunk = true;
         }
      }
}
void Level::loadChunk(std::string chunkPath, cocos2d::Vec2 offset){
   this->level_offset = offset;
   this->current_level_offset = offset;
   this->level = cocos2d::TMXTiledMap::create(chunkPath);
   this->level->setScale(scaleOffset);
   this->level->setPosition(level_offset);
   //this->level->setPosition(level_offset);
   this->level_layer_midleground = this->level->getLayer("midleground");
   this->currentLayer->getChildByName(SceneEntities::gamesession)->addChild(this->level,1);
   /*Define level size*/
   WorldProperties::mapSize.setSize(this->level->getMapSize().width  * this->level->getTileSize().width  * scaleOffset,
                                    this->level->getMapSize().height * this->level->getTileSize().height * scaleOffset);
   initLevelObjects();
   initBackground();
}
void Level::unloadChunk(){
   currentLayer->getChildByName(SceneEntities::gamesession)->removeChild(level);
   for (auto& i : level_bodies){
      currentLayer->getChildByName(SceneEntities::gamesession)->removeChild(i);
   }
   WorldProperties::enemySpawnPoint.clear();
   WorldProperties::levelDeathZone.clear();
   WorldProperties::levelItems.clear();
}

void Level::initLevelObjects(){
   //Get object layer
   auto group = level->getObjectGroup("objectsLayer");
   //Get all objects(points,poligons,reqtangles etc)
   auto& objects = group->getObjects();
   //Save their properties to memory
   for (auto& obj : objects){
      //Get data from property of tiled map
      cocos2d::ValueMap& dict = obj.asValueMap();
      float x = dict["x"].asFloat()           * scaleOffset + current_level_offset.x;//Here somo poligons just wont be in right place FIX IT!!!
      float y = dict["y"].asFloat()           * scaleOffset + current_level_offset.y;
      float width = dict["width"].asFloat()   * scaleOffset;
      float height = dict["height"].asFloat() * scaleOffset;
      //Init node for add in physic scene physic bodies
      level_bodies.push_back(cocos2d::Node::create());
      auto ground_body = cocos2d::PhysicsBody::createBox(cocos2d::Size(width,height));
      ground_body->setDynamic(false);
      ground_body->setGravityEnable(false);
      ground_body->setPositionOffset(cocos2d::Vec2(width/2,height/2));
      ground_body->setContactTestBitmask(0xFF);
      //Define object wall
      if (dict["name"].asString() == "wall"){
         ground_body->setCollisionBitmask(0x03);
         level_bodies.back()->setPhysicsBody(ground_body);
      }
      //Define object floor
      else if (dict["name"].asString() == "floor"){
         ground_body->setCollisionBitmask(0x01);
         level_bodies.back()->setPhysicsBody(ground_body);
      }
      else if (dict["name"].asString() == "roof"){
         ground_body->setCollisionBitmask(0x05);
         level_bodies.back()->setPhysicsBody(ground_body);
      }
      //Define object door
      else if (dict["name"].asString() == "door"){
         WorldProperties::levelItems.push_back(std::pair<std::string,cocos2d::Rect>("door",cocos2d::Rect(x,y,width,height)));
      }
      //Define object stair
      else if (dict["name"].asString() == "stair"){
         WorldProperties::levelItems.push_back(std::pair<std::string,cocos2d::Rect>("stair",cocos2d::Rect(x,y,width,height)));
      }
      //Define object steps
      else if (dict["name"].asString() == "steps"){
         std::vector<cocos2d::Vec2> points;
         points.push_back(cocos2d::Vec2(0,0));
         //  size of tiles(16 as default) *
         //  how many tiles in width(height) * 
         //  scale factor(2) -
         //  factor of cocos resizing<1> *
         //  where this poligon look at (to right 1 or to left -1)                                                   
         points.push_back(cocos2d::Vec2((level_layer_midleground->getMapTileSize().width * dict["widthInTiles"].asInt() * scaleOffset - dict["widthInTiles"].asInt()*8)*dict["toRightMod"].asInt(),
                                        level_layer_midleground->getMapTileSize().height* dict["heightInTiles"].asInt()* scaleOffset - dict["widthInTiles"].asInt()*8));
         points.push_back(cocos2d::Vec2((level_layer_midleground->getMapTileSize().width * dict["widthInTiles"].asInt() * scaleOffset - dict["widthInTiles"].asInt()*8)*dict["toRightMod"].asInt(),
                                        0));
         cocos2d::PhysicsBody* poligon = cocos2d::PhysicsBody::createPolygon(points.data(),points.size());
         poligon->setDynamic(false);
         poligon->setGravityEnable(false);
         poligon->setContactTestBitmask(0xFF);
         poligon->setCollisionBitmask(0x04);
         level_bodies.back()->setPhysicsBody(poligon);
      }

      level_bodies.back()->setPosition(x, y );
      currentLayer->getChildByName(SceneEntities::gamesession)->addChild(level_bodies.back());
      /*Define where will be appear player*/
      if (dict["name"].asString() == "PlayerSpawnPoint")
         WorldProperties::playerSpawnPoint.setPoint(x,y);
      /*Define where is new locations*/
      if (dict["name"].asString() == "NewLocation"){
         level_offset.x = dict["nextLevelOffsetX"].asFloat();
         level_offset.y = dict["nextLevelOffsetY"].asFloat();
         WorldProperties::chunks_transitions.push_back(std::pair<cocos2d::Rect,std::pair<std::string,cocos2d::Vec2>>
                                                         (cocos2d::Rect(x,y,width,height),std::pair<std::string,cocos2d::Vec2>(dict["nextChunk"].asString(),level_offset)));
      }
      /*Define where will appears enemies*/
      if (dict["name"].asString() == "EnemySpawnPoint")
         WorldProperties::enemySpawnPoint.push_back(cocos2d::Vec2(x,y));
      /*Define where death will be emidiatly*/
      if (dict["name"].asString() == "DeathZone")
         WorldProperties::levelDeathZone.push_back(cocos2d::Rect(x,y,width,height));
   }
}
void Level::initBackground(){
   this->backgroundSprite = cocos2d::Sprite::create("world/area0/backgroundImage.png");
   cocos2d::Texture2D::TexParams tpar = {
      cocos2d::backend::SamplerFilter::NEAREST,
      cocos2d::backend::SamplerFilter::NEAREST,
      cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE,
      cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE
   };
   backgroundSprite->getTexture()->setTexParameters(tpar);
   backgroundSprite->setPosition(WorldProperties::screenSize.width/2,WorldProperties::screenSize.height/2);
   backgroundSprite->setScale(MAX(WorldProperties::screenSize.width/backgroundSprite->getBoundingBox().size.width,
                                  WorldProperties::screenSize.height/backgroundSprite->getBoundingBox().size.height));
   currentLayer->getChildByName(SceneEntities::bg)->addChild(backgroundSprite);
}
