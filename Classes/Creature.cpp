#include "Creature.h"
#include "GameLayer.h"

BodyMap::BodyMap(unsigned int heads,unsigned int torses,unsigned int hands,unsigned int tails,unsigned int legs,unsigned int speedLimit){
    this->heads = heads;
    this->hands = hands;
    this->torses = torses;
    this->tails = tails;
    this->legs = legs;
    this->speedLimit = speedLimit;
}

Creature::Creature(std::string texturePath,BodyMap bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id){
    body_map = new BodyMap(bMap.heads,bMap.torses,bMap.hands,bMap.tails,bMap.legs,bMap.speedLimit);
    creature_sprite = cocos2d::Sprite::create(texturePath);
    creature_sprite->setPosition(pos);
    static_cast<GameLayer*>(gameLayer)->addChild(creature_sprite,0,id);
}

Creature::~Creature(){
    delete body_map;
}

///////////////////////////////////////////////////////*Enemy class*///////////////////////////////////////////////////////
Enemy::Enemy(std::string texturePath,BodyMap bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id) :
    Creature(texturePath,bMap,pos,gameLayer,id){
    creature_sprite->setColor(cocos2d::Color3B::RED);
    playerNode = static_cast<GameLayer*>(gameLayer)->getChildByName(LayerChild::player);
}
void Enemy::update(float dt){
    cocos2d::Rect rect;
    /*here setup normal position*/
    rect.setRect(playerNode->getPosition().x,playerNode->getPosition().y,playerNode->getBoundingBox().size.width*2,playerNode->getBoundingBox().size.height*2);
    if (creature_sprite->getBoundingBox().intersectsRect(rect)){
        printf("collision\t%d\n",i);
        i++;   
    }
}

///////////////////////////////////////////////////////*Player class*///////////////////////////////////////////////////////
Player::Player(std::string texturePath,BodyMap bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id) :
    Creature(texturePath,bMap,pos,gameLayer,id){

}
void Player::update(float dt){
    if (GameUI::Control_Ball::getMoving()){
        creature_sprite->runAction(cocos2d::MoveBy::create(1.f,GameUI::Control_Ball::getDirection()));
    }
}