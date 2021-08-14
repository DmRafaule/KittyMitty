#include "GameLayer.h"
#include "engEnums.hpp"
#include "engMacros.hpp"

cocos2d::Size               WorldProperties::screenSize = cocos2d::Size();
cocos2d::Size               WorldProperties::mapSize = cocos2d::Size();
cocos2d::Vec2               WorldProperties::playerSpawnPoint = cocos2d::Vec2();
std::vector<cocos2d::Vec2>  WorldProperties::enemySpawnPoint(0);
std::vector<cocos2d::Rect>  WorldProperties::levelDeathZone(0);
std::vector<std::pair<std::string,cocos2d::Rect>>  WorldProperties::levelItems(0);
std::vector<LevelTransition> WorldProperties::levelTransitions(0);
LevelTransition::LevelTransition(){}
LevelTransition::LevelTransition(cocos2d::Rect reqt,std::string path,std::string backgroundPath,cocos2d::Vec2 offset){
   this->reqt = reqt;
   this->path = path;
   this->backgroundPath = backgroundPath;
   this->offset = offset;
}


World::World(){};
World::World(uint levelNum,GameLayer* currentLayer){
   level = new Level(levelNum,currentLayer);
}
World::~World(){
   delete level;
}
void World::update(float dt){
   level->update(dt);
}

Level::Level(){};
Level::Level(uint level,GameLayer* currentLayer){
   this->scaleOffset = 2;
   this->currentLayer = currentLayer;
   switch (level){
   case 0:{
      loadChunk("world/area0/level0.tmx","world/area0/backgroundImage.png");
      break;
   }
   case 1:{
      break;
   
   }
   }
}
Level::~Level(){}
void Level::initLevelObjects(){
   //Get object layer
   auto group = level->getObjectGroup("objectsLayer");
   //Clear before we will push new one transition obj
   WorldProperties::levelTransitions.clear();
   //Get all objects(points,poligons,reqtangles etc)
   auto& objects = group->getObjects();
   //Save their properties to memory
   for (auto& obj : objects){
      //Get data from property of tiled map
      cocos2d::ValueMap& dict = obj.asValueMap();
      float x = dict["x"].asFloat()           * scaleOffset;
      float y = dict["y"].asFloat()           * scaleOffset;
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
         cocos2d::Vec2 level_offset;
         level_offset.x = dict["nextLevelOffsetX"].asFloat();
         level_offset.y = dict["nextLevelOffsetY"].asFloat();
         WorldProperties::levelTransitions.push_back(LevelTransition(cocos2d::Rect(x,y,width,height),dict["nextChunk"].asString(),dict["levelBackground"].asString(),level_offset));
      }
      /*Define where will appears enemies*/
      if (dict["name"].asString() == "EnemySpawnPoint")
         WorldProperties::enemySpawnPoint.push_back(cocos2d::Vec2(x,y));
      /*Define where death will be emidiatly*/
      if (dict["name"].asString() == "DeathZone")
         WorldProperties::levelDeathZone.push_back(cocos2d::Rect(x,y,width,height));
   }
}
void Level::initBackground(std::string chunkBackground){
   this->backgroundSprite = cocos2d::Sprite::create(chunkBackground);
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
void Level::update(float dt){
   //Will execute if hero not in NewLocation reqtangle
   for (const auto& lvl : WorldProperties::levelTransitions){
      /*Will execute if hero in NewLocation reqt*/
      if (currentLayer->getChildByName(SceneEntities::gamesession)->getChildByTag(2)->getBoundingBox().intersectsRect(lvl.reqt)){
         //If Hero passed through NewLocation reqt on right side of map
         if (lvl.offset.x == 0 && lvl.offset.y == 0)
            currentLayer->getPlayer()->setPlayerPosition(40,
                                                         currentLayer->getPlayer()->getCreatureSprite()->getPosition().y);
         //If Hero passed through NewLocation reqt on left side of map
         else if (lvl.offset.x == 1 && lvl.offset.y == 0)
            currentLayer->getPlayer()->setPlayerPosition(WorldProperties::mapSize.width  - WorldProperties::screenSize.width - 60,
                                                         currentLayer->getPlayer()->getCreatureSprite()->getPosition().y);
         else if (lvl.offset.x == 0 && lvl.offset.y == 1)
            currentLayer->getPlayer()->setPlayerPosition(currentLayer->getPlayer()->getCreatureSprite()->getPosition().x,
                                                         WorldProperties::mapSize.height - WorldProperties::screenSize.height - 40);
         else if (lvl.offset.x == 1 && lvl.offset.y == 1)
            currentLayer->getPlayer()->setPlayerPosition(currentLayer->getPlayer()->getCreatureSprite()->getPosition().x,
                                                         40);
         //First release memory allocated for previose lvl
         unloadChunk();
         //Then allocate new mem for new lvl
         loadChunk(lvl.path, lvl.backgroundPath);
         //Set up camera on new location
         currentLayer->getChildByName(SceneEntities::gamesession)->stopAllActions();
         currentLayer->getChildByName(SceneEntities::gamesession)->runAction(
              cocos2d::Follow::createWithOffset(
                  currentLayer->getPlayer()->getCreatureSprite(),
                  -100,-100,
                  cocos2d::Rect(
                     0,
                     0,
                     WorldProperties::mapSize.width - WorldProperties::screenSize.width,
                     WorldProperties::mapSize.height - WorldProperties::screenSize.height 
                  )
              )
          );
      }
   }
}

void Level::loadChunk(std::string chunkPath,std::string chunkBackground){
   this->level = cocos2d::TMXTiledMap::create(chunkPath);
   this->level->setScale(scaleOffset);
   level->setMapSize(cocos2d::Size(99,99));
   this->level_layer_midleground = this->level->getLayer("midleground");
   this->currentLayer->getChildByName(SceneEntities::gamesession)->addChild(this->level,ZLevel::BACKGROUND);
   /*Define level size*/
   WorldProperties::mapSize.setSize(this->level->getMapSize().width  * this->level->getTileSize().width  * scaleOffset,
                                    this->level->getMapSize().height * this->level->getTileSize().height * scaleOffset);
   //Init all level objects
   initLevelObjects();
   //Init background image
   initBackground(chunkBackground);
}
void Level::unloadChunk(){
   //Remove all level tiles
   //Remove all level objects
   currentLayer->getChildByName(SceneEntities::gamesession)->removeChild(level);
   currentLayer->getChildByName(SceneEntities::bg)->removeChild(backgroundSprite);
   for (auto& i : level_bodies){
      currentLayer->getChildByName(SceneEntities::gamesession)->removeChild(i);
   }
   
   //Remove all creatures(not hero)
   for (auto & enemy : *(currentLayer->getEnemy()))
      enemy->removeCreature();
   //Clean arr
   currentLayer->getEnemy()->clear();
   //Clean spawn points for new enemies
   WorldProperties::enemySpawnPoint.clear();
   //Clean death zones for new lvl
   WorldProperties::levelDeathZone.clear();
   //Clean all level items for new lvl
   WorldProperties::levelItems.clear();
}
