#include "Player.h"
#include "engMacros.hpp"
///////////////////////////////////////////////////////*Player class*///////////////////////////////////////////////////////
Player::Player(CreatureInfo::Type type,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id) :
    Creature(type,pos,gameLayer,id){
    
    currentInteractedEnemy = -1;
    creature_info.characteristic.stamina_regeneration_counter = 0;
    creature_part.find(PartCreatureType::TOP)->second->armor = 5;
}
void Player::setPlayerPosition(float x, float y){
    creature_sprite->setPosition(x,y);
    if (isWeaponSet)
        creature_weapon->getSprite()->setPosition(x,y);
    if (creature_info.isStatisticsShowing)
        creature_statistics->setPosition(x,y);
}
void Player::update(float dt){
    showStatistics(DebugStatistics::GAME_STATS);
    if (isNewState){
        updateCurrentState();
    }
    updatePermament();
    updateRegeneration(dt);
}
void Player::remove(){
    
}
void Player::showStatistics(DebugStatistics type){
    /*For statistics*/
    if (creature_info.isStatisticsShowing){
        setStatistics(type);
    }
}
void Player::removeStatistics(cocos2d::Node* layer){
    if (creature_info.isStatisticsShowing){
        creature_info.isStatisticsShowing = false;
        layer->removeChild(creature_statistics);
    }
}
void Player::initEnemyContainer(std::vector<Enemy*> enemies){
    this->enemyNode = &enemies;
}