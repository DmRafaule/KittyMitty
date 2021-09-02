#pragma once

#include "Enemy.h"

class Boss : public Enemy{
public:
   Boss(CreatureInfo::Type type,std::string for_who,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id);
};