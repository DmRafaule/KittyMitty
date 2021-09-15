#include "Enemy.h"
#include "engMacros.hpp"


BehaviorState::BehaviorState(CreatureInfo::State state, CreatureInfo::DMove dmove, float time){
    this->state = state;
    this->dmove = dmove;
    this->time  = time;
}
Sensor::Sensor(){

}
Sensor::Sensor(Sensor::TypeSensor type, cocos2d::Vec2 offset){
    this->type = type;
    switch(type){
        case TypeSensor::NEAR_BY_SIDE:{
            this->whereTo.x = 16;
            this->whereTo.y = 0;
            this->howTo.x   = 20;
            this->howTo.y   = 28;
            break;
        }
        case TypeSensor::NEAR_BY_SIDETOP2X:{
            this->whereTo.x = 16;
            this->whereTo.y = 75;
            this->howTo.x   = 10;
            this->howTo.y   = 28;
            break;
        }
        case TypeSensor::GROUND_UNDER_ME:{
            this->whereTo.x = 0;
            this->whereTo.y = -20;
            this->howTo.x   = 15;
            this->howTo.y   = 15;
            break;
        }
        case TypeSensor::GROUND_LEFTWARD_IS:{
            this->whereTo.x = -50;
            this->whereTo.y = -40;
            this->howTo.x   = 15;
            this->howTo.y   = 15;
            break;
        }
        case TypeSensor::GROUND_RIGHTWARD_IS:{
            this->whereTo.x = 50;
            this->whereTo.y = -40;
            this->howTo.x   = 15;
            this->howTo.y   = 15;
            break;
        }
        case TypeSensor::NEAR_BY_TOP:{
            this->whereTo.x = 0;
            this->whereTo.y = 32;
            this->howTo.x   = 28;
            this->howTo.y   = 10;
            break;
        }
        case TypeSensor::SOMETHING_IN_VISION_R:{
            this->whereTo.x = 110;
            this->whereTo.y = 32;
            this->howTo.x   = 50;
            this->howTo.y   = 20;
            break;
        }
        case TypeSensor::SOMETHING_ON_THE_SAME_LEVEL:{
            this->whereTo.x = 200;
            this->whereTo.y = 0;
            this->howTo.x   = 150;
            this->howTo.y   = 150;
            break;
        }
        case TypeSensor::SOMETHING_ABOVE:{
            this->whereTo.x = 200;
            this->whereTo.y = 200;
            this->howTo.x   = 150;
            this->howTo.y   = 150;
            break;
        }
        case TypeSensor::SOMETHING_BELOW:{
            this->whereTo.x = 200;
            this->whereTo.y = -200;
            this->howTo.x   = 150;
            this->howTo.y   = 150;
            break;
        }
    }
    this->whereTo.x += offset.x;
    this->whereTo.y += offset.y;
}


///////////////////////////////////////////////////////*Enemy class*///////////////////////////////////////////////////////
Enemy::Enemy(){

}
Enemy::Enemy(CreatureInfo::Type type,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id) :
    Creature(type,pos,gameLayer,id){
    creature_memorySensors = 0;
    creature_currentAttackPattern = 0;
    if (type == CreatureInfo::Type::KOOL_HASH){
        //BC function defineBattleAI call twice we push Type attack twice(stupid solution),
        //first for animation, second for stats changing
        creature_attackPattern.push_back(TypeAttacke::TOP_TO_DOWN);
        creature_attackPattern.push_back(TypeAttacke::TOP_TO_DOWN);
        creature_attackPattern.push_back(TypeAttacke::DOWN_TO_TOP);
        creature_attackPattern.push_back(TypeAttacke::DOWN_TO_TOP);
        creature_attackPattern.push_back(TypeAttacke::LEFT_TO_RIGHT);
        creature_attackPattern.push_back(TypeAttacke::LEFT_TO_RIGHT);
        creature_attackPattern.push_back(TypeAttacke::RIGHT_TO_LEFT);
        creature_attackPattern.push_back(TypeAttacke::RIGHT_TO_LEFT);
    }
    else if (type == CreatureInfo::Type::ERENU_DOO){
        creature_attackPattern.push_back(TypeAttacke::TOP_TO_DOWN);
        creature_attackPattern.push_back(TypeAttacke::TOP_TO_DOWN);
        creature_attackPattern.push_back(TypeAttacke::LEFT_TO_RIGHT);
        creature_attackPattern.push_back(TypeAttacke::LEFT_TO_RIGHT);
    }
    else if (type == CreatureInfo::Type::GOO_ZOO){
        creature_attackPattern.push_back(TypeAttacke::TOP_TO_DOWN);
        creature_attackPattern.push_back(TypeAttacke::TOP_TO_DOWN);
        creature_attackPattern.push_back(TypeAttacke::LEFT_TO_RIGHT);
        creature_attackPattern.push_back(TypeAttacke::LEFT_TO_RIGHT);

    }
    isVision  = false;
    sawPlayer = false;
}
void Enemy::remove(){
    creature_part.clear();
    currentLayer->removeChild(creature_sprite);
    if (isWeaponSet)
        currentLayer->removeChild(creature_weapon->getSprite());
    currentLayer->removeChild(creature_vision);
    while (!creature_behaviorStates.empty()){
        creature_behaviorStates.pop();
    }
    while (!creature_visionPattern.empty()){
        creature_visionPattern.pop();
    }

    if (creature_info.isStatisticsShowing)
        currentLayer->removeChild(creature_statistics);
    creature_info.isStatisticsShowing = false;
}
void Enemy::initPlayerDependenceFields(){
    player = currentLayer->getChildByTag(2);
}
void Enemy::setAI(std::string typeBehaviorPattern){
    //Set sensors to custom activity
    creature_memorySensors = Sensor::CUSTOM;
    //Define custom behavior patterns
    if (typeBehaviorPattern == "wait")
        creature_currentBehaviorPattern = BehaviorPattern::WAITING_NEW_BEHAVIORPATTERN;
    else if (typeBehaviorPattern == "patrol")
        creature_currentBehaviorPattern = BehaviorPattern::PATROL;
    else 
        creature_currentBehaviorPattern = BehaviorPattern::WAITING_NEW_BEHAVIORPATTERN;
}
void Enemy::showStatistics(DebugStatistics type){
    /*For statistics*/
    if (creature_info.isStatisticsShowing){
        setStatistics(type);
        creature_statistics->runAction(cocos2d::MoveTo::create(0.2,cocos2d::Vec2(creature_sprite->getPosition().x + creature_statistics->getBoundingBox().size.width/2,
                                                                                 creature_sprite->getPosition().y + creature_statistics->getBoundingBox().size.height/2)));
    }
}
void Enemy::removeStatistics(cocos2d::Node* layer){
    if (creature_info.isStatisticsShowing){
        creature_info.isStatisticsShowing = false;
        layer->removeChild(creature_statistics);
    }
}
void Enemy::update(float dt){
    showStatistics(DebugStatistics::ACTUAL_GAME);
    
    if (isNewState){
        updateCurrentState();
    }
    updatePermament();

}
void Enemy::updateVision(){
    //If player in vision radius
    if (getDistanceTo(player->getPosition()) < creature_info.characteristic.vision_radius || 
        sawPlayer){//Maybe here you should add some height statement, for detection
        sawPlayer = true;
        if (creature_visionPattern.empty()){
            //Here define wich sensor creature should use /AI/
            switch (creature_info.state){
            case CreatureInfo::State::IDLE:{
                setVisionPattern(std::queue<Sensor>({Sensor(Sensor::GROUND_UNDER_ME,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::GROUND_UNDER_ME,cocos2d::Vec2(0,-10)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(10,0)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(-10,0)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(0,10)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(0,-10)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(10,0)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(-10,0)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(0,10)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(0,-10)),}));
                break;
            }
            case CreatureInfo::State::ATTACK:{
                break;
            }
            case CreatureInfo::State::GET_DAMMAGE:{
                if (isWeaponSet)
                    getWeapon()->getSprite()->stopAllActions();
                break;
            }
            case CreatureInfo::State::START_RUN:{
                break;
            }
            case CreatureInfo::State::RUNNING:{
                setVisionPattern(std::queue<Sensor>({Sensor(Sensor::GROUND_UNDER_ME,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::GROUND_UNDER_ME,cocos2d::Vec2(0,-10)),
                                                     Sensor(Sensor::GROUND_UNDER_ME,cocos2d::Vec2(25,0)),
                                                     Sensor(Sensor::GROUND_UNDER_ME,cocos2d::Vec2(-25,0)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(10,0)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(-10,0)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(10,0)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(-10,0)),
                                                     Sensor(Sensor::NEAR_BY_SIDE,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::NEAR_BY_SIDE,cocos2d::Vec2(0,0))}));
                break;
            }
            case CreatureInfo::State::STAND_UP:{
                break;
            }
            case CreatureInfo::State::BRACKING:{
                break;
            }
            case CreatureInfo::State::IN_JUMP:{
                break;
            }
            case CreatureInfo::State::JUMP_FROM_WALL:{
                setVisionPattern(std::queue<Sensor>({Sensor(Sensor::SOMETHING_IN_VISION_R,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::SOMETHING_IN_VISION_R,cocos2d::Vec2(0,20)),
                                                     Sensor(Sensor::SOMETHING_IN_VISION_R,cocos2d::Vec2(0,-20)),
                                                     Sensor(Sensor::SOMETHING_IN_VISION_R,cocos2d::Vec2(0,-40)),
                                                     Sensor(Sensor::SOMETHING_IN_VISION_R,cocos2d::Vec2(0,-60)),
                                                     Sensor(Sensor::SOMETHING_IN_VISION_R,cocos2d::Vec2(0,-80)),}));
                break;
            }
            case CreatureInfo::State::IN_FALL:{
                break;
            }
            case CreatureInfo::State::SOARING:{
                setVisionPattern(std::queue<Sensor>({Sensor(Sensor::NEAR_BY_SIDE,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::NEAR_BY_SIDE,cocos2d::Vec2(0,0))}));
                break;
            }
            case CreatureInfo::State::LAND_ON:{
                setVisionPattern(std::queue<Sensor>({Sensor(Sensor::GROUND_UNDER_ME,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::GROUND_UNDER_ME,cocos2d::Vec2(0,-10)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(10,0)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(-10,0)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(0,10)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(0,-10)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(10,0)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(-10,0)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(0,10)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(0,-10)),}));
                break;
            }
            case CreatureInfo::State::ON_STEPS:{
                setVisionPattern(std::queue<Sensor>({Sensor(Sensor::GROUND_UNDER_ME,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::GROUND_UNDER_ME,cocos2d::Vec2(0,-10)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(10,0)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(-10,0)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(0,10)),
                                                     Sensor(Sensor::GROUND_RIGHTWARD_IS,cocos2d::Vec2(0,-10)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(10,0)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(-10,0)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(0,10)),
                                                     Sensor(Sensor::GROUND_LEFTWARD_IS,cocos2d::Vec2(0,-10)),}));
                break;
            }
            case CreatureInfo::State::MOVE_BY_STEPS:{
                break;
            }
            case CreatureInfo::State::ON_WALL:{
                setVisionPattern(std::queue<Sensor>({Sensor(Sensor::NEAR_BY_SIDE,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::NEAR_BY_SIDE,cocos2d::Vec2(0,0))}));
                break;
            }
            case CreatureInfo::State::LETGO:{
                break;
            }
            case CreatureInfo::State::TAKE_ROOF:{
                break;
            }
            case CreatureInfo::State::INTERACTING:{
                
                break;
            }
            case CreatureInfo::State::DEATH:{
                break;
            }
            case CreatureInfo::State::CLIMBING:{
                break;
            }
            case CreatureInfo::State::STOP:{
                setVisionPattern(std::queue<Sensor>({Sensor(Sensor::SOMETHING_ON_THE_SAME_LEVEL,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::SOMETHING_ABOVE,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::SOMETHING_ABOVE,cocos2d::Vec2(0,50)),
                                                     Sensor(Sensor::SOMETHING_ABOVE,cocos2d::Vec2(0,-50)),
                                                     Sensor(Sensor::SOMETHING_BELOW,cocos2d::Vec2(0,0)),
                                                     Sensor(Sensor::SOMETHING_BELOW,cocos2d::Vec2(0,50)),
                                                     Sensor(Sensor::SOMETHING_BELOW,cocos2d::Vec2(0,-50))}));
                break;
            }
            case CreatureInfo::State::IN_BATTLE:{
                
                break;
            }
            }
        }
        else {
            setLookAt(creature_visionPattern.front());//Extract from queue lookPattern
            creature_visionPattern.pop();//Remove extracted lookPattern
            //Here maybe you need add some clean up for creature_vision
        }
    }
}
void Enemy::updateBehavior(float dt){
    if (getDistanceTo(player->getPosition()) < creature_info.characteristic.vision_radius || 
        sawPlayer){
        packBehaviorStates(dt);
        unpackBehaviorState(dt);
    }
}
void Enemy::defineDirection(){
    if (player->getPositionX() > creature_sprite->getPositionX())
        creature_info.dmove = CreatureInfo::RIGHT;
    else 
        creature_info.dmove = CreatureInfo::LEFT;
}
void Enemy::defineBattleAI(){
    // Update only if weapon set and in current moment dont taking dammage
    if (isWeaponSet && creature_info.state != CreatureInfo::GET_DAMMAGE && creature_info.state != CreatureInfo::ATTACK ){
        if (getDistanceTo(player->getPosition()) < creature_weapon->getCaracteristics().weapon_range && 
            creature_info.state != CreatureInfo::IN_BATTLE){
            creature_behaviorStates.push(BehaviorState(CreatureInfo::IN_BATTLE,creature_info.dmove));
            // Here some logic for attacking
            if (creature_attackPattern.size() <= creature_currentAttackPattern){
                creature_currentAttackPattern = 0;
            }
            creature_info.dattack = creature_attackPattern.at(creature_currentAttackPattern);
            creature_behaviorStates.push(BehaviorState(CreatureInfo::ATTACK,creature_info.dmove,2));
            creature_currentAttackPattern++;
        }
        else if (getDistanceTo(player->getPosition()) > creature_weapon->getCaracteristics().weapon_range && 
                creature_info.state == CreatureInfo::IN_BATTLE){
            creature_behaviorStates.push(BehaviorState(CreatureInfo::IDLE,creature_info.dmove));
            creature_currentAttackPattern = 0;
        }
        //Try to keep distance between him and player
        if (getDistanceTo(player->getPosition()) <= creature_weapon->getCaracteristics().weapon_range/2 && 
            creature_info.state == CreatureInfo::IN_BATTLE){
            creature_behaviorStates.push(BehaviorState(CreatureInfo::START_RUN,CreatureInfo::DMove(creature_info.dmove * -1)));
            creature_currentAttackPattern = 0;
        }
    }
}
BehaviorPattern Enemy::defineBehavior(){
    /*Depends on which sensors are active will produce related behavior /AI/ */
    switch(creature_memorySensors){
        case Sensor::GROUND_UNDER_ME | Sensor::GROUND_RIGHTWARD_IS:{
            creature_currentBehaviorPattern = BehaviorPattern::STOP_BEFORE_SOMETHING;
            break;
        }
        case Sensor::GROUND_UNDER_ME | Sensor::GROUND_LEFTWARD_IS:{
            creature_currentBehaviorPattern = BehaviorPattern::STOP_BEFORE_SOMETHING;
            break;
        }
        /* On the ground, and something right before face of creature*/
        case Sensor::GROUND_LEFTWARD_IS | Sensor::GROUND_UNDER_ME | Sensor::GROUND_RIGHTWARD_IS | Sensor::NEAR_BY_SIDE:{
            if (creature_info.surface == CreatureInfo::InteractedSurface::WALL)
                creature_currentBehaviorPattern = BehaviorPattern::WALL_JUMP_FROM;
            else if (creature_info.surface == CreatureInfo::InteractedSurface::FLOOR)//In this case it's game crutch for detecting stairs and other physicly invisible obj
                creature_currentBehaviorPattern = BehaviorPattern::USING_ITEM;
            break;
        }
        /* In fall or in falling, he strike something by it's face*/
        case Sensor::NEAR_BY_SIDE:{
            if (creature_info.surface == CreatureInfo::InteractedSurface::WALL)
                creature_currentBehaviorPattern = BehaviorPattern::WALL_JUMP_FROM;
            else if (creature_info.surface == CreatureInfo::InteractedSurface::FLOOR)//In this case it's game crutch for detecting stairs and other physicly invisible obj
                creature_currentBehaviorPattern = BehaviorPattern::USING_ITEM;
            break;
        }
        case Sensor::SOMETHING_IN_VISION_R:{
                creature_currentBehaviorPattern = BehaviorPattern::WALL_JUMP_TO;
            break;
        }
        case Sensor::GROUND_LEFTWARD_IS | Sensor::GROUND_UNDER_ME | Sensor::GROUND_RIGHTWARD_IS:{
            creature_currentBehaviorPattern = BehaviorPattern::CHAISING;
            break;
        }
        case Sensor::GROUND_UNDER_ME:{
            creature_currentBehaviorPattern = BehaviorPattern::STOP_BEFORE_SOMETHING;
            break;
        }
        case Sensor::GROUND_LEFTWARD_IS:{
            creature_currentBehaviorPattern = BehaviorPattern::CHANGE_VERTICAL;
            break;
        }
        case Sensor::GROUND_RIGHTWARD_IS:{
            creature_currentBehaviorPattern = BehaviorPattern::CHANGE_VERTICAL;
            break;
        }
        //There is one vertical surfface, above
        case Sensor::SOMETHING_ABOVE:{
            creature_currentBehaviorPattern = BehaviorPattern::CHANGE_VERTICAL;
            break;
        }
        //There is one vertical surfface, on the same level
        case Sensor::SOMETHING_ON_THE_SAME_LEVEL:{
            creature_currentBehaviorPattern = BehaviorPattern::CHANGE_VERTICAL;
            break;
        }
        //There is one vertical surfface, bellow
        case Sensor::SOMETHING_BELOW:{
            creature_currentBehaviorPattern = BehaviorPattern::CHANGE_VERTICAL;
            break;
        }
        //There is two vertical surface ahead(on the creature level, and below)
        case Sensor::SOMETHING_ON_THE_SAME_LEVEL | Sensor::SOMETHING_BELOW:{
            creature_currentBehaviorPattern = BehaviorPattern::CHANGE_VERTICAL;
            break;
        }
        //There is two vertical surface ahead(below, and above)
        case Sensor::SOMETHING_BELOW | Sensor::SOMETHING_ABOVE:{
            creature_currentBehaviorPattern = BehaviorPattern::CHANGE_VERTICAL;
            break;
        }
        //There is two vertical surface ahead(on the creature level, and above)
        case Sensor::SOMETHING_ON_THE_SAME_LEVEL | Sensor::SOMETHING_ABOVE:{
            creature_currentBehaviorPattern = BehaviorPattern::CHANGE_VERTICAL;
            break;
        }
        //There is three vertical surface ahead
        case Sensor::SOMETHING_BELOW | Sensor::SOMETHING_ON_THE_SAME_LEVEL | Sensor::SOMETHING_ABOVE:{
            creature_currentBehaviorPattern = BehaviorPattern::CHANGE_VERTICAL;
            break;
        }
        case Sensor::EMPTY:{//If creature have not active sensors it will whait outcome interuption(in my game it player)
            creature_currentBehaviorPattern = BehaviorPattern::WAITING_NEW_BEHAVIORPATTERN;
            break;
        }
        case Sensor::CUSTOM:{//Defind on loadLevel World.cpp
            creature_currentBehaviorPattern = creature_currentBehaviorPattern;
            break;
        }
        default:{
            //CCLOG("undefind behavior\n");
            creature_currentBehaviorPattern = BehaviorPattern::WAITING_NEW_BEHAVIORPATTERN;
            break;
        }
    }
    return creature_currentBehaviorPattern;
}
void Enemy::packBehaviorStates(float dt){
    /*Will pack behavior states queue only if behaviorStates queue is empty and look pattern also empty*/
    if (creature_behaviorStates.empty() && creature_visionPattern.empty()){
        defineDirection();
        defineBattleAI();//Enter point to battle fase, for enemy(Maybe you should add oporunity enemy attack in aire)
        //Here define what kind of actions creature should do /AI/
        switch(defineBehavior()){
            case BehaviorPattern::CHAISING:{
                //CCLOG("chaising\n");
                //On the ground
                if (creature_info.state == CreatureInfo::State::IDLE ||
                    creature_info.state == CreatureInfo::State::STAND_UP)
                    creature_behaviorStates.push(BehaviorState(CreatureInfo::START_RUN,creature_info.dmove));
                //In air
                else if (creature_info.state == CreatureInfo::State::IN_FALL ||
                         creature_info.state == CreatureInfo::State::IN_JUMP)
                    creature_behaviorStates.push(BehaviorState(CreatureInfo::SOARING,creature_info.dmove));
                //On steps
                else if (creature_info.state == CreatureInfo::State::ON_STEPS)
                    creature_behaviorStates.push(BehaviorState(CreatureInfo::MOVE_BY_STEPS,creature_info.dmove));

                break;
            }
            case BehaviorPattern::CHANGE_VERTICAL:{
                if (player->getPositionY() > (creature_sprite->getPositionY() + 20)){
                    //CCLOG("jump to above\n");
                    if (creature_info.characteristic.stamina >= 5 && creature_info.characteristic.current_jump_ability_num <= creature_info.characteristic.jump_ability){
                        creature_behaviorStates.push(BehaviorState(CreatureInfo::State::IN_JUMP,creature_info.dmove,0.1));
                        creature_behaviorStates.push(BehaviorState(CreatureInfo::State::IN_JUMP,creature_info.dmove,0.1));
                    }
                    creature_behaviorStates.push(BehaviorState(CreatureInfo::State::SOARING,creature_info.dmove,0.2));
                }
                else if (player->getPositionY() < (creature_sprite->getPositionY() - 20)){
                    //CCLOG("fall to bottom\n");
                    creature_behaviorStates.push(BehaviorState(CreatureInfo::State::START_RUN,creature_info.dmove));
                    creature_behaviorStates.push(BehaviorState(CreatureInfo::State::SOARING,creature_info.dmove,1));
                }
                else{             
                    //CCLOG("jump over pit\n");
                    if (creature_info.characteristic.stamina >= 5 && creature_info.characteristic.current_jump_ability_num <= creature_info.characteristic.jump_ability)
                        creature_behaviorStates.push(BehaviorState(CreatureInfo::State::IN_JUMP,creature_info.dmove,0.1));
                    creature_behaviorStates.push(BehaviorState(CreatureInfo::State::SOARING,creature_info.dmove,0.2));
                }
                break;
            }
            case BehaviorPattern::USING_ITEM:{
                //CCLOG("using item\n");
                creature_behaviorStates.push(BehaviorState(CreatureInfo::INTERACTING,creature_info.dmove));
            }
            case BehaviorPattern::WALL_JUMP_TO:{
                if (creature_info.characteristic.stamina >= 5 && creature_info.characteristic.current_jump_ability_num <= creature_info.characteristic.jump_ability){
                    //CCLOG("jump to wall\n");
                    creature_behaviorStates.push(BehaviorState(CreatureInfo::State::IN_JUMP,creature_info.dmove,0.3));//Time delay is important here, if it's 0 interacting will not happend
                    creature_behaviorStates.push(BehaviorState(CreatureInfo::State::SOARING,creature_info.dmove));
                }
                break;
            }
            case BehaviorPattern::WALL_JUMP_FROM:{
                if (creature_info.characteristic.stamina >= 5 && creature_info.characteristic.current_jump_ability_num <= creature_info.characteristic.jump_ability){
                    //CCLOG("jump from wall\n");
                    creature_behaviorStates.push(BehaviorState(CreatureInfo::JUMP_FROM_WALL,creature_info.dmove,0.3));//Time delay is important here, if it's 0 interacting will not happend
                }
                break;
            }
            case BehaviorPattern::STOP_BEFORE_SOMETHING:{
                //CCLOG("stop and think\n");
                creature_behaviorStates.push(BehaviorState(CreatureInfo::State::STOP,creature_info.dmove));
                break;
            }
            case BehaviorPattern::WAITING_NEW_BEHAVIORPATTERN:{
                //CCLOG("waiting\n");
                break;
            }
            case BehaviorPattern::PATROL:{
                //CCLOG("patroling\n");
                creature_behaviorStates.push(BehaviorState(CreatureInfo::START_RUN,CreatureInfo::RIGHT,3));
                creature_behaviorStates.push(BehaviorState(CreatureInfo::START_RUN,CreatureInfo::LEFT,3));
                break;
            }
        }
    }
}
void Enemy::unpackBehaviorState(float dt){
    /* If our queue of behaviorPatterns contain some patterns*/
    if (!creature_behaviorStates.empty()){
        creature_behaviorStates.front().time -= dt;
        /*Will execute pattern only after some time, for those patterns*/
        if (creature_behaviorStates.front().time <= 0){
            setCreatureState(creature_behaviorStates.front().state);
            creature_info.dmove = creature_behaviorStates.front().dmove; 
            creature_behaviorStates.pop();
            isVision = false;
        }
    }
}
void Enemy::setVisionPattern(std::queue<Sensor> pattern){
    //Clear memory of sensors before load new one
    creature_memorySensors = 0;
    //Assigning new patterns
    while (!pattern.empty()){
        creature_visionPattern.push(pattern.front());
        pattern.pop();    
    }
    /*This is need because last lookPattern not updated, so */
    creature_visionPattern.push(Sensor(Sensor::EMPTY,cocos2d::Vec2(0,0)));
}
void Enemy::setLookAt(const Sensor& look){
    //Enable vision 
    isVision = true;
    //Remove previos vision node
    currentLayer->removeChild(creature_vision);
    //Create physic body for vision
    cocos2d::PhysicsBody* vision_body = cocos2d::PhysicsBody::createBox(cocos2d::Size(look.howTo));//Define size of vision
    vision_body->setCollisionBitmask(indentificator + 100);
    vision_body->setContactTestBitmask(0xFF);
    vision_body->setMass(10);
    vision_body->setDynamic(false);
    vision_body->setGravityEnable(false);

    creature_vision = cocos2d::Node::create();
    creature_vision->setPhysicsBody(vision_body);
    creature_vision->setPosition(creature_sprite->getPositionX() + look.whereTo.x * creature_info.dmove,creature_sprite->getPositionY() + look.whereTo.y * creature_sprite->getBoundingBox().size.height/64 );//Define where to look
    currentLayer->addChild(creature_vision);
    //Set up current sensor type (for memorize what it will see)
    creature_currentSensor = look.type;
}
