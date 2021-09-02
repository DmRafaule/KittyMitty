#pragma once

#include "Enemy.h"

/**
 * @brief This one describe behavior of bosses
*/
class Boss : public Enemy{
public:
   Boss(CreatureInfo::Type type,std::string for_who,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id);
   virtual void update(float dt) override;
private:
   //Depends on how boss fills, boss stage will change
   void updateBossFight(float dt);
};