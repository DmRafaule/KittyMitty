#pragma once 

#include <cocos2d.h>

class World {
public:
   World(std::string world_file_path,cocos2d::Node* currentLayer);
   ~World();
   void update(float dt);
   inline const cocos2d::Vec2 getPlayerSpawnPoint() { return playerSpawnPoint; };
   inline const std::vector<cocos2d::Vec2> getEnemySpawnPoint() { return enemySpawnPoint; };
   inline const cocos2d::Size getMapSize() { return mapSize; };
   inline const cocos2d::Rect getLevelEnd() { return levelEnd; };
private:
   cocos2d::TMXTiledMap* level;
   cocos2d::TMXLayer* level_layer_midleground;
   static std::vector<std::string> level_obj_groupes;
   cocos2d::Node* currentLayer;

   cocos2d::Size mapSize;
   cocos2d::Vec2 playerSpawnPoint;
   std::vector<cocos2d::Vec2> enemySpawnPoint;
   cocos2d::Rect   levelEnd;
};