#pragma once

#include <cocos2d.h>
#include "Weapon.h"
#include "Armor.h"
#include "engEnums.hpp"
#include <map>
#include <queue>


class Creature{
public:
    Creature();
    Creature(CreatureInfo::Type type ,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id);
    virtual ~Creature();
    virtual void update(float dt) = 0;
    /*Clearers*/
    virtual void remove() = 0;
    /*Getters*/
    inline CreatureInfo* getCreatureInfo() { return &creature_info; };
    inline cocos2d::Sprite* getCreatureSprite() { return creature_sprite; };
    inline bool getWeaponSetupStatus() { return isWeaponSet; }; 
    inline std::map<PartCreatureType,CreatureInfo::Part*>* getPartCreature() { return &creature_part; };
    inline Weapon* getWeapon() { return creature_weapon;};
    void getStatistics();//Display information about creature node
    /*Setters*/
    void setCreatureState(CreatureInfo::State creature_state);
    void setStatistics(DebugStatistics mode);//Init information about creature node 
    void removeStatistics();// Remove statistics
    void setWeapon(WeaponType wMap );//Set creature_weapon to creature and current layer
    void setArmor(ArmorType aType);//Set armor for creature
    float getDistanceTo(cocos2d::Vec2 target);// Define distance to target
protected:
    /*Initializer*/
    void initStats();
    void initAnimations();
    void initBody(cocos2d::Vec2 pos);
    
    void updatePermament();//update stuff permamently
    void updateCurrentState();//Make some actions depens on which state creature have
    
    void showStatistics(DebugStatistics type);
    void losingStamina();
    void regeneratingStamina(float dt);
    void addAnimation(std::string anim_name,uint frame_number,float delay,bool restoreOrigFr);
protected:
    /*Properties related to whole creature*/
    std::multimap<std::string,cocos2d::Animate*> animations;//Animations
    std::map<PartCreatureType,CreatureInfo::Part*> creature_part;//Parts of creature
    cocos2d::PhysicsBody*     creature_physic_body;
    cocos2d::Sprite*          creature_sprite;//Container for holding sprite creature
    cocos2d::Label*           creature_statistics;
    cocos2d::Node*            currentLayer;//Current playing scene;
    CreatureInfo              creature_info;
    Weapon*                   creature_weapon; //Using weapon

    int                       indentificator;
    bool                      isStatisticsShowing;
    bool                      isNewState;
    bool                      isWeaponSet;
    static bool               isInInteraction;
};
