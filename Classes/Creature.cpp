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
    isAlive = true;
}
void Enemy::update(float dt){
    
}

///////////////////////////////////////////////////////*Player class*///////////////////////////////////////////////////////
Player::Player(std::string texturePath,BodyMap bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id) :
    Creature(texturePath,bMap,pos,gameLayer,id){
    enemyNode = static_cast<GameLayer*>(gameLayer)->getEnemy();
    layer = gameLayer;
    currentInteractedEnemy = -1;
}
void Player::update(float dt){
    //For moves
    if (ControlBall::getMoving()){
        creature_sprite->runAction(cocos2d::MoveBy::create(1.f,ControlBall::getDirection()));
    }
    //For attacke
    if (ControlAttc::getAttacke()){
        //Which enemy will attacke
        interaction_radius.setRect(creature_sprite->getPosition().x - creature_sprite->getBoundingBox().size.width*1.5,creature_sprite->getPosition().y - creature_sprite->getBoundingBox().size.height*1.5,
                     creature_sprite->getBoundingBox().size.width*3,creature_sprite->getBoundingBox().size.height*3);
        for (int i=0; i < enemyNode.size(); ++i){
            if (enemyNode[i]->getCreatureSprite()->getBoundingBox().intersectsRect(interaction_radius)){
                currentInteractedEnemy = i;
            }
        }
        /*Type of dammage/attacke for selected enemy*/
        switch(ControlAttc::getDirectionAttacke()){
        case DirectionAttacke::BOTTOMLEFT_TO_TOPRIGHT:{
            OUT("bottomleft to topright\n");
            break;
        }
        case DirectionAttacke::BOTTOMRIGHT_TO_TOPLEFT:{
            OUT("bottomright to topleft\n");
            break;
        }
        case DirectionAttacke::DOWN_TO_TOP:{
            OUT("down to top\n");
            break;
        }
        case DirectionAttacke::LEFT_TO_RIGHT:{
            OUT("left to right\n");
            break;
        }
        case DirectionAttacke::RIGHT_TO_LEFT:{
            OUT("right to left\n");
            break;
        }
        case DirectionAttacke::TOP_TO_DOWN:{
            OUT("top to down\n");
            break;
        }
        case DirectionAttacke::TOPLEFT_TO_BOTTOMRIGHT:{
            OUT("topleft to bottomright\n");
            break;
        }
        case DirectionAttacke::TOPRIGHT_TO_BOTTOMLEFT:{
            OUT("topright to bottomleft\n");
            break;
        }
        }
        ControlAttc::setAttacke(false);
        /*Which will die*/
        if (currentInteractedEnemy >= 0){
            /*First clean our OpenGL calls*/
            static_cast<GameLayer*>(layer)->removeChildByName(enemyNode[currentInteractedEnemy]->getCreatureSprite()->getName());
            /*Second clean our Data holder*/
            enemyNode.erase(enemyNode.begin()+currentInteractedEnemy);
            currentInteractedEnemy = -1;
        }
    }
    
}