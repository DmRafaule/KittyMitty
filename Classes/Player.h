#pragma once

#include "Creature.h"

class Enemy;
class Player : public Creature{
public:
    Player(CreatureInfo::Type Type, cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id);
    virtual void update(float dt) override;
    virtual void remove() override;
    virtual void showStatistics(DebugStatistics type) override;
    virtual void removeStatistics(cocos2d::Node* layer) override;
    void initEnemyContainer(std::vector<Enemy*> enemies);
    /*set up positions for creature sprite, and all attachment(weapon,armor, spells ...)*/
    void setPlayerPosition(float x, float y);
    inline int* getCurrentInteractedEnemy(){ return &currentInteractedEnemy; };
private:
    std::vector<Enemy*>* enemyNode;//All enemies which player can giveEffect//MAYBE NOT NEED
    int  currentInteractedEnemy;//current available enemy for giveEffect
};