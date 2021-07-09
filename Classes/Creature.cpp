#include "Creature.h"
#include "GameLayer.h"

///////////////////////////////////////////////////////*Creature class*///////////////////////////////////////////////////////
Creature::Creature(std::string texturePath,CreatureType creature_type,cocos2d::Vec2 pos,void* gameLayer,std::string id){
    this->creature_type = creature_type;
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
    static_cast<GameLayer*>(gameLayer)->addChild(creature_sprite,0,id);
}
Creature::~Creature(){
    creature_parts.clear();
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

void Creature::setCreatureSpeed(uint creature_speed){
    this->creature_speed = creature_speed;
}
void Creature::setCreatureBlood(uint creature_blood){
    this->creature_blood = creature_blood;
}
void Creature::setCreatureStamina(uint creature_stamina){
    this->creature_stamina =creature_stamina;
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
                
                enemyNode[currentInteractedEnemy]->setPart(PartCreatureType::HEAD,PartCreatureStatus::WONDED,20,2);
                enemyNode[currentInteractedEnemy]->setPart(PartCreatureType::UPPER_TORSE,PartCreatureStatus::CUTTED,0,0);
                enemyNode[currentInteractedEnemy]->setPart(PartCreatureType::LEG,PartCreatureStatus::CUTTED,0,0);
                
                
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
            //static_cast<GameLayer*>(currentlayer)->removeChildByName(enemyNode[currentInteractedEnemy]->getCreatureSprite()->getName());
            ///*Second clean our Data holder*/
            //enemyNode.erase(enemyNode.begin()+currentInteractedEnemy);
            currentInteractedEnemy = -1;
        }
    }
    
}