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
   void loadChunk(std::string chunkPath,std::string chunkBackground);
   void unloadChunk();

   void initLevelLayers(std::string chunkPath);
   void initLevelObjects();
   void initBackground(std::string chunkBackground);
   void initCreatures();
private:
   cocos2d::TMXTiledMap* level;
   cocos2d::TMXLayer* level_layer_midleground;
   std::vector<cocos2d::Node*> level_bodies;
   GameLayer* currentLayer;
   cocos2d::Sprite* backgroundSprite;

   float scaleOffset;
   uint  creatureIndex;
};
