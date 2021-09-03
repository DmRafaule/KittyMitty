#include "Player.h"

///////////////////////////////////////////////////////*Player class*///////////////////////////////////////////////////////
Player::Player(CreatureInfo::Type type,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id) :
    Creature(type,pos,gameLayer,id){
    currentInteractedEnemy = -1;
    creature_info.characteristic.stamina_regeneration_counter = 0;
}
void Player::setPlayerPosition(float x, float y){
    creature_sprite->setPosition(x,y);
    if (isWeaponSet)
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
void Player::remove(){
    
}
void Player::initEnemyContainer(std::vector<Enemy*> enemies){
    this->enemyNode = &enemies;
}