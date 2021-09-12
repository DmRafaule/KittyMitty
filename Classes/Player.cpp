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
    showStatistics(DebugStatistics::GAME_STATS);
    if (isNewState){
        updateCurrentState();
    }
    updatePermament();
}
void Player::remove(){
    
}
void Player::initStatistics(cocos2d::Node* layer){
    if (!isStatisticsShowing){
        isStatisticsShowing = true;
        creature_statistics = cocos2d::Label::createWithTTF("","fonts/arial.ttf",18,cocos2d::Size::ZERO);
        creature_statistics->setPosition(WorldProperties::screenSize.width * 0.1,WorldProperties::screenSize.height * 0.75);
        layer->addChild(creature_statistics,SceneZOrder::USER_INTERFACE);
    }
    else{
        isStatisticsShowing = false;
        layer->removeChild(creature_statistics);
    }
}
void Player::showStatistics(DebugStatistics type){
    /*For statistics*/
    if (isStatisticsShowing){
        setStatistics(type);
        //creature_statistics->runAction(cocos2d::MoveTo::create(0.2,cocos2d::Vec2(creature_sprite->getPosition().x + creature_statistics->getBoundingBox().size.width/2,
        //                                                                         creature_sprite->getPosition().y + creature_statistics->getBoundingBox().size.height/2)));
    }
}
void Player::initEnemyContainer(std::vector<Enemy*> enemies){
    this->enemyNode = &enemies;
}