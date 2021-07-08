#include "Creature.h"
#include "GameLayer.h"

Creature::Creature(std::string texturePath,CreatureType type,cocos2d::Vec2 pos,void* gameLayer,std::string id){
    this->type = type;
    switch(type){
        case CreatureType::HUMANOID:{
            parts.push_back(PartCreature(PartCreatureType::HEAD));
            parts.push_back(PartCreature(PartCreatureType::UPPER_TORSE));
            parts.push_back(PartCreature(PartCreatureType::HAND));
            parts.push_back(PartCreature(PartCreatureType::HAND));
            parts.push_back(PartCreature(PartCreatureType::BUTTOM_TORSE));
            parts.push_back(PartCreature(PartCreatureType::LEG));
            parts.push_back(PartCreature(PartCreatureType::LEG));
            creature_speed  = 30;
            break;
        }
        case CreatureType::ANIMAL:{
            break;
        }
    }
    creature_sprite = cocos2d::Sprite::create(texturePath);
    creature_sprite->setPosition(pos);
    static_cast<GameLayer*>(gameLayer)->addChild(creature_sprite,0,id);
}
Creature::~Creature(){
    parts.clear();
}


Creature::PartCreature::PartCreature(PartCreatureType type){
    this->status    = PartCreatureStatus::NORMAL;
    this->type      = type;
}

///////////////////////////////////////////////////////*Enemy class*///////////////////////////////////////////////////////
Enemy::Enemy(std::string texturePath,CreatureType bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id) :
    Creature(texturePath,bMap,pos,gameLayer,id){
    creature_sprite->setColor(cocos2d::Color3B::RED);
    
}
void Enemy::update(float dt){
    
}

///////////////////////////////////////////////////////*Player class*///////////////////////////////////////////////////////
Player::Player(std::string texturePath,CreatureType bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id) :
    Creature(texturePath,bMap,pos,gameLayer,id){
    enemyNode = static_cast<GameLayer*>(gameLayer)->getEnemy();
    currentlayer = gameLayer;
    currentInteractedEnemy = -1;
}
void Player::update(float dt){
    //For moves
    if (ControlBall::getMoving()){
        creature_sprite->runAction(cocos2d::MoveBy::create(1.f,ControlBall::getDirection()));
    }
    //For attacke
    if (ControlAttc::getAttacke()){
        //Set to default state
        ControlAttc::setAttacke(false);
        //Which enemy will attacke
        interaction_radius.setRect(creature_sprite->getPosition().x - creature_sprite->getBoundingBox().size.width*1.5,creature_sprite->getPosition().y - creature_sprite->getBoundingBox().size.height*1.5,
                     creature_sprite->getBoundingBox().size.width*3,creature_sprite->getBoundingBox().size.height*3);
        for (int i=0; i < enemyNode.size(); ++i){
            if (enemyNode[i]->getCreatureSprite()->getBoundingBox().intersectsRect(interaction_radius)){
                currentInteractedEnemy = i;
            }
        }
        if (currentInteractedEnemy >= 0){
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
                getStatusOfPartsEnemy(enemyNode[currentInteractedEnemy]->getPartsOfCreature());
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
            /*Which will die*/
            /*First clean our OpenGL calls*/
            static_cast<GameLayer*>(currentlayer)->removeChildByName(enemyNode[currentInteractedEnemy]->getCreatureSprite()->getName());
            /*Second clean our Data holder*/
            enemyNode.erase(enemyNode.begin()+currentInteractedEnemy);
            currentInteractedEnemy = -1;
        }
    }
    
}
void Player::getStatusOfPartsEnemy(std::vector<PartCreature> target){
    for (auto &i : target)
        OUT("%s => %s\n",i.getType() == PartCreatureType::HEAD ? "head" : 
                         i.getType() == PartCreatureType::BUTTOM_TORSE ? "buttom torse" : 
                         i.getType() == PartCreatureType::UPPER_TORSE ? "upper torse" : 
                         i.getType() == PartCreatureType::HAND ? "hand" : 
                         i.getType() == PartCreatureType::LEG ? "leg" : 
                         "undefind",
                         i.getStatus() == PartCreatureStatus::NORMAL ? "normal" :
                         i.getStatus() == PartCreatureStatus::CUTTED ? "cutted" :
                         "wonded");
}