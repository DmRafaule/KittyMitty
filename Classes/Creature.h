#pragma once

#include <cocos2d.h>
#include "Weapon.h"
#include "engEnums.hpp"
#include <map>
#include <queue>


class Creature{
friend class PartCreature;
public:
    Creature();
    Creature(CreatureInfo::Type type ,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id);
    virtual ~Creature();
    virtual void update(float dt) = 0;
    /*Clearers*/
    virtual void remove() = 0;
    /*Getters*/
    inline const CreatureInfo* getCreatureInfo() { return &creature_info; };
    inline cocos2d::Sprite* getCreatureSprite() { return creature_sprite; };
    inline bool getWeaponSetupStatus() { return isWeaponSet; }; 
    uint getPart(PartCreatureType part_type, PartCreatureField part_field);
    void getStatistics();//Display information about creature node 
    inline Weapon* getWeapon() { return creature_weapon;};
    /*Setters*/
    void setCreatureState(CreatureInfo::State creature_state);
    inline CreatureInfo* setCreatureInfo() { return &creature_info; };
    void setPart(PartCreatureType part_type, PartCreatureStatus part_status, uint part_densityDef);
    void setStatistics(DebugStatistics mode);//Init information about creature node 
    void setWeapon(WeaponType wMap );//Set creature_weapon to creature and current layer
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
    /*This is part of body(*/
    class PartCreature{
    friend class Creature;
    public:
        PartCreature(PartCreatureType part_type);
        /*Getters*/
        inline PartCreatureStatus getStatus(){ return part_status; };
        inline PartCreatureType getType(){ return part_type; };
        inline uint getDensity(){ return part_densityDef; };
        inline uint getPenetration(){ return part_penetrationDef; };
        inline uint getCrushing(){ return part_crushingDef; };
    private:
        /*Property related to part of creature*/
        PartCreatureStatus              part_status;
        PartCreatureType                part_type;
        uint                            part_densityDef;//Can be concatenated (for exm armor,shild etc.)
        uint                            part_penetrationDef;
        uint                            part_crushingDef;
    };
    /*Properties related to whole creature*/
    std::multimap<std::string,cocos2d::Animate*> animations;//Animations
    std::vector<PartCreature> creature_parts;//Container for holding info about creature_parts of creature 
    cocos2d::PhysicsBody*     creature_physic_body;
    cocos2d::Sprite*          creature_sprite;//Container for holding sprite creature
    cocos2d::Label*           creature_statistics;
    Weapon*                   creature_weapon; //Using weapon
    CreatureInfo              creature_info;
    cocos2d::Node*            currentLayer;//Current playing scene;

    int                       indentificator;
    bool                      isStatisticsShowing;
    bool                      isNewState;
    bool                      isWeaponSet;
    static bool               isInInteraction;
};
