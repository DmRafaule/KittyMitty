#include "GameLayer.h"
#include "engEnums.hpp"
#include "engMacros.hpp"
#include "GameActions.hpp"
#include "Enemy.h"
#include "Boss.h"
#include "NPC.h"

cocos2d::Size               WorldProperties::screenSize = cocos2d::Size();
cocos2d::Size               WorldProperties::mapSize = cocos2d::Size();
std::multimap<CreatureInfo::Type,LevelCreatures> WorldProperties::creatureObj;
std::multimap<std::string,LevelPhysicalObj> WorldProperties::dynamicObj;
std::map<std::string,cocos2d::Action*> WorldProperties::actionPool;
std::vector<LevelNonePhysicalObj> WorldProperties::staticObj(0);

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
LevelPhysicalObj::LevelPhysicalObj(std::string frameName,std::string typeAction, cocos2d::Rect rect,std::string nameTarget){
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
   initPoolActions();
   switch (level){
   case 0:{
      loadLevel("world/area0/playground.tmx","world/area0/backgroundImage.png");
      break;
   }
   case 1:{
      break;
   
   }
   }
}
Level::~Level(){}
void Level::parseLevelLayers(std::string chunkPath){
   this->level = cocos2d::TMXTiledMap::create(chunkPath);
   this->level->setScale(scaleOffset);
   this->level->setMapSize(cocos2d::Size(99,99));
   this->level_layer_midleground = this->level->getLayer("midleground");
   this->currentLayer->getChildByName(SceneLayer::gamesession)->addChild(this->level,SceneZOrder::BACKGROUND);
   /*Define level size*/
   WorldProperties::mapSize.setSize(this->level->getMapSize().width  * this->level->getTileSize().width  * scaleOffset,
                                    this->level->getMapSize().height * this->level->getTileSize().height * scaleOffset);
}
void Level::parseLevelObjects(){
   //Get object layer
   auto group = level->getObjectGroup("objectsLayer");
   //Clear before we will push new one transition obj
   WorldProperties::staticObj.clear();
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
      cocos2d::Rect rect(x,y,width,height);
      
      parseStaticPhysicalObj(dict,rect);//Maybe in the future you need add type for all physical object, maybe
      
      if (dict["type"].asString() == "dynamicObj"){
         parseDynObjects(dict,rect);
      }
      else if (dict["type"].asString() == "staticObj"){
         parseStaticNonePhysicalObj(dict,rect);
      }
      else if (dict["type"].asString() == "creatureObj"){
         parseCreatureObj(dict,rect);         
      }
   }
}
void Level::parseDynObjects(cocos2d::ValueMap& dict, cocos2d::Rect& rect){
   LevelPhysicalObj obj;
   obj.frameName     = dict["frameName"].asString();
   obj.typeAction    = dict["typeAction"].asString();
   obj.targetID      = dict["targetID"].asInt();
   obj.targetAction  = dict["targetAction"].asString();
   obj.isCollided    = dict["isCollided"].asBool();
   obj.id            = dict["id"].asUnsignedInt();
   obj.rect          = rect;

   WorldProperties::dynamicObj.emplace(dict["name"].asString(),obj);
}
void Level::parseStaticNonePhysicalObj(cocos2d::ValueMap& dict, cocos2d::Rect& rect){
   cocos2d::Vec2 level_offset;
   level_offset.x = dict["nextLevelOffsetX"].asFloat();
   level_offset.y = dict["nextLevelOffsetY"].asFloat();

   LevelNonePhysicalObj obj;
   obj.name = dict["name"].asString();
   obj.path = dict["nextChunk"].asString();
   obj.backgroundPath = dict["levelBackground"].asString();
   obj.offset = level_offset;
   obj.reqt = rect;
   
   WorldProperties::staticObj.push_back(obj);
}
void Level::parseStaticPhysicalObj(cocos2d::ValueMap& dict, cocos2d::Rect& rect){
   //Init node for add in physic scene physic bodies
   level_bodies.push_back(cocos2d::Node::create());
   auto ground_body = cocos2d::PhysicsBody::createBox(cocos2d::Size(rect.size.width,rect.size.height));
   ground_body->setDynamic(false);
   ground_body->setGravityEnable(false);
   ground_body->setPositionOffset(cocos2d::Vec2(rect.size.width/2,rect.size.height/2));
   ground_body->setContactTestBitmask(0xFF);

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
   level_bodies.back()->setPosition(rect.origin.x, rect.origin.y );
   currentLayer->getChildByName(SceneLayer::gamesession)->addChild(level_bodies.back());
}
void Level::parseCreatureObj(cocos2d::ValueMap& dict, cocos2d::Rect& rect){
   LevelCreatures obj;
   obj.typeCr     = dict["typeCreature"].asInt();
   obj.typeWepon  = dict["typeWeapon"].asInt();
   obj.typeAI     = dict["typeAI"].asInt();
   obj.typeBehavior = dict["typeBehavior"].asString();
   obj.name       = dict["name"].asString();
   obj.point      = rect.origin;

   WorldProperties::creatureObj.emplace(static_cast<CreatureInfo::Type>(obj.typeCr),obj);
}
void Level::addAnimation(std::string anim_name,uint frame_number,float delay,bool restoreOrigFr){
   auto animation = cocos2d::Animation::create();
   for (uint i = 0; i < frame_number;++i){
       std::string name = anim_name + std::to_string(i) + ".png";
       auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
       animation->addSpriteFrame(frame);
       
   }
   animation->setDelayPerUnit(delay);
   animation->setRestoreOriginalFrame(restoreOrigFr);
   WorldProperties::actionPool.emplace(anim_name,cocos2d::Animate::create(animation));
   WorldProperties::actionPool.find(anim_name)->second->retain();
}
void Level::initPoolActions(){
   addAnimation("door_open",5,0.1,false);
   addAnimation("lever",6,0.1,false);
   addAnimation("button_push",3,0.1,true);

   cocos2d::Action* MoveHorisontal = cocos2d::RepeatForever::create(cocos2d::Sequence::create(PhysicMoveBy::create(2,cocos2d::Vec2(-50,0)),PhysicMoveBy::create(2,cocos2d::Vec2(50,0)),nullptr));
   cocos2d::Action* MoveVertical   = cocos2d::RepeatForever::create(cocos2d::Sequence::create(PhysicMoveBy::create(2,cocos2d::Vec2(0,100)),PhysicMoveBy::create(2,cocos2d::Vec2(0,-100)),nullptr));
   cocos2d::Action* RotateClW = cocos2d::RepeatForever::create(PhysicRotateBy::create(2,10));//Here is a bug some platform expand their angelSpeed very fast
   cocos2d::Action* RotateOClW = cocos2d::RepeatForever::create(PhysicRotateBy::create(2,-10));
   cocos2d::Action* HangOuting = cocos2d::RepeatForever::create(cocos2d::EaseInOut::create(cocos2d::Sequence::create(cocos2d::MoveBy::create(0.8,cocos2d::Vec2(0,20)),cocos2d::MoveBy::create(0.5,cocos2d::Vec2(0,-20)),nullptr),1.4));

   WorldProperties::actionPool.emplace("moveH",MoveHorisontal);
   WorldProperties::actionPool.emplace("moveV",MoveVertical);
   WorldProperties::actionPool.emplace("rotateClockWise",RotateClW);
   WorldProperties::actionPool.emplace("rotateOpositClockWise",RotateOClW);
   WorldProperties::actionPool.emplace("hangOuting",HangOuting);
   WorldProperties::actionPool.emplace("nill",cocos2d::MoveBy::create(0,cocos2d::Vec2(0,0)));

   MoveHorisontal->retain();
   MoveVertical->retain();
   RotateClW->retain();
   RotateOClW->retain();
   HangOuting->retain();
   WorldProperties::actionPool.find("nill")->second->retain();
}
void Level::initDynamicObjects(){
   for (auto & obj : WorldProperties::dynamicObj){
      cocos2d::Sprite* spr = cocos2d::Sprite::createWithSpriteFrameName(obj.second.frameName);
      obj.second.spr = spr;
      //Transform position
      spr->setPosition(obj.second.rect.origin.x + obj.second.rect.size.width/2,obj.second.rect.origin.y + obj.second.rect.size.height/2);
      //Init physic body like floor
      auto spr_ph = cocos2d::PhysicsBody::createBox(obj.second.rect.size/2);
      spr_ph->setDynamic(false);
      spr_ph->setGravityEnable(false);
      spr_ph->setCollisionBitmask(obj.second.isCollided);
      spr_ph->setContactTestBitmask(0xFF);
      spr->setPhysicsBody(spr_ph);
      if (obj.second.frameName == "swordStock.png" || obj.second.frameName == "spearStock.png" || obj.second.frameName == "axeStock.png" || obj.second.frameName == "longsword.png")
         spr->setScale(scaleOffset + 3);
      else
         spr->setScale(scaleOffset);
      /*This is prevent of bluring my textures*/
      cocos2d::Texture2D::TexParams tpar = {
          cocos2d::backend::SamplerFilter::NEAREST,
          cocos2d::backend::SamplerFilter::NEAREST,
          cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE,
          cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE
      };
      spr->getTexture()->setTexParameters(tpar);
      level_dynamic_obj.push_back(spr);
      //Init action for this object
      spr->runAction(WorldProperties::actionPool.find(obj.second.typeAction)->second->clone());
      currentLayer->getChildByName(SceneLayer::gamesession)->addChild(level_dynamic_obj.back(),0,obj.second.id+100);//100 because some game objects somewhere till 100
   }
   //Set up target for targeting Obj(such as lever or buttons)
   for (auto & obj : WorldProperties::dynamicObj){
      obj.second.target = currentLayer->getChildByName(SceneLayer::gamesession)->getChildByTag(obj.second.targetID+100);
   }
}
void Level::initBackground(std::string chunkBackground){
   this->level_bgSprite = cocos2d::Sprite::create(chunkBackground);
   cocos2d::Texture2D::TexParams tpar = {
      cocos2d::backend::SamplerFilter::NEAREST,
      cocos2d::backend::SamplerFilter::NEAREST,
      cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE,
      cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE
   };
   level_bgSprite->getTexture()->setTexParameters(tpar);
   level_bgSprite->setPosition(WorldProperties::screenSize.width/2,WorldProperties::screenSize.height/2);
   level_bgSprite->setScale(MAX(WorldProperties::screenSize.width/level_bgSprite->getBoundingBox().size.width,
                                  WorldProperties::screenSize.height/level_bgSprite->getBoundingBox().size.height));
   currentLayer->getChildByName(SceneLayer::bg)->addChild(level_bgSprite);
}
void Level::initCreatures(){
   for (auto& en : WorldProperties::creatureObj){
      Enemy* e;
      if (en.first != CreatureInfo::Type::KITTYMITTY && en.first != CreatureInfo::Type::BOSS && en.first != CreatureInfo::Type::NPC){
         e = new Enemy(en.first,en.second.point,currentLayer->getChildByName(SceneLayer::gamesession),currentLayer->getEnemy()->size()+6);
         e->setWeapon((WeaponType)en.second.typeWepon);
         e->setAI(en.second.typeAI,en.second.typeBehavior);
         e->initPlayerDependenceFields();
         currentLayer->getEnemy()->push_back(e);
      }
      else if (en.first == CreatureInfo::Type::BOSS){
         e = new Boss(en.first,en.second.name,en.second.point,currentLayer->getChildByName(SceneLayer::gamesession),currentLayer->getEnemy()->size()+6);
         e->setWeapon((WeaponType)en.second.typeWepon);
         e->setAI(en.second.typeAI,en.second.typeBehavior);
         e->initPlayerDependenceFields();
         currentLayer->getEnemy()->push_back(e);
      }
      else if (en.first == CreatureInfo::Type::NPC){
         e = new NPC(en.first,en.second.name,en.second.point,currentLayer->getChildByName(SceneLayer::gamesession),currentLayer->getEnemy()->size()+6);
         //e->setWeapon((WeaponType)en.second.typeWepon);
         e->setAI(en.second.typeAI,en.second.typeBehavior);
         e->initPlayerDependenceFields();
         currentLayer->getEnemy()->push_back(e);
      }
   }
}
void Level::update(float dt){
   for (const auto& lvl : WorldProperties::staticObj){
      if (currentLayer->getChildByName(SceneLayer::gamesession)->getChildByTag(2)->getBoundingBox().intersectsRect(lvl.reqt)){
         /*Will execute if hero in NewLocation reqt*/
         if (lvl.name == "NewLocation"){
            switchLevel(lvl);
         }
         /*Will execute if hero in NewLocation reqt*/
         else if (lvl.name == "DeathZone"){
            OUT("death zone\n");
         }
      }
   }
}
void Level::switchLevel(const LevelNonePhysicalObj& lvl){
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
   unloadLevel();
   //Then allocate new mem for new lvl
   loadLevel(lvl.path, lvl.backgroundPath);
   //Set up camera on new location
   currentLayer->getChildByName(SceneLayer::gamesession)->stopAllActions();
   currentLayer->getChildByName(SceneLayer::gamesession)->runAction(
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
void Level::loadLevel(std::string chunkPath,std::string chunkBackground){
   parseLevelLayers(chunkPath);
   parseLevelObjects();
   initDynamicObjects();
   initBackground(chunkBackground);
   initCreatures();
}
void Level::unloadLevel(){
   //Remove all level tiles
   currentLayer->getChildByName(SceneLayer::gamesession)->removeChild(level);
   //Remove all level objects
   currentLayer->getChildByName(SceneLayer::bg)->removeChild(level_bgSprite);
   for (auto& i : level_bodies){
      currentLayer->getChildByName(SceneLayer::gamesession)->removeChild(i);
   }
   //Remove all dynamic obj
   WorldProperties::dynamicObj.clear();//Erase data about tiled obj
   for (auto & dObj : level_dynamic_obj)//Erase dyn obj from eng calls 
      currentLayer->getChildByName(SceneLayer::gamesession)->removeChild(dObj);
   //Erase data from container
   level_dynamic_obj.clear();
   //Remove all creatures(not hero)
   for (auto & enemy : *(currentLayer->getEnemy()))
      enemy->remove();
   //Clean arr of enemies
   currentLayer->getEnemy()->clear();
   //Clean spawn points for new enemies
   WorldProperties::creatureObj.clear();
}
