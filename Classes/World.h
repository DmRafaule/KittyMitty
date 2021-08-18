#pragma once 

#include <cocos2d.h>

class Level;
class Chunk;
class GameLayer;


class World {
public:
   World();
   World(uint level,GameLayer* currentLayer);
   virtual ~World();
   virtual void update(float dt);
private:
   Level* level;
};
class Level : public World{
public:
   Level();
   Level(uint level,GameLayer* currentLayer);
   virtual ~Level();
   virtual void update(float dt) override;
private:
   void switchLevel(const LevelNonePhysicalObj& lvl);
   void loadLevel(std::string chunkPath,std::string chunkBackground);
   void unloadLevel();

   void parseLevelLayers(std::string chunkPath);//Parse data from .tmx file to game(about layers)
   void parseLevelObjects();//Parse data from .tmx file to game(about objects) and init static obj
      void parseDynObjects(cocos2d::ValueMap& dict, cocos2d::Rect& rect);//Parse dynamic physical objeccts from .tmx
      void parseStaticNonePhysicalObj(cocos2d::ValueMap& dict, cocos2d::Rect& rect);//Parse static none physical objeccts from .tmx
      void parseStaticPhysicalObj(cocos2d::ValueMap& dict, cocos2d::Rect& rect);//Parse static physical objeccts from .tmx
   void initDynamicObjects();//Init dynamic obj
   void initPoolActions();//push up to poolActions all needed actions
   void initBackground(std::string chunkBackground);//Init bg image
   void initCreatures();//Init Creatures on level

   void addAnimation(std::string anim_name,uint frame_number,float delay,bool restoreOrigFr);//Add animation to poolActions
private:
   cocos2d::TMXTiledMap* level;
   cocos2d::TMXLayer* level_layer_midleground;
   std::vector<cocos2d::Node*> level_bodies;
   std::vector<cocos2d::Sprite*> level_dynamic_obj;
   cocos2d::Sprite* level_bgSprite;
   GameLayer* currentLayer;

   float scaleOffset;
};