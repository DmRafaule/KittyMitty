#include "Creature.h"
#include "GameLayer.h"
#include <dirent.h>

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
CreatureInfo::Part::Part(){}
CreatureInfo::Part::Part(PartCreatureType type, PartCreatureStatus status, uint integrality) : 
    type(type), status(status), integrality(integrality), maxIntegrality(integrality){

}

///////////////////////////////////////////////////////*Creature class*///////////////////////////////////////////////////////
bool Creature::isInInteraction = false;
Creature::Creature(){
}
Creature::Creature(CreatureInfo::Type type, cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id){
    this->currentLayer = gameLayer;
    this->isStatisticsShowing = false;
    this->isNewState  = false;
    this->isWeaponSet = false;
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
            this->creature_info.animation.framesIdleNum   = std::vector<uint>({15,4,7,4,2,7,2,5,5,2,4,4,5,5,4,8});
            creature_part.emplace(PartCreatureType::TOP,(new CreatureInfo::Part(PartCreatureType::TOP,PartCreatureStatus::NORMAL,2)));
            creature_part.emplace(PartCreatureType::MIDDLE,(new CreatureInfo::Part(PartCreatureType::MIDDLE,PartCreatureStatus::NORMAL,3)));
            creature_part.emplace(PartCreatureType::BOTTOM,(new CreatureInfo::Part(PartCreatureType::BOTTOM,PartCreatureStatus::NORMAL,2)));
            creature_info.characteristic.velocity_limit  = 200;
            creature_info.characteristic.jump_power = 120;
            creature_info.characteristic.acceleration_power = 45;
            creature_info.characteristic.stamina = 100;
            creature_info.characteristic.stamina_limit = 100;
            creature_info.characteristic.blood   = 20;
            creature_info.characteristic.jump_ability = 1;
            creature_info.characteristic.current_jump_ability_num = 0;
            creature_info.characteristic.mass = 10;
            creature_info.characteristic.vision_radius = 300;
            break;
        }
        case CreatureInfo::Type::KOOL_HASH:{
            this->creature_info.animation.animationForWho = "kool-hash";
            this->creature_info.animation.framesIdleNum   = std::vector<uint>({8,3,5,3,4,9,4,6,5,4,4,4,8,5,4,7});
            creature_part.emplace(PartCreatureType::TOP,(new CreatureInfo::Part(PartCreatureType::TOP,PartCreatureStatus::NORMAL,2)));
            creature_part.emplace(PartCreatureType::MIDDLE,(new CreatureInfo::Part(PartCreatureType::MIDDLE,PartCreatureStatus::NORMAL,3)));
            creature_part.emplace(PartCreatureType::BOTTOM,(new CreatureInfo::Part(PartCreatureType::BOTTOM,PartCreatureStatus::NORMAL,2)));
            creature_info.characteristic.velocity_limit  = 180;
            creature_info.characteristic.jump_power = 120;
            creature_info.characteristic.acceleration_power = 55;
            creature_info.characteristic.stamina = 80;
            creature_info.characteristic.stamina_limit = 80;
            creature_info.characteristic.blood   = 20;
            creature_info.characteristic.jump_ability = 1;
            creature_info.characteristic.current_jump_ability_num = 0;
            creature_info.characteristic.mass = 15;
            creature_info.characteristic.vision_radius = 200;
            break;
        }
        case CreatureInfo::Type::ERENU_DOO:{
            this->creature_info.animation.animationForWho = "erenu-doo";
            this->creature_info.animation.framesIdleNum   = std::vector<uint>({9,4,5,4,8,0,4,10,0,4,3,0,5,5,0,10});
            creature_part.emplace(PartCreatureType::TOP,(new CreatureInfo::Part(PartCreatureType::TOP,PartCreatureStatus::NORMAL,5)));
            creature_part.emplace(PartCreatureType::MIDDLE,(new CreatureInfo::Part(PartCreatureType::MIDDLE,PartCreatureStatus::NORMAL,5)));
            creature_part.emplace(PartCreatureType::BOTTOM,(new CreatureInfo::Part(PartCreatureType::BOTTOM,PartCreatureStatus::NORMAL,4)));
            creature_info.characteristic.velocity_limit  = 50;
            creature_info.characteristic.jump_power = 0;
            creature_info.characteristic.acceleration_power = 15;
            creature_info.characteristic.stamina = 300;
            creature_info.characteristic.stamina_limit = 300;
            creature_info.characteristic.blood   = 10;
            creature_info.characteristic.jump_ability = 0;
            creature_info.characteristic.current_jump_ability_num = 0;
            creature_info.characteristic.mass = 25;
            creature_info.characteristic.vision_radius = 100;
            break;
        }
        case CreatureInfo::Type::GOO_ZOO:{
            this->creature_info.animation.animationForWho = "goo-zoo";
            this->creature_info.animation.framesIdleNum   = std::vector<uint>({7,5,4,5,4,7,3,7,0,2,4,0,6,5,0,11});
            creature_part.emplace(PartCreatureType::TOP,(new CreatureInfo::Part(PartCreatureType::TOP,PartCreatureStatus::NORMAL,4)));
            creature_part.emplace(PartCreatureType::MIDDLE,(new CreatureInfo::Part(PartCreatureType::MIDDLE,PartCreatureStatus::NORMAL,6)));
            creature_part.emplace(PartCreatureType::BOTTOM,(new CreatureInfo::Part(PartCreatureType::BOTTOM,PartCreatureStatus::NORMAL,3)));
            creature_info.characteristic.velocity_limit  = 120;
            creature_info.characteristic.jump_power = 80;
            creature_info.characteristic.acceleration_power = 15;
            creature_info.characteristic.stamina = 200;
            creature_info.characteristic.stamina_limit = 200;
            creature_info.characteristic.blood   = 60;
            creature_info.characteristic.jump_ability = 0;
            creature_info.characteristic.current_jump_ability_num = 0;
            creature_info.characteristic.mass = 50;
            creature_info.characteristic.vision_radius = 300;
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
    addAnimation("_animation_climbing",creature_info.animation.framesIdleNum[14],0.2,false);
    addAnimation("_animation_death",creature_info.animation.framesIdleNum[15],0.2,false);

    
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
    currentLayer->addChild(creature_sprite,SceneZOrder::MIDLEGROUND,indentificator);

}
Creature::~Creature(){}
float Creature::getDistanceTo(cocos2d::Vec2 target){
    float cat1 = std::fabs(creature_sprite->getPositionX() - target.x);
    float cat2 = std::fabs(creature_sprite->getPositionY() - target.y);
    return std::sqrt((std::pow(cat1,2)+std::pow(cat2,2)));
}
void Creature::getStatistics(){
    if (!isStatisticsShowing){
        isStatisticsShowing = true;
        creature_statistics = cocos2d::Label::createWithTTF("","fonts/arial.ttf",18,cocos2d::Size::ZERO);
        creature_statistics->setPosition(creature_sprite->getPosition());
        currentLayer->addChild(creature_statistics,SceneZOrder::USER_INTERFACE);
    }
    else{
        isStatisticsShowing = false;
        currentLayer->removeChild(creature_statistics);
    }
}
void Creature::removeStatistics(){
    if (isStatisticsShowing){
        isStatisticsShowing = false;
        currentLayer->removeChild(creature_statistics);
    }
}
void Creature::setStatistics(DebugStatistics mode){
    std::string partStatus;
    /*Set strings about part of body*/
    partStatus.append("Status:\n");
    if (mode == DebugStatistics::GAME_STATS){
        for (auto &part : creature_part){
            switch(part.second->type){
                case PartCreatureType::TOP:{
                    partStatus.append("\ttop:");
                    break;
                }
                case PartCreatureType::MIDDLE:{
                    partStatus.append("\tmidle:");
                    break;
                }
                case PartCreatureType::BOTTOM:{
                    partStatus.append("\tbottom:");
                    break;
                }
            }
            partStatus.append(part.second->status == PartCreatureStatus::NORMAL ? "norm-" :
                              part.second->status == PartCreatureStatus::WONDED ? "wonded-" :
                              part.second->status == PartCreatureStatus::CUTTED ? "cutted-" :
                              "killed");
            partStatus.append(std::to_string(part.second->integrality) + "\n");

        }
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
                          creature_info.state == CreatureInfo::State::CLIMBING ? "CLIMBING\n" :
                          creature_info.state == CreatureInfo::State::DEATH ? "DEATH\n" :
                          creature_info.state == CreatureInfo::State::STOP ? "STOP\n" :
                          creature_info.state == CreatureInfo::State::IN_BATTLE ? "IN_BATTLE\n" :
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
        isWeaponSet = true;
        creature_weapon = new Sword("swordStock.png",this);
        break;
    }
    case WeaponType::AXE:{
        isWeaponSet = true;
        creature_weapon = new Axe("axeStock.png",this);
        break;
    }
    case WeaponType::SPEAR:{
        isWeaponSet = true;
        creature_weapon = new Spear("spearStock.png",this);
        break;
    }
    default:{
        isWeaponSet = false;
        CCLOG("Weapon not set\n");
        break;
    }
    }
    creature_weapon->getSprite()->setPosition(creature_sprite->getPosition());
    currentLayer->addChild(creature_weapon->getSprite(),SceneZOrder::MIDLEGROUND);
}
void Creature::setArmor(ArmorType aType){
    
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
    if (isWeaponSet)
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
        creature_weapon->updateAttackAnimation();
        setCreatureState(CreatureInfo::State::IDLE);
        break;
    }
    case CreatureInfo::State::GET_DAMMAGE:{
        creature_sprite->stopAllActions();
        creature_sprite->runAction(animations.find("_animation_getdammage")->second);
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
        if(isWeaponSet)
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
        creature_physic_body->setVelocity(cocos2d::Vec2(0,creature_physic_body->getVelocity().y));
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
        if(isWeaponSet)
            creature_weapon->getSprite()->setFlippedX(isFlipped);
        newVelocity = cocos2d::Vec2(creature_info.characteristic.acceleration_power * creature_info.dmove, 0);
        creature_physic_body->setVelocity(cocos2d::Vec2(creature_physic_body->getVelocity().x + newVelocity.x,
                                                        creature_physic_body->getVelocity().y));
        ///*I add this because I wont let creatures soaring when they are on the ground*/
        //if (creature_physic_body->getVelocity().y > -1 && creature_physic_body->getVelocity().y < 1){
        //    setCreatureState(CreatureInfo::LAND_ON);
        //}
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
        Creature::isInInteraction = true;
        bool isIntersection = false;
        for (const auto& lI : WorldProperties::dynamicObj){
            //Will open door if creature neer by, it's a door and buttom E pressed
            if (creature_sprite->getBoundingBox().intersectsRect(lI.second.rect) && lI.second.frameName == "door_open0.png"){
                //Door closed
                if (lI.second.spr->getPhysicsBody()->getCollisionBitmask() == 0x01){
                    lI.second.spr->runAction(WorldProperties::actionPool.find("door_open")->second->clone());//Change on lI.second.typeAction
                    lI.second.spr->getPhysicsBody()->setPositionOffset(cocos2d::Vec2(10,0));
                    lI.second.spr->getPhysicsBody()->setCollisionBitmask(0);
                    isIntersection = false;
                }
                //Door opened
                else {
                    lI.second.spr->runAction(WorldProperties::actionPool.find("door_open")->second->clone()->reverse());
                    lI.second.spr->getPhysicsBody()->setPositionOffset(cocos2d::Vec2(-10,0));
                    lI.second.spr->getPhysicsBody()->setCollisionBitmask(0x01);
                    isIntersection = false;
                }
                creature_sprite->runAction(cocos2d::MoveBy::create(0,cocos2d::Vec2(-10 * creature_info.dmove,0)));//I need this because, when you open door, some collision not updated
            }
            //Will activate something
            else if (creature_sprite->getBoundingBox().intersectsRect(lI.second.rect) && lI.second.frameName == "lever0.png"){
                //Execute once
                if (lI.second.target->getNumberOfRunningActions() == 0){
                    lI.second.spr->runAction(WorldProperties::actionPool.find("lever")->second->clone());
                    lI.second.target->runAction(WorldProperties::actionPool.find(lI.second.targetAction)->second->clone());
                }
                isIntersection = false;
            }
            //Will climbin on stair if creature neer by, it's a stair and buttom E pressed
            else if (creature_sprite->getBoundingBox().intersectsRect(lI.second.rect) && lI.second.frameName == "empty.png"){
                //Here animation for climbing
                setCreatureState(CreatureInfo::State::CLIMBING);
                isIntersection = true;
            }
            else if (creature_sprite->getBoundingBox().intersectsRect(lI.second.rect) && lI.second.frameName == "button_push0.png"){
                //Execute once
                if (lI.second.target->getNumberOfRunningActions() == 0){
                    lI.second.spr->runAction(WorldProperties::actionPool.find("button_push")->second->clone());
                    lI.second.target->runAction(WorldProperties::actionPool.find(lI.second.targetAction)->second->clone());
                }
                isIntersection = false;
            }
        
        }
        //If we not closed to item or buttom not pressed we will reser buttom state
        if (!isIntersection){
            setCreatureState(CreatureInfo::State::IDLE);
        }
        break;
    }
    case CreatureInfo::State::DEATH:{
        creature_sprite->stopAllActions();
        creature_sprite->runAction(animations.find("_animation_death")->second);
        isNewState = false;
        break;
    }
    case CreatureInfo::State::CLIMBING:{
        creature_sprite->stopAllActions();
        creature_sprite->runAction(animations.find("_animation_climbing")->second);
        creature_physic_body->setVelocity(cocos2d::Vec2(0,400));
        isNewState = false;
        break;
    }
    case CreatureInfo::State::STOP:{
        creature_sprite->stopAllActions();
        setCreatureState(CreatureInfo::State::IDLE);
        creature_physic_body->setVelocity(cocos2d::Vec2(0,0));
        break;
    }
    case CreatureInfo::State::IN_BATTLE:{
        creature_sprite->stopAllActions();
        creature_sprite->runAction(cocos2d::RepeatForever::create(animations.find("_animation_idle")->second));
        isNewState = false;
        creature_physic_body->setVelocity(cocos2d::Vec2(0,creature_physic_body->getVelocity().y));
        break;
    }
    }
}
