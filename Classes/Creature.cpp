#include "Creature.h"
#include "GameLayer.h"
#include <dirent.h>
///////////////////////////////////////////////////////*Creature class*///////////////////////////////////////////////////////
Creature::Creature(CreatureInfo info, cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id){
    this->isStatisticsShowing = false;
    this->isNewState = false;
    this->creature_info.type = info.type;
    this->creature_info.state = CreatureInfo::State::IN_FALL;
    this->currentLayer = gameLayer;
    switch(creature_info.type){
        case CreatureInfo::Type::HUMANOID:{
            creature_parts.push_back(PartCreature(PartCreatureType::HEAD));
            creature_parts.push_back(PartCreature(PartCreatureType::UPPER_TORSE));
            creature_parts.push_back(PartCreature(PartCreatureType::HAND_LEFT));
            creature_parts.push_back(PartCreature(PartCreatureType::HAND_RIGHT));
            creature_parts.push_back(PartCreature(PartCreatureType::BUTTOM_TORSE));
            creature_parts.push_back(PartCreature(PartCreatureType::LEG_LEFT));
            creature_parts.push_back(PartCreature(PartCreatureType::LEG_RIGHT));
            creature_info.characteristic.velocity_limit  = 200;
            creature_info.characteristic.jump_power = 120;
            creature_info.characteristic.acceleration_power = 45;
            creature_info.characteristic.stamina = 100;
            creature_info.characteristic.stamina_limit = 100;
            creature_info.characteristic.blood   = 20;
            creature_info.characteristic.jump_ability = 2;
            creature_info.characteristic.current_jump_ability_num = 0;
            creature_info.characteristic.mass = 10;
            break;
        }
        case CreatureInfo::Type::ANIMAL:{
            break;
        }
    }
    initAnimations(info.animation);
    creature_sprite = cocos2d::Sprite::createWithSpriteFrameName(info.animation.animationForWho + "_animation_idle0.png");
    creature_physic_body = cocos2d::PhysicsBody::createEdgeBox(cocos2d::Size(creature_sprite->getBoundingBox().size.width/2,creature_sprite->getBoundingBox().size.height),cocos2d::PhysicsMaterial(0,0,1.5));
    creature_physic_body->setMass(creature_info.characteristic.mass);
    creature_physic_body->setDynamic(true);
    creature_physic_body->setVelocityLimit(creature_info.characteristic.velocity_limit);
    creature_physic_body->setGravityEnable(true);
    creature_physic_body->setCollisionBitmask(id);
    creature_physic_body->setContactTestBitmask(0xFF);
    creature_sprite->setPhysicsBody(creature_physic_body);
    creature_sprite->setPosition(pos);
    creature_sprite->setScale(5);
    /*This is prevent of bluring my textures*/
    cocos2d::Texture2D::TexParams tpar = {
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE
    };
    creature_sprite->getTexture()->setTexParameters(tpar);
    currentLayer->addChild(creature_sprite,ZLevel::MIDLEGROUND,id);
    creature_sprite->runAction(animation_idle);
}
void Creature::initAnimations(CreatureInfo::Animation infoAnimation){

    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("textures/animations/" + infoAnimation.animationForWho + "/animationSheet.plist");
    currentLayer->addChild(cocos2d::SpriteBatchNode::create("textures/animations/" + infoAnimation.animationForWho + "/animationSheet.png"));

    auto animation = cocos2d::Animation::create();
    for (uint i = 0; i < infoAnimation.framesIdleNum[0];++i){
        std::string name = infoAnimation.animationForWho + "_animation_idle" + std::to_string(i) + ".png";
        auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
        animation->addSpriteFrame(frame);
    }
    animation->setDelayPerUnit(0.35f);
    animation->setRestoreOriginalFrame(true);
    animation_idle = cocos2d::Animate::create(animation);
    animation_idle->retain();

    auto animation1 = cocos2d::Animation::create();
    for (uint i = 0; i < infoAnimation.framesIdleNum[1];++i){
        std::string name = infoAnimation.animationForWho + "_animation_run" + std::to_string(i) + ".png";
        auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
        animation1->addSpriteFrame(frame);
    }
    animation1->setDelayPerUnit(0.15f);
    animation1->setRestoreOriginalFrame(false);
    animation_speedUp = cocos2d::Animate::create(animation1);
    animation_speedUp->retain();

    auto animation2 = cocos2d::Animation::create();
    for (uint i = 4; i < infoAnimation.framesIdleNum[2] + 4 ;++i){
        std::string name = infoAnimation.animationForWho + "_animation_run" + std::to_string(i) + ".png";
        auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
        animation2->addSpriteFrame(frame);
    }
    animation2->setDelayPerUnit(0.2f);
    animation2->setRestoreOriginalFrame(false);
    animation_run = cocos2d::Animate::create(animation2);
    animation_run->retain();

    auto animation3 = cocos2d::Animation::create();
    for (uint i = 11; i < infoAnimation.framesIdleNum[3] + 11;++i){
        std::string name = infoAnimation.animationForWho + "_animation_run" + std::to_string(i) + ".png";
        auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
        animation3->addSpriteFrame(frame);
        
    }
    animation3->setDelayPerUnit(0.3f);
    animation3->setRestoreOriginalFrame(false);
    animation_slowdown = cocos2d::Animate::create(animation3);
    animation_slowdown->retain();

    auto animation4 = cocos2d::Animation::create();
    for (uint i = 15; i < infoAnimation.framesIdleNum[4] + 15;++i){
        std::string name = infoAnimation.animationForWho + "_animation_run" + std::to_string(i) + ".png";
        auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
        animation4->addSpriteFrame(frame);
        
    }
    animation4->setDelayPerUnit(0.2f);
    animation4->setRestoreOriginalFrame(false);
    animation_braking = cocos2d::Animate::create(animation4);
    animation_braking->retain();

}
Creature::~Creature(){
    creature_parts.clear();
}
void Creature::removeSprite(){
    currentLayer->removeChildByName(creature_sprite->getName());
}
void Creature::removeStatistics(){
    if (isStatisticsShowing)
        currentLayer->removeChild(creature_statistics);
    isStatisticsShowing = false;
}
void Creature::setPart(PartCreatureType part_type, PartCreatureStatus part_status, uint part_densityDef){
    for (auto& part: creature_parts){
        if (part.part_type == part_type){
            part.part_status  = part_status;
            part.part_densityDef = part_densityDef;
        }
    }
}
uint Creature::getPart(PartCreatureType part_type, PartCreatureField part_field){
    for (auto& part : creature_parts){
        if (part.part_type == part_type){
            if (PartCreatureField::STATUS == part_field) return part.part_status;
            else if (PartCreatureField::DENSITY == part_field) return part.part_densityDef;
            else if (PartCreatureField::PENETRATION == part_field) return part.part_penetrationDef;
            else if (PartCreatureField::CRUSHING == part_field) return part.part_crushingDef;

        }
    }
}
PartOrgan& Creature::getOrgan(PartCreatureType part_type,PartOrganType part_organ_type){
    for (auto& part : creature_parts){
        if (part.part_type == part_type){
            for (auto& organ : part.part_organs){
                if (organ.type == part_organ_type)
                    return organ;
            }
        }
    }
}
void Creature::getStatistics(){
    if (!isStatisticsShowing){
        isStatisticsShowing = true;
        creature_statistics = cocos2d::Label::createWithTTF("","fonts/arial.ttf",18,cocos2d::Size::ZERO);
        creature_statistics->setPosition(creature_sprite->getPosition());
        currentLayer->addChild(creature_statistics,ZLevel::USER_INTERFACE);
    }
    else{
        isStatisticsShowing = false;
        currentLayer->removeChild(creature_statistics);
    }
}

void Creature::setOrgan(PartCreatureType part_type, PartOrganType part_organ_type,PartCreatureStatus status){
    for (auto& part : creature_parts){
        if (part.part_type == part_type){
            for (auto& organ : part.part_organs){
                if (organ.type == part_organ_type)
                    organ.status = status;
            }
        }
    }
}
void Creature::setCreatureState(CreatureInfo::State creature_state){
    this->creature_info.state  = creature_state;
    isNewState = true;
}

void Creature::setStatistics(DebugStatistics mode){
    std::string partStatus;
    /*Set strings about part of body*/
    partStatus.append("Status:\n");
    if (mode == DebugStatistics::GAME_STATS){
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
                case PartCreatureType::HAND_LEFT:{
                    partStatus.append("\thand left:");
                    break;
                }
                case PartCreatureType::HAND_RIGHT:{
                    partStatus.append("\thand right:");
                    break;
                }
                case PartCreatureType::BUTTOM_TORSE:{
                    partStatus.append("\tbotTorse:");
                    break;
                }
                case PartCreatureType::LEG_LEFT:{
                    partStatus.append("\tleg left:");
                    break;
                }
                case PartCreatureType::LEG_RIGHT:{
                    partStatus.append("\tleg right:");
                    break;
                }
            }
            partStatus.append(part.part_status == PartCreatureStatus::NORMAL ? "norm-" :
                              part.part_status == PartCreatureStatus::WONDED ? "wonded-" :
                              part.part_status == PartCreatureStatus::CUTTED ? "cutted-" :
                              "killed");
            partStatus.append(std::to_string(part.part_densityDef) + "-" + std::to_string(part.part_penetrationDef) + "-" + std::to_string(part.part_crushingDef) + "\n");

        }
        partStatus.append("brain:");
        partStatus.append(getOrgan(PartCreatureType::HEAD,PartOrganType::BRAIN).status == PartCreatureStatus::NORMAL ? "norm\n" :
                          getOrgan(PartCreatureType::HEAD,PartOrganType::BRAIN).status == PartCreatureStatus::WONDED ? "wonded\n" :
                          getOrgan(PartCreatureType::HEAD,PartOrganType::BRAIN).status == PartCreatureStatus::CUTTED ? "cutted\n" :
                          "killed\n");
        partStatus.append("lungs:");
        partStatus.append(getOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::LUNGS).status == PartCreatureStatus::NORMAL ? "norm\n" :
                          getOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::LUNGS).status == PartCreatureStatus::WONDED ? "wonded\n" :
                          getOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::LUNGS).status == PartCreatureStatus::CUTTED ? "cutted\n" :
                          "killed\n");
        partStatus.append("heart:");
        partStatus.append(getOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::HEART).status == PartCreatureStatus::NORMAL ? "norm\n" :
                          getOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::HEART).status == PartCreatureStatus::WONDED ? "wonded\n" :
                          getOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::HEART).status == PartCreatureStatus::CUTTED ? "cutted\n" :
                          "killed\n");
        partStatus.append("gut:");
        partStatus.append(getOrgan(PartCreatureType::BUTTOM_TORSE,PartOrganType::GUT).status == PartCreatureStatus::NORMAL ? "norm\n" :
                          getOrgan(PartCreatureType::BUTTOM_TORSE,PartOrganType::GUT).status == PartCreatureStatus::WONDED ? "wonded\n" :
                          getOrgan(PartCreatureType::BUTTOM_TORSE,PartOrganType::GUT).status == PartCreatureStatus::CUTTED ? "cutted\n" :
                          "killed\n");
        /*Set strings about body*/
        partStatus.append("blood:" + std::to_string(creature_info.characteristic.blood) + "l\n");    
        partStatus.append("stamina:" + std::to_string(creature_info.characteristic.stamina) + "\n");
    }
    else if (mode == DebugStatistics::PHYSICS){
        partStatus.append("Position x = ");
        partStatus.append(std::to_string(creature_sprite->getPosition().x) + "\ty= " + std::to_string(creature_sprite->getPosition().y) + "\n");
        partStatus.append("Velocity x = ");
        partStatus.append(std::to_string(creature_physic_body->getVelocity().x) + "\ty= " + std::to_string(creature_physic_body->getVelocity().y) + "\n");
        partStatus.append("State==");
        partStatus.append(creature_info.state == CreatureInfo::State::IDLE ? "IDLE\n" :
                          creature_info.state == CreatureInfo::State::START_RUN ? "START_RUN\n" :
                          creature_info.state == CreatureInfo::State::RUNNING ? "RUN\n" :
                          creature_info.state == CreatureInfo::State::BRACKING ? "BRACKING\n" :
                          creature_info.state == CreatureInfo::State::STAND_UP ? "STAND_UP\n" :
                          creature_info.state == CreatureInfo::State::IN_JUMP ? "IN_JUMP\n" :
                          creature_info.state == CreatureInfo::State::IN_FALL ? "IN_FALL\n" :
                          creature_info.state == CreatureInfo::State::INTERACTING ? "INTERACTING\n" :
                          creature_info.state == CreatureInfo::State::ON_EDGE ? "ON_EDGE\n" :
                          creature_info.state == CreatureInfo::State::ON_STEPS ? "ON_STEPS\n" :
                          creature_info.state == CreatureInfo::State::ON_WALL ? "ON_WALL\n" :
                          creature_info.state == CreatureInfo::State::GRAB_ON ? "GRAB_ON\n" :
                          creature_info.state == CreatureInfo::State::LAND_ON ? "LAND_ON\n" :
                          "UNDEFIND\n");    
    
    }
    
    creature_statistics->setString(partStatus);
}
void Creature::setWeapon(WeaponType wMap ){
    switch (wMap){
    case WeaponType::SWORD:{
        creature_weapon = new Sword("swordStock.png",creature_sprite,this);
        break;
    }
    case WeaponType::AXE:{
        creature_weapon = new Axe("axeStock.png",creature_sprite,this);
        break;
    }
    case WeaponType::SPEAR:{
        creature_weapon = new Spear("spearStock.png",creature_sprite,this);
        break;
    }
    }
    creature_weapon->getSprite()->setPosition(creature_sprite->getPosition());
    creature_weapon->getDammageSprite()->setPosition(creature_weapon->getSprite()->getPosition());
    currentLayer->addChild(creature_weapon->getSprite(),ZLevel::MIDLEGROUND);
    currentLayer->addChild(creature_weapon->getDammageSprite(),ZLevel::MIDLEGROUND);
}
void Creature::showStatistics(DebugStatistics type){
    /*For statistics*/
    if (isStatisticsShowing){
        setStatistics(type);
        creature_statistics->runAction(cocos2d::MoveTo::create(0.2,cocos2d::Vec2(creature_sprite->getPosition().x + creature_statistics->getBoundingBox().size.width/2,
                                                                                 creature_sprite->getPosition().y + creature_statistics->getBoundingBox().size.height/2)));
    }
}
void Creature::losingStamina(){
    if ((creature_physic_body->getVelocity().x > 100 || creature_physic_body->getVelocity().x < -100) &&
        creature_physic_body->getVelocity().y == 0){
        //creature_info.characteristic.stamina--;
    }
}
void Creature::regeneratingStamina(float dt){
    //For regeneration of stamina
    creature_info.characteristic.stamina_regeneration_counter += dt;
    //How fast regeneration will be applied
    if (creature_info.characteristic.stamina < creature_info.characteristic.stamina_limit && creature_info.characteristic.stamina_regeneration_counter >= 1){
        creature_info.characteristic.stamina++;
        creature_info.characteristic.stamina_regeneration_counter = 0;
    }
    //Just be sure if stamina will below 0 will not negative
    if (creature_info.characteristic.stamina <= 0)
        creature_info.characteristic.stamina = 0;
}
void Creature::updatePermament(){
    creature_weapon->update();
    if (creature_physic_body->getVelocity().y < -5 && creature_info.state != CreatureInfo::State::IN_FALL){
        setCreatureState(CreatureInfo::State::IN_FALL);
    }
}
void Creature::updateCurrentState(){
    switch (creature_info.state){
    case CreatureInfo::State::IDLE:{
        if (creature_sprite->getNumberOfRunningActions() == 0){
            creature_sprite->runAction(cocos2d::RepeatForever::create(animation_idle));
            isNewState = false;
        }
        break;
    }
    case CreatureInfo::State::START_RUN:{
        creature_sprite->stopAllActions();
        creature_sprite->runAction(animation_speedUp);
        setCreatureState(CreatureInfo::State::RUNNING);
        break;
    }
    case CreatureInfo::State::RUNNING:{
        if (creature_sprite->getNumberOfRunningActions() == 0){
            creature_sprite->stopAllActions();
            creature_sprite->runAction(cocos2d::RepeatForever::create(animation_run));
        }
        
        bool isFlipped;
        cocos2d::Vec2 newVelocity;
        if (creature_info.dmove == CreatureInfo::DMove::LEFT){
            isFlipped = true;
            creature_sprite->setFlippedX(true);
        }
        else if (creature_info.dmove == CreatureInfo::DMove::RIGHT){
            isFlipped = false;
            creature_sprite->setFlippedX(false);
        }
        creature_weapon->getSprite()->setFlippedX(isFlipped);
        newVelocity = cocos2d::Vec2(creature_info.characteristic.acceleration_power * creature_info.dmove, 0);
        creature_physic_body->setVelocity(cocos2d::Vec2(creature_physic_body->getVelocity().x + newVelocity.x,
                                                        creature_physic_body->getVelocity().y));
        
        break;
    }
    case CreatureInfo::State::STAND_UP:{
        if (creature_physic_body->getVelocity().x >= -5 && creature_physic_body->getVelocity().x <= 5){
            creature_sprite->stopAllActions();
            creature_sprite->runAction(animation_slowdown);
            setCreatureState(CreatureInfo::State::IDLE);
        }
        break;
    }
    case CreatureInfo::State::BRACKING:{
        creature_sprite->stopAllActions();
        creature_sprite->runAction(cocos2d::RepeatForever::create(animation_braking));
        setCreatureState(CreatureInfo::State::STAND_UP);
        break;
    }
    case CreatureInfo::State::GRAB_ON:{
        creature_sprite->stopAllActions();
        isNewState = false;
        break;
    }
    case CreatureInfo::State::IN_JUMP:{
        creature_sprite->stopAllActions();
        creature_info.characteristic.current_jump_ability_num++;
        //Lose some stamina
        creature_info.characteristic.stamina = creature_info.characteristic.stamina - 2;
        cocos2d::Vec2 newVelocity = cocos2d::Vec2(0,creature_info.characteristic.jump_power);
        /*Set vertical velocity once for the body*/
        creature_physic_body->setVelocity(cocos2d::Vec2(creature_physic_body->getVelocity().x,
                                                            newVelocity.y));
        isNewState = false;
        break;
    }
    case CreatureInfo::State::IN_FALL:{
        creature_sprite->stopAllActions();
        isNewState = false;
        break;
    }
    case CreatureInfo::State::LAND_ON:{
        creature_sprite->stopAllActions();
        creature_info.characteristic.current_jump_ability_num = 0;
        creature_physic_body->setVelocity(cocos2d::Vec2(0,creature_physic_body->getVelocity().y));
        setCreatureState(CreatureInfo::State::IDLE);
        break;
    }
    case CreatureInfo::State::ON_EDGE:{
        creature_sprite->stopAllActions();
        isNewState = false;
        break;
    }
    case CreatureInfo::State::ON_STEPS:{
        creature_sprite->stopAllActions();
        isNewState = false;
        break;
    }
    case CreatureInfo::State::ON_WALL:{
        creature_sprite->stopAllActions();
        creature_info.characteristic.current_jump_ability_num = 0;
        creature_physic_body->setVelocity(cocos2d::Vec2(0,0));
        isNewState = false;
        break;
    }
    case CreatureInfo::State::INTERACTING:{
        creature_sprite->stopAllActions();
        bool isIntersection = false;
        for (const auto& lI : WorldProperties::levelItems){
            //Will open door if creature neer by, it's a door and buttom E pressed
            if (creature_sprite->getBoundingBox().intersectsRect(lI.second) && lI.first == "door"){
                isIntersection = true;
            }
            //Will climbin on stair if creature neer by, it's a stair and buttom E pressed
            else if (creature_sprite->getBoundingBox().intersectsRect(lI.second) && lI.first == "stair"){
                isIntersection = true;
            }
        
        }
        //If we not closed to item or buttom not pressed we will reser buttom state
        if (!isIntersection){
            setCreatureState(CreatureInfo::State::IDLE);
        }
        isNewState = false;
        break;
    }
    }
}
///////////////////////////////////////////////////////*PartCreature class*///////////////////////////////////////////////////////
PartOrgan::PartOrgan(PartOrganType type){
    this->type = type;
    this->status = PartCreatureStatus::NORMAL;
}
Creature::PartCreature::PartCreature(PartCreatureType part_type){
    this->part_status      = PartCreatureStatus::NORMAL;
    this->part_type        = part_type;
    this->part_penetrationDef = 1;
    this->part_crushingDef    = 5;
    switch (part_type){
    case PartCreatureType::HEAD:{
        this->part_densityDef = 10;
        this->part_organs.push_back(PartOrgan(PartOrganType::BRAIN));
        break;
    }
    case PartCreatureType::UPPER_TORSE:{
        this->part_densityDef = 30;
        this->part_organs.push_back(PartOrgan(PartOrganType::LUNGS));
        this->part_organs.push_back(PartOrgan(PartOrganType::HEART));     
        break;
    }
    case PartCreatureType::HAND_LEFT:{
        this->part_densityDef = 10;
        this->part_organs.push_back(PartOrgan(PartOrganType::NONE));
        break;
    }
    case PartCreatureType::HAND_RIGHT:{
        this->part_densityDef = 10;
        this->part_organs.push_back(PartOrgan(PartOrganType::NONE));
        break;
    }
    case PartCreatureType::BUTTOM_TORSE:{
        this->part_densityDef = 20;
        this->part_organs.push_back(PartOrgan(PartOrganType::GUT));
        break;
    }
    case PartCreatureType::LEG_LEFT:{
        this->part_densityDef = 15;
        this->part_organs.push_back(PartOrgan(PartOrganType::NONE));
        break;
    }
    case PartCreatureType::LEG_RIGHT:{
        this->part_densityDef = 15;
        this->part_organs.push_back(PartOrgan(PartOrganType::NONE));
        break;
    }
    }
}
CreatureInfo::CreatureInfo(){}
CreatureInfo::CreatureInfo(Type type,CreatureInfo::Animation animation){
    this->type = type;
    this->animation.animationForWho = animation.animationForWho;
    for (uint i = 0; i < animation.framesIdleNum.size(); ++i)
        this->animation.framesIdleNum.push_back(animation.framesIdleNum[i]);
}
CreatureInfo::Animation::Animation(){}
CreatureInfo::Animation::Animation(std::vector<uint> framesIdleNum,std::string animationForWho){
    for (uint i = 0; i < framesIdleNum.size(); ++i){
        this->framesIdleNum.push_back(framesIdleNum[i]);
    }
    this->animationForWho = animationForWho;
}

///////////////////////////////////////////////////////*Enemy class*///////////////////////////////////////////////////////
Enemy::Enemy(CreatureInfo info,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id) :
    Creature(info,pos,gameLayer,id){
    
}
void Enemy::update(float dt){
    showStatistics(DebugStatistics::PHYSICS);
    if (isNewState){
        updateCurrentState();
    }
    updatePermament();
}

///////////////////////////////////////////////////////*Player class*///////////////////////////////////////////////////////
Player::Player(CreatureInfo info,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id) :
    Creature(info,pos,gameLayer,id){
    //enemyNode = static_cast<GameLayer*>(gameLayer)->getEnemy();
    currentInteractedEnemy = -1;
    creature_info.characteristic.stamina_regeneration_counter = 0;
}
void Player::update(float dt){
    showStatistics(DebugStatistics::PHYSICS);
    if (isNewState){
        updateCurrentState();
    }
    updatePermament();
}


/**Some code for future
 **First clean engine's calls
 *enemyNode->at(currentInteractedEnemy)->removeSprite();
 *enemyNode->at(currentInteractedEnemy)->removeStatistics();
 **Second clean game's  call
 *enemyNode->erase(enemyNode->begin()+currentInteractedEnemy);
*/