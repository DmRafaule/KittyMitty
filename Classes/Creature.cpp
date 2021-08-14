#include "Creature.h"
#include "GameLayer.h"
#include <dirent.h>

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


///////////////////////////////////////////////////////*Creature class*///////////////////////////////////////////////////////
Creature::Creature(CreatureInfo::Type type, cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id){
    this->currentLayer = gameLayer;
    this->isStatisticsShowing = false;
    this->isNewState = false;
    this->indentificator = id;
    this->creature_info.type = type;
    this->creature_info.state = CreatureInfo::State::IN_FALL;

    initStats();
    initAnimations();
    initBody(pos);
}
void Creature::initStats(){ 
    switch(creature_info.type){
        case CreatureInfo::Type::KITTYMITTY:{
            this->creature_info.animation.animationForWho = "hero";
            this->creature_info.animation.framesIdleNum   = std::vector<uint>({15,4,7,4,2,7,2,5,5,2,4,4,5,5});

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
            creature_info.characteristic.jump_ability = 1;
            creature_info.characteristic.current_jump_ability_num = 0;
            creature_info.characteristic.mass = 10;
            break;
        }
        case CreatureInfo::Type::KOOL_HASH:{
            this->creature_info.animation.animationForWho = "kool-hash";
            this->creature_info.animation.framesIdleNum   = std::vector<uint>({8,3,5,3,4,9,4,6,5,4,4,4,8,5});

            creature_parts.push_back(PartCreature(PartCreatureType::HEAD));
            creature_parts.push_back(PartCreature(PartCreatureType::UPPER_TORSE));
            creature_parts.push_back(PartCreature(PartCreatureType::HAND_LEFT));
            creature_parts.push_back(PartCreature(PartCreatureType::HAND_RIGHT));
            creature_parts.push_back(PartCreature(PartCreatureType::BUTTOM_TORSE));
            creature_parts.push_back(PartCreature(PartCreatureType::LEG_LEFT));
            creature_parts.push_back(PartCreature(PartCreatureType::LEG_RIGHT));
            creature_info.characteristic.velocity_limit  = 180;
            creature_info.characteristic.jump_power = 120;
            creature_info.characteristic.acceleration_power = 55;
            creature_info.characteristic.stamina = 80;
            creature_info.characteristic.stamina_limit = 80;
            creature_info.characteristic.blood   = 20;
            creature_info.characteristic.jump_ability = 1;
            creature_info.characteristic.current_jump_ability_num = 0;
            creature_info.characteristic.mass = 15;
            break;
        }
        case CreatureInfo::Type::ERENU_DOO:{
            this->creature_info.animation.animationForWho = "erenu-doo";
            this->creature_info.animation.framesIdleNum   = std::vector<uint>({9,4,5,4,8,0,4,10,0,4,3,0,5,5});

            creature_parts.push_back(PartCreature(PartCreatureType::HEAD));
            creature_parts.push_back(PartCreature(PartCreatureType::UPPER_TORSE));
            creature_parts.push_back(PartCreature(PartCreatureType::HAND_LEFT));
            creature_parts.push_back(PartCreature(PartCreatureType::HAND_RIGHT));
            creature_parts.push_back(PartCreature(PartCreatureType::BUTTOM_TORSE));
            creature_parts.push_back(PartCreature(PartCreatureType::LEG_LEFT));
            creature_parts.push_back(PartCreature(PartCreatureType::LEG_RIGHT));
            creature_info.characteristic.velocity_limit  = 50;
            creature_info.characteristic.jump_power = 0;
            creature_info.characteristic.acceleration_power = 15;
            creature_info.characteristic.stamina = 300;
            creature_info.characteristic.stamina_limit = 300;
            creature_info.characteristic.blood   = 10;
            creature_info.characteristic.jump_ability = 0;
            creature_info.characteristic.current_jump_ability_num = 0;
            creature_info.characteristic.mass = 25;
            break;
        }
        case CreatureInfo::Type::GOO_ZOO:{
            this->creature_info.animation.animationForWho = "goo-zoo";
            this->creature_info.animation.framesIdleNum   = std::vector<uint>({7,5,4,5,4,7,3,7,0,2,4,0,6,5});

            creature_parts.push_back(PartCreature(PartCreatureType::HEAD));
            creature_parts.push_back(PartCreature(PartCreatureType::UPPER_TORSE));
            creature_parts.push_back(PartCreature(PartCreatureType::HAND_LEFT));
            creature_parts.push_back(PartCreature(PartCreatureType::HAND_RIGHT));
            creature_parts.push_back(PartCreature(PartCreatureType::BUTTOM_TORSE));
            creature_parts.push_back(PartCreature(PartCreatureType::LEG_LEFT));
            creature_parts.push_back(PartCreature(PartCreatureType::LEG_RIGHT));
            creature_info.characteristic.velocity_limit  = 120;
            creature_info.characteristic.jump_power = 80;
            creature_info.characteristic.acceleration_power = 15;
            creature_info.characteristic.stamina = 200;
            creature_info.characteristic.stamina_limit = 200;
            creature_info.characteristic.blood   = 60;
            creature_info.characteristic.jump_ability = 0;
            creature_info.characteristic.current_jump_ability_num = 0;
            creature_info.characteristic.mass = 50;
            break;
        }
        case CreatureInfo::Type::AVR:{
            this->creature_info.animation.animationForWho = "avr";
            this->creature_info.animation.framesIdleNum   = std::vector<uint>({12,3,4,3,7,8,8,6,5,2,4,4,6,5});

            creature_parts.push_back(PartCreature(PartCreatureType::HEAD));
            creature_parts.push_back(PartCreature(PartCreatureType::UPPER_TORSE));
            creature_parts.push_back(PartCreature(PartCreatureType::HAND_LEFT));
            creature_parts.push_back(PartCreature(PartCreatureType::HAND_RIGHT));
            creature_parts.push_back(PartCreature(PartCreatureType::BUTTOM_TORSE));
            creature_parts.push_back(PartCreature(PartCreatureType::LEG_LEFT));
            creature_parts.push_back(PartCreature(PartCreatureType::LEG_RIGHT));
            creature_info.characteristic.velocity_limit  = 300;
            creature_info.characteristic.jump_power = 180;
            creature_info.characteristic.acceleration_power = 45;
            creature_info.characteristic.stamina = 500;
            creature_info.characteristic.stamina_limit = 500;
            creature_info.characteristic.blood   = 30;
            creature_info.characteristic.jump_ability = 2;
            creature_info.characteristic.current_jump_ability_num = 0;
            creature_info.characteristic.mass = 10;
            break;
        }
    }
}
void Creature::initAnimations(){

    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("textures/animations/" + creature_info.animation.animationForWho + "/animationSheet.plist");
    currentLayer->addChild(cocos2d::SpriteBatchNode::create("textures/animations/" + creature_info.animation.animationForWho + "/animationSheet.png"));
    
    addAnimation("_animation_idle",creature_info.animation.framesIdleNum[0],0.35,true);
    addAnimation("_animation_startrun",creature_info.animation.framesIdleNum[1],0.1,false);
    addAnimation("_animation_run",creature_info.animation.framesIdleNum[2],0.15,false);
    addAnimation("_animation_standup",creature_info.animation.framesIdleNum[3],0.15,false);
    addAnimation("_animation_braking",creature_info.animation.framesIdleNum[4],0.2,false);
    addAnimation("_animation_injump",creature_info.animation.framesIdleNum[5],0.03,false);
    addAnimation("_animation_infall",creature_info.animation.framesIdleNum[6],0.2,false);
    addAnimation("_animation_landon",creature_info.animation.framesIdleNum[7],0.1,false);
    addAnimation("_animation_onwall",creature_info.animation.framesIdleNum[8],0.15,false);
    addAnimation("_animation_soaring",creature_info.animation.framesIdleNum[9],0.2,false);
    addAnimation("_animation_stepsrun",creature_info.animation.framesIdleNum[10],0.2,false);
    addAnimation("_animation_jumpfromwall",creature_info.animation.framesIdleNum[11],0.1,false);
    addAnimation("_animation_attack",creature_info.animation.framesIdleNum[12],0.07,false);
    addAnimation("_animation_getdammage",creature_info.animation.framesIdleNum[13],0.1,false);

    
}
void Creature::addAnimation(std::string anim_name,uint frame_number,float delay,bool restoreOrigFr){
    auto animation = cocos2d::Animation::create();
    for (uint i = 0; i < frame_number;++i){
        std::string name = creature_info.animation.animationForWho + anim_name + std::to_string(i) + ".png";
        auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
        animation->addSpriteFrame(frame);
        
    }
    animation->setDelayPerUnit(delay);
    animation->setRestoreOriginalFrame(restoreOrigFr);
    animations.emplace(anim_name,cocos2d::Animate::create(animation));
    animations.find(anim_name)->second->retain();

}
void Creature::initBody(cocos2d::Vec2 pos){
    creature_sprite = cocos2d::Sprite::createWithSpriteFrameName(creature_info.animation.animationForWho + "_animation_idle0.png");
    creature_physic_body = cocos2d::PhysicsBody::createEdgeBox(cocos2d::Size(creature_sprite->getBoundingBox().size.width/2,creature_sprite->getBoundingBox().size.height),cocos2d::PhysicsMaterial(0,0,1.5));
    creature_physic_body->setMass(creature_info.characteristic.mass);
    creature_physic_body->setDynamic(true);
    creature_physic_body->setVelocityLimit(creature_info.characteristic.velocity_limit);
    creature_physic_body->setGravityEnable(true);
    creature_physic_body->setCollisionBitmask(indentificator);
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
    currentLayer->addChild(creature_sprite,ZLevel::MIDLEGROUND,indentificator);
}
Creature::~Creature(){
    
}
void Creature::removeCreature(){
    creature_parts.clear();
    currentLayer->removeChild(creature_sprite);
    currentLayer->removeChild(creature_weapon->getSprite());

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
                          creature_info.state == CreatureInfo::State::ATTACK ? "ATTACK\n" :
                          creature_info.state == CreatureInfo::State::GET_DAMMAGE ? "GET_DAMMAGE\n" :
                          creature_info.state == CreatureInfo::State::START_RUN ? "START_RUN\n" :
                          creature_info.state == CreatureInfo::State::RUNNING ? "RUN\n" :
                          creature_info.state == CreatureInfo::State::BRACKING ? "BRACKING\n" :
                          creature_info.state == CreatureInfo::State::STAND_UP ? "STAND_UP\n" :
                          creature_info.state == CreatureInfo::State::IN_JUMP ? "IN_JUMP\n" :
                          creature_info.state == CreatureInfo::State::JUMP_FROM_WALL ? "JUMP_FROM_WALL\n" :
                          creature_info.state == CreatureInfo::State::IN_FALL ? "IN_FALL\n" :
                          creature_info.state == CreatureInfo::State::INTERACTING ? "INTERACTING\n" :
                          creature_info.state == CreatureInfo::State::ON_STEPS ? "ON_STEPS\n" :
                          creature_info.state == CreatureInfo::State::ON_WALL ? "ON_WALL\n" :
                          creature_info.state == CreatureInfo::State::TAKE_ROOF ? "TAKE_ROOF\n" :
                          creature_info.state == CreatureInfo::State::LAND_ON ? "LAND_ON\n" :
                          creature_info.state == CreatureInfo::State::LETGO ? "LETGO\n" :
                          creature_info.state == CreatureInfo::State::MOVE_BY_STEPS ? "MOVE_BY_STEPS\n" :
                          creature_info.state == CreatureInfo::State::SOARING ? "SOARING\n" :
                          "UNDEFIND\n");    
    
    }
    
    creature_statistics->setString(partStatus);
}
void Creature::showStatistics(DebugStatistics type){
    /*For statistics*/
    if (isStatisticsShowing){
        setStatistics(type);
        creature_statistics->runAction(cocos2d::MoveTo::create(0.2,cocos2d::Vec2(creature_sprite->getPosition().x + creature_statistics->getBoundingBox().size.width/2,
                                                                                 creature_sprite->getPosition().y + creature_statistics->getBoundingBox().size.height/2)));
    }
}

void Creature::setCreatureState(CreatureInfo::State creature_state){
    this->creature_info.state  = creature_state;
    isNewState = true;
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
    if (creature_physic_body->getVelocity().y < -5 && creature_info.state != CreatureInfo::State::IN_FALL &&
        creature_info.state != CreatureInfo::State::ON_STEPS){
        setCreatureState(CreatureInfo::State::IN_FALL);
    }
    if (creature_info.state == CreatureInfo::State::IN_FALL || creature_info.state == CreatureInfo::State::IN_JUMP){
        if (creature_physic_body->getVelocity().x > 0)
            creature_info.dmove = CreatureInfo::DMove::RIGHT;
        else if (creature_physic_body->getVelocity().x < 0)
            creature_info.dmove = CreatureInfo::DMove::LEFT;
    }
}
void Creature::updateCurrentState(){
    switch (creature_info.state){
    case CreatureInfo::State::ATTACK:{
        creature_sprite->stopAllActions();
        creature_sprite->runAction(animations.find("_animation_attack")->second);
        setCreatureState(CreatureInfo::State::IDLE);
        break;
    }
    case CreatureInfo::State::GET_DAMMAGE:{
        creature_sprite->stopAllActions();
        creature_sprite->runAction(animations.find("_animation_getdammage")->second);
        if (creature_info.dmove == CreatureInfo::DMove::RIGHT)
            creature_physic_body->setVelocity(cocos2d::Vec2(-50,50));
        else if (creature_info.dmove == CreatureInfo::DMove::LEFT)
            creature_physic_body->setVelocity(cocos2d::Vec2(50,50));
        else if (creature_info.dmove == CreatureInfo::DMove::TOP)
            creature_physic_body->setVelocity(cocos2d::Vec2(0,-50));
        else if (creature_info.dmove == CreatureInfo::DMove::DOWN)
            creature_physic_body->setVelocity(cocos2d::Vec2(0,50));
        setCreatureState(CreatureInfo::State::IDLE);
        break;
    }
    case CreatureInfo::State::IDLE:{
        if (creature_sprite->getNumberOfRunningActions() == 0){
            creature_sprite->stopAllActions();
            creature_sprite->runAction(cocos2d::RepeatForever::create(animations.find("_animation_idle")->second));
            isNewState = false;
        }
        break;
    }
    case CreatureInfo::State::START_RUN:{
        creature_sprite->stopAllActions();
        creature_sprite->runAction(animations.find("_animation_startrun")->second);
        setCreatureState(CreatureInfo::State::RUNNING);
        break;
    }
    case CreatureInfo::State::RUNNING:{
        if (creature_sprite->getNumberOfRunningActions() == 0){
            creature_sprite->stopAllActions();
            creature_sprite->runAction(cocos2d::RepeatForever::create(animations.find("_animation_run")->second));
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
            creature_sprite->runAction(animations.find("_animation_standup")->second);
            setCreatureState(CreatureInfo::State::IDLE);
        }
        break;
    }
    case CreatureInfo::State::BRACKING:{
        creature_sprite->stopAllActions();
        creature_sprite->runAction(cocos2d::RepeatForever::create(animations.find("_animation_braking")->second));
        setCreatureState(CreatureInfo::State::STAND_UP);
        break;
    }
    case CreatureInfo::State::IN_JUMP:{
        creature_sprite->stopAllActions();
        creature_sprite->runAction(animations.find("_animation_injump")->second);
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
    case CreatureInfo::State::JUMP_FROM_WALL:{
        if (creature_info.dmove == CreatureInfo::DMove::LEFT)
            creature_physic_body->setVelocity(cocos2d::Vec2(100,100));
        else if (creature_info.dmove == CreatureInfo::DMove::RIGHT)
            creature_physic_body->setVelocity(cocos2d::Vec2(-100,100));
        creature_sprite->stopAllActions();
        creature_sprite->runAction(animations.find("_animation_jumpfromwall")->second);
        isNewState = false;
        break;
    }
    case CreatureInfo::State::IN_FALL:{
        creature_sprite->stopAllActions();
        if (creature_sprite->getNumberOfRunningActions() == 0){
            creature_sprite->stopAllActions();
            creature_sprite->runAction(cocos2d::RepeatForever::create(animations.find("_animation_infall")->second));
        }
        isNewState = false;
        break;
    }
    case CreatureInfo::State::SOARING:{
        creature_sprite->stopAllActions();
        creature_sprite->runAction(animations.find("_animation_soaring")->second);
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
    case CreatureInfo::State::LAND_ON:{
        creature_sprite->stopAllActions();
        creature_sprite->runAction(animations.find("_animation_landon")->second);
        creature_info.characteristic.current_jump_ability_num = 0;
        creature_physic_body->setVelocity(cocos2d::Vec2(0,creature_physic_body->getVelocity().y));
        setCreatureState(CreatureInfo::State::IDLE);
        break;
    }
    case CreatureInfo::State::ON_STEPS:{
        creature_sprite->stopAllActions();
        creature_sprite->runAction(cocos2d::RepeatForever::create(animations.find("_animation_idle")->second));
        creature_physic_body->setVelocity(cocos2d::Vec2(0,0));
        creature_info.characteristic.current_jump_ability_num = 0;
        isNewState = false;
        break;
    }
    case CreatureInfo::State::MOVE_BY_STEPS:{
        if (creature_sprite->getNumberOfRunningActions() == 1){
            creature_sprite->stopAllActions();
            creature_sprite->runAction(cocos2d::RepeatForever::create(animations.find("_animation_stepsrun")->second));
            creature_sprite->runAction(cocos2d::RepeatForever::create(cocos2d::RotateBy::create(0,0)));
        }
        if (creature_physic_body->getVelocity().x >= 45 || creature_physic_body->getVelocity().x <= -45){
            setCreatureState(CreatureInfo::State::IN_FALL);
        }
        creature_physic_body->setVelocity(cocos2d::Vec2(creature_info.characteristic.acceleration_power * creature_info.dmove,20));
        break;
    }
    case CreatureInfo::State::ON_WALL:{
        creature_sprite->stopAllActions();
        creature_sprite->runAction(animations.find("_animation_onwall")->second);
        if (creature_info.dmove == CreatureInfo::DMove::LEFT){
            creature_sprite->setFlippedX(false);
        }
        else if (creature_info.dmove == CreatureInfo::DMove::RIGHT){
            creature_sprite->setFlippedX(true);
        }
        creature_info.characteristic.current_jump_ability_num = 0;
        creature_physic_body->setVelocity(cocos2d::Vec2(0,0));
        
        //setCreatureState(CreatureInfo::State::IDLE);
        break;
    }
    case CreatureInfo::State::LETGO:{
        creature_sprite->stopAllActions();
        if (creature_info.dmove == CreatureInfo::DMove::LEFT)
            creature_physic_body->setVelocity(cocos2d::Vec2(25,0));
        else
            creature_physic_body->setVelocity(cocos2d::Vec2(-25,0));
        creature_info.characteristic.current_jump_ability_num = 3;
        setCreatureState(CreatureInfo::State::IN_FALL);
        break;
    }
    case CreatureInfo::State::TAKE_ROOF:{
        creature_sprite->stopAllActions();
        creature_info.characteristic.current_jump_ability_num = 3;
        creature_physic_body->setVelocity(cocos2d::Vec2(0,creature_physic_body->getVelocity().y));
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

///////////////////////////////////////////////////////*Enemy class*///////////////////////////////////////////////////////
Enemy::Enemy(CreatureInfo::Type type,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id) :
    Creature(type,pos,gameLayer,id){
    
}
void Enemy::update(float dt){
    showStatistics(DebugStatistics::PHYSICS);
    if (isNewState){
        updateCurrentState();
    }
    updatePermament();
}

///////////////////////////////////////////////////////*Player class*///////////////////////////////////////////////////////
Player::Player(CreatureInfo::Type type,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id) :
    Creature(type,pos,gameLayer,id){
    //enemyNode = static_cast<GameLayer*>(gameLayer)->getEnemy();
    currentInteractedEnemy = -1;
    creature_info.characteristic.stamina_regeneration_counter = 0;
}
void Player::setPlayerPosition(float x, float y){
    creature_sprite->setPosition(x,y);
    creature_weapon->getSprite()->setPosition(x,y);
    if (isStatisticsShowing)
        creature_statistics->setPosition(x,y);
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
 *enemyNode->at(currentInteractedEnemy)->removeCreature();
 **Second clean game's  call
 *enemyNode->erase(enemyNode->begin()+currentInteractedEnemy);
*/