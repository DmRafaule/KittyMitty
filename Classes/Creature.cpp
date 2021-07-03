#include "Creature.h"
#include "GameLayer.h"

BodyMap::BodyMap(unsigned int heads,unsigned int torses,unsigned int hands,unsigned int tails,unsigned int legs){
    this->heads = heads;
    this->hands = hands;
    this->torses = torses;
    this->tails = tails;
    this->legs = legs;
}

Creature::Creature(std::string texturePath,BodyMap bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id){
    creature_sprite = cocos2d::Sprite::create(texturePath);
    creature_sprite->setPosition(pos);
    static_cast<GameLayer*>(gameLayer)->addChild(creature_sprite,0,id);
}

Creature::~Creature(){

}