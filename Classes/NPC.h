#pragma once 

#include "Enemy.h"
/**
 * @brief This class describe behavior of none-playable characters
*/
class NPC : public Enemy{
public:
   NPC(CreatureInfo::Type type,std::string for_who,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id);
   virtual void update(float dt) override;
   virtual void updateVision() override;
};