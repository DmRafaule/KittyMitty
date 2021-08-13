#pragma once 

#include <cocos2d.h>

class Level;
class Chunk;

class World {
public:
   World();
   World(uint level,cocos2d::Node* currentLayer);
   virtual ~World();
   virtual void update(float dt);
private:
   Level* level;
};
class Level : public World{
public:
   Level();
   Level(uint level,cocos2d::Node* currentLayer);
   virtual ~Level();
   virtual void update(float dt) override;
private:
   void initLevelObjects();
   void initBackground();
   void loadChunk(std::string chunkPath, cocos2d::Vec2 offset);
   void unloadChunk();
private:
   cocos2d::TMXTiledMap* level;
   cocos2d::TMXLayer* level_layer_midleground;
   std::vector<cocos2d::Node*> level_bodies;
   cocos2d::Node* currentLayer;
   cocos2d::Sprite* backgroundSprite;
   cocos2d::Vec2 level_offset;

   float scaleOffset;
   bool isNewChunk = false;
};
