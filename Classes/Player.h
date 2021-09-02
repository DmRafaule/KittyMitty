#pragma once

#include "Creature.h"

class Enemy;
class Player : public Creature{
public:
    Player(CreatureInfo::Type Type, cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id);
    virtual void update(float dt) override;
    virtual void remove() override;
    /**
     * @brief set up positions for creature sprite, and all attachment(weapon,armor, spells ...)
    */
    void setPlayerPosition(float x, float y);
    /**
     * @return pointer to data of creature_parts of player object
    */
    inline std::vector<PartCreature> getPartsOfCreature() {return creature_parts;};
    inline int* getCurrentInteractedEnemy(){ return &currentInteractedEnemy; };
private:
    std::vector<Enemy*>* enemyNode;//All enemies which player can giveEffect//MAYBE NOT NEED
    int  currentInteractedEnemy;//current available enemy for giveEffect
};