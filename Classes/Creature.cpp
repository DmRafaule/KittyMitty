#include "Creature.h"
#include "GameLayer.h"

///////////////////////////////////////////////////////*Creature class*///////////////////////////////////////////////////////
Creature::Creature(std::string texturePath,CreatureType creature_type,cocos2d::Vec2 pos,void* gameLayer,std::string id){
    isStatisticsShowing = false;
    this->creature_type = creature_type;
    currentlayer = gameLayer;
    switch(creature_type){
        case CreatureType::HUMANOID:{
            creature_parts.push_back(PartCreature(PartCreatureType::HEAD));
            creature_parts.push_back(PartCreature(PartCreatureType::UPPER_TORSE));
            creature_parts.push_back(PartCreature(PartCreatureType::HAND));
            creature_parts.push_back(PartCreature(PartCreatureType::HAND));
            creature_parts.push_back(PartCreature(PartCreatureType::BUTTOM_TORSE));
            creature_parts.push_back(PartCreature(PartCreatureType::LEG));
            creature_parts.push_back(PartCreature(PartCreatureType::LEG));
            creature_speed  = 30;
            creature_stamina = 100;
            creature_blood   = 20;
            break;
        }
        case CreatureType::ANIMAL:{
            break;
        }
    }
    creature_sprite = cocos2d::Sprite::create(texturePath);
    creature_sprite->setPosition(pos);
    static_cast<GameLayer*>(currentlayer)->addChild(creature_sprite,Layer::MIDLEGROUND,id);
}
Creature::~Creature(){
    creature_parts.clear();
}
void Creature::removeSprite(){
    static_cast<GameLayer*>(currentlayer)->removeChildByName(creature_sprite->getName());
}
void Creature::removeStatistics(){
    if (isStatisticsShowing)
        static_cast<GameLayer*>(currentlayer)->removeChild(creature_statistics);
    isStatisticsShowing = false;
}
void Creature::setPart(PartCreatureType part_type, PartCreatureStatus part_status, uint part_density, uint part_penetration){
    for (auto& part: creature_parts){
        if (part.part_type == part_type){
            part.part_status  = part_status;
            part.part_density = part_density;
            part.part_penetration = part_penetration;
        }
    }
}
uint Creature::getPart(PartCreatureType part_type, PartCreatureField part_field){
    for (auto& part : creature_parts){
        if (part.part_type == part_type){
            if (PartCreatureField::STATUS == part_field) return part.part_status;
            else if (PartCreatureField::DENSITY == part_field) return part.part_density;
            else if (PartCreatureField::PENETRATION == part_field) return part.part_penetration;
        }
    }
}
void Creature::getStatistics(){
    if (!isStatisticsShowing){
        isStatisticsShowing = true;
        creature_statistics = cocos2d::Label::createWithTTF("","fonts/arial.ttf",18,cocos2d::Size::ZERO);
        creature_statistics->setPosition(creature_sprite->getPosition());
        static_cast<GameLayer*>(currentlayer)->addChild(creature_statistics);
    }
    else{
        isStatisticsShowing = false;
        static_cast<GameLayer*>(currentlayer)->removeChild(creature_statistics);
    }
}

void Creature::setCreatureSpeed(uint creature_speed){
    this->creature_speed = creature_speed;
}
void Creature::setCreatureBlood(uint creature_blood){
    this->creature_blood = creature_blood;
}
void Creature::setCreatureStamina(uint creature_stamina){
    this->creature_stamina =creature_stamina;
}
void Creature::setStatistics(){
    std::string partStatus;

    /*Set strings about part of body*/
    partStatus.append("Status:\n");
    for (PartCreature &part : creature_parts){
        switch(part.part_type){
            case PartCreatureType::HEAD:{
                partStatus.append("\thead:");
                break;
            }
            case PartCreatureType::UPPER_TORSE:{
                partStatus.append("\tupTorse:");
                break;
            }
            case PartCreatureType::HAND:{
                partStatus.append("\thand:");
                break;
            }
            case PartCreatureType::BUTTOM_TORSE:{
                partStatus.append("\tbotTorse:");
                break;
            }
            case PartCreatureType::LEG:{
                partStatus.append("\tleg:");
                break;
            }
        }
        partStatus.append(part.part_status == PartCreatureStatus::NORMAL ? "norm-" : part.part_status == PartCreatureStatus::WONDED ? "wonded-" : "cutted-");
        partStatus.append(std::to_string(part.part_density) + "-" + std::to_string(part.part_penetration) + "\n");
    }
    /*Set strings about body*/
    partStatus.append("blood:" + std::to_string(creature_blood) + "l\n");    
    partStatus.append("stamina:" + std::to_string(creature_stamina) + "\n");
    creature_statistics->setString(partStatus);
}

///////////////////////////////////////////////////////*PartCreature class*///////////////////////////////////////////////////////
Creature::PartCreature::PartCreature(PartCreatureType part_type){
    this->part_status      = PartCreatureStatus::NORMAL;
    this->part_type        = part_type;
    this->part_penetration = 1;
    switch (part_type){
    case PartCreatureType::HEAD:{
        this->part_density = 10;
        break;
    }
    case PartCreatureType::UPPER_TORSE:{
        this->part_density = 30;     
        break;
    }
    case PartCreatureType::HAND:{
        this->part_density = 10;
        break;
    }
    case PartCreatureType::BUTTOM_TORSE:{
        this->part_density = 20;
        break;
    }
    case PartCreatureType::LEG:{
        this->part_density = 15;
        break;
    }
    }
}

///////////////////////////////////////////////////////*Enemy class*///////////////////////////////////////////////////////
Enemy::Enemy(std::string texturePath,CreatureType bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id) :
    Creature(texturePath,bMap,pos,gameLayer,id){
    creature_sprite->setColor(cocos2d::Color3B::RED);
    
}
void Enemy::update(float dt){
    if (isStatisticsShowing){
        setStatistics();
        creature_statistics->runAction(cocos2d::MoveTo::create(0.2,creature_sprite->getPosition()));
    }
}
void Enemy::attachWeapon(std::string wMap ){

}

///////////////////////////////////////////////////////*Player class*///////////////////////////////////////////////////////
Player::Player(std::string texturePath,CreatureType bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id) :
    Creature(texturePath,bMap,pos,gameLayer,id){
    enemyNode = static_cast<GameLayer*>(gameLayer)->getEnemy();
    currentInteractedEnemy = -1;
}
void Player::update(float dt){
    /*For statistics*/
    if (isStatisticsShowing){
        setStatistics();
        creature_statistics->runAction(cocos2d::MoveTo::create(0.2,creature_sprite->getPosition()));
    }
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
        for (int i=0; i < enemyNode->size(); ++i){
            if (enemyNode->at(i)->getCreatureSprite()->getBoundingBox().intersectsRect(interaction_radius)){
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
                enemyNode->at(currentInteractedEnemy)->setPart(PartCreatureType::HEAD,PartCreatureStatus::WONDED,20,2);
                enemyNode->at(currentInteractedEnemy)->setPart(PartCreatureType::UPPER_TORSE,PartCreatureStatus::CUTTED,0,0);
                enemyNode->at(currentInteractedEnemy)->setPart(PartCreatureType::LEG,PartCreatureStatus::CUTTED,0,0);
                
                /*Which will die*/

                /*First clean engine's calls*/
                enemyNode->at(currentInteractedEnemy)->removeSprite();
                enemyNode->at(currentInteractedEnemy)->removeStatistics();
                /*Second clean game's  calls*/
                enemyNode->erase(enemyNode->begin()+currentInteractedEnemy);
                currentInteractedEnemy = -1;

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
            
        }
    }
    
}
void Player::attachWeapon(std::string wMap ){

}