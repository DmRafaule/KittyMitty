#include "GameLayer.h"
#include "engEnums.hpp"
#include "engMacros.hpp"
#include "GameActions.hpp"

cocos2d::Size               WorldProperties::screenSize = cocos2d::Size();
cocos2d::Size               WorldProperties::mapSize = cocos2d::Size();
cocos2d::Vec2               WorldProperties::playerSpawnPoint = cocos2d::Vec2();
std::vector<std::pair<std::string,cocos2d::Rect>>  WorldProperties::levelItems(0);
std::vector<LevelCreatures> WorldProperties::creatureObj(0);
std::vector<LevelNonePhysicalObj> WorldProperties::interactiveObj(0);
std::vector<LevelPhysicalObj> WorldProperties::dynamicObj(0);

LevelNonePhysicalObj::LevelNonePhysicalObj(){}
LevelNonePhysicalObj::LevelNonePhysicalObj(cocos2d::Rect reqt,std::string path,std::string backgroundPath,cocos2d::Vec2 offset,std::string name){
   this->reqt = reqt;
   this->path = path;
   this->backgroundPath = backgroundPath;
   this->offset = offset;
   this->name = name;
}
LevelCreatures::LevelCreatures(){}
LevelCreatures::LevelCreatures(uint typeCr,uint typeWepon,cocos2d::Vec2 point){
   this->point = point;
   this->typeCr = typeCr;
   this->typeWepon = typeWepon;
}
LevelPhysicalObj::LevelPhysicalObj(){}
LevelPhysicalObj::LevelPhysicalObj(std::string frameName,std::string typeAction, cocos2d::Rect rect){
   this->frameName = frameName; 
   this->typeAction = typeAction;
   this->rect = rect;
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
   this->creatureIndex = 6;
   initDynamicActions();
   switch (level){
   case 0:{
      loadChunk("world/area0/level1.tmx","world/area0/backgroundImage.png");
      break;
   }
   case 1:{
      break;
   
   }
   }
}
Level::~Level(){}
void Level::initLevelLayers(std::string chunkPath){
   this->level = cocos2d::TMXTiledMap::create(chunkPath);
   this->level->setScale(scaleOffset);
   this->level->setMapSize(cocos2d::Size(99,99));
   this->level_layer_midleground = this->level->getLayer("midleground");
   this->currentLayer->getChildByName(SceneEntities::gamesession)->addChild(this->level,ZLevel::BACKGROUND);
   /*Define level size*/
   WorldProperties::mapSize.setSize(this->level->getMapSize().width  * this->level->getTileSize().width  * scaleOffset,
                                    this->level->getMapSize().height * this->level->getTileSize().height * scaleOffset);
}
void Level::initLevelObjects(){
   //Get object layer
   auto group = level->getObjectGroup("objectsLayer");
   //Clear before we will push new one transition obj
   WorldProperties::interactiveObj.clear();
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
         WorldProperties::interactiveObj.push_back(LevelNonePhysicalObj(cocos2d::Rect(x,y,width,height),dict["nextChunk"].asString(),dict["levelBackground"].asString(),level_offset,dict["name"].asString()));
      }
      /*Define where will appears enemies*/
      if (dict["name"].asString() == "EnemySpawnPoint"){
         WorldProperties::creatureObj.push_back(LevelCreatures(dict["typeCreature"].asInt(),dict["typeWeapon"].asInt(),cocos2d::Vec2(x,y)));
      }
      /*Define where death will be emidiatly*/
      if (dict["name"].asString() == "DeathZone"){
         cocos2d::Vec2 level_offset;
         level_offset.x = -1;
         level_offset.y = -1;
         WorldProperties::interactiveObj.push_back(LevelNonePhysicalObj(cocos2d::Rect(x,y,width,height),"null","null",level_offset,dict["name"].asString()));
      }
      //Define where will be platforms(dynamic)
      if (dict["name"].asString() == "platform"){//HERE make push up for dynobj
         WorldProperties::dynamicObj.push_back(LevelPhysicalObj(dict["frameName"].asString(),dict["typeAction"].asString(),cocos2d::Rect(x,y,width,height)));
      }
   }
}
void Level::initDynamicActions(){
   cocos2d::Action* MoveHorisontal = cocos2d::RepeatForever::create(cocos2d::Sequence::create(PhysicMoveBy::create(2,cocos2d::Vec2(-50,0)),PhysicMoveBy::create(2,cocos2d::Vec2(50,0)),nullptr));
   cocos2d::Action* MoveVertical   = cocos2d::RepeatForever::create(cocos2d::Sequence::create(PhysicMoveBy::create(2,cocos2d::Vec2(0,100)),PhysicMoveBy::create(2,cocos2d::Vec2(0,-100)),nullptr));
   cocos2d::Action* RotateClW = cocos2d::RepeatForever::create(PhysicRotateBy::create(2,20));
   cocos2d::Action* RotateOClW = cocos2d::RepeatForever::create(PhysicRotateBy::create(2,-20));

   actionPool.emplace("moveH",MoveHorisontal);
   actionPool.emplace("moveV",MoveVertical);
   actionPool.emplace("rotateClockWise",RotateClW);
   actionPool.emplace("rotateOpositClockWise",RotateOClW);
   
   MoveHorisontal->retain();
   MoveVertical->retain();
   RotateClW->retain();
   RotateOClW->retain();
}
void Level::initDynamicObjects(){
   for (auto & obj : WorldProperties::dynamicObj){
      cocos2d::Sprite* spr = cocos2d::Sprite::createWithSpriteFrameName(obj.frameName);
      //Transform position
      spr->setPosition(obj.rect.origin.x + obj.rect.size.width/2,obj.rect.origin.y + obj.rect.size.height/2);
      //Init physic body like floor
      auto spr_ph = cocos2d::PhysicsBody::createBox(obj.rect.size/2);
      spr_ph->setDynamic(false);
      spr_ph->setGravityEnable(false);
      spr_ph->setCollisionBitmask(0x01);
      spr_ph->setContactTestBitmask(0xFF);
      spr->setPhysicsBody(spr_ph);
      spr->setScale(scaleOffset);
      /*This is prevent of bluring my textures*/
      cocos2d::Texture2D::TexParams tpar = {
          cocos2d::backend::SamplerFilter::NEAREST,
          cocos2d::backend::SamplerFilter::NEAREST,
          cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE,
          cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE
      };
      //Init action for this object
      spr->runAction(actionPool.find(obj.typeAction)->second->clone());
      spr->getTexture()->setTexParameters(tpar);
      level_dynamic_obj.push_back(spr);
      currentLayer->getChildByName(SceneEntities::gamesession)->addChild(level_dynamic_obj.back());
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
void Level::initCreatures(){
   for (auto& en : WorldProperties::creatureObj){
      Enemy* e;
      e = new Enemy((CreatureInfo::Type)en.typeCr,en.point,currentLayer->getChildByName(SceneEntities::gamesession),creatureIndex);
      e->setWeapon((WeaponType)en.typeWepon);
      creatureIndex++;
      currentLayer->getEnemy()->push_back(e);
   }
}
void Level::update(float dt){
   for (const auto& lvl : WorldProperties::interactiveObj){
      if (currentLayer->getChildByName(SceneEntities::gamesession)->getChildByTag(2)->getBoundingBox().intersectsRect(lvl.reqt)){
         /*Will execute if hero in NewLocation reqt*/
         if (lvl.name == "NewLocation"){
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
         else if (lvl.name == "DeathZone"){
            OUT("death zone\n");
         }
      }
   }
}

void Level::loadChunk(std::string chunkPath,std::string chunkBackground){
   initLevelLayers(chunkPath);
   //Init all level objects
   initLevelObjects();
   //Init moving platforms;
   initDynamicObjects();
   //Init background image
   initBackground(chunkBackground);
   //Init creatures
   initCreatures();
}
void Level::unloadChunk(){
   creatureIndex = 6;
   //Remove all level tiles
   //Remove all level objects
   currentLayer->getChildByName(SceneEntities::gamesession)->removeChild(level);
   currentLayer->getChildByName(SceneEntities::bg)->removeChild(backgroundSprite);
   for (auto& i : level_bodies){
      currentLayer->getChildByName(SceneEntities::gamesession)->removeChild(i);
   }
   //Remove all dynamic obj
   WorldProperties::dynamicObj.clear();//Erase data about tiled obj
   for (auto & dObj : level_dynamic_obj)//Erase dyn obj from eng calls 
      currentLayer->getChildByName(SceneEntities::gamesession)->removeChild(dObj);
   level_dynamic_obj.clear();//Erase data from container
   //level_dynamic_obj.clear();
   //Remove all creatures(not hero)
   for (auto & enemy : *(currentLayer->getEnemy()))
      enemy->removeCreature();
   //Clean arr
   currentLayer->getEnemy()->clear();
   //Clean spawn points for new enemies
   WorldProperties::creatureObj.clear();
   //Clean all level items for new lvl
   WorldProperties::levelItems.clear();
}
