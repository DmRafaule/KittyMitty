#pragma once

#include <cocos2d.h>
#include "Weapon.h"
#include "engEnums.hpp"


class Creature{
friend class PartCreature;
public:
    Creature(CreatureInfo info ,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,std::string id);
    virtual ~Creature();
    virtual void update(float dt) = 0;
    /*Initializer*/
    void initAnimations(Animation infoAnimation);
    /*Clearers*/
    void removeSprite();
    void removeStatistics();
    /*Getters*/
    inline cocos2d::Sprite* getCreatureSprite() { return creature_sprite; };
    inline CreatureType getCreatureType() {return creature_type; };
    inline const CreatureState* getCreatureState() {return &creature_state; };
    inline const CreatureCharacteristics* getCreatureCharacteristic() {return &creature_characteristics; };
    uint getPart(PartCreatureType part_type, PartCreatureField part_field);
    PartOrgan& getOrgan(PartCreatureType part_type, PartOrganType part_organ_type);
    void getStatistics();//Display information about creature node 
    inline Weapon* getWeapon() { return creature_weapon;};
    inline DirectionMove* getCreatureDirectionMove() { return &creature_direction_move;};
    /*Setters*/
    void setPart(PartCreatureType part_type, PartCreatureStatus part_status, uint part_densityDef);
    void setOrgan(PartCreatureType part_type,PartOrganType part_organ_type,PartCreatureStatus status);
    inline CreatureCharacteristics* setCreatureCharacteristic() {return &creature_characteristics; };
    void setCreatureDirectionMove(DirectionMove dirMove);
    void setCreatureState(CreatureState creature_state);
    void setStatistics(DebugStatistics mode);//Init information about creature node 
    void setWeapon(WeaponType wMap );//Set creature_weapon to creature and current layer
protected:
    void updatePermament();//update stuff permamently
    void updateCurrentState();//Make some actions depens on which state creature have
    void showStatistics(DebugStatistics type);
    void losingStamina();
    void regeneratingStamina(float dt);
    /*This is part of body(*/
    class PartCreature{
    friend class Creature;
    public:
        PartCreature(PartCreatureType part_type);
        /*Getters*/
        inline PartCreatureStatus getStatus(){ return part_status; };
        inline PartCreatureType getType(){ return part_type; };
        inline std::vector<PartOrgan> getPartOrgan(){ return part_organs;}
        inline uint getDensity(){ return part_densityDef; };
        inline uint getPenetration(){ return part_penetrationDef; };
        inline uint getCrushing(){ return part_crushingDef; };
    private:
        /*Property related to part of creature*/
        std::vector<PartOrgan>          part_organs;
        PartCreatureStatus              part_status;
        PartCreatureType                part_type;
        uint                            part_densityDef;//Can be concatenated (for exm armor,shild etc.)
        uint                            part_penetrationDef;
        uint                            part_crushingDef;
    };
protected:
    /*Properties related to whole creature*/
    std::vector<PartCreature> creature_parts;//Container for holding info about creature_parts of creature 
    cocos2d::PhysicsBody*     creature_physic_body;
    cocos2d::Sprite*          creature_sprite;//Container for holding sprite creature
    cocos2d::Label*           creature_statistics;
    Weapon*                   creature_weapon;
    CreatureType              creature_type;//CreatureType of creature
    CreatureCharacteristics   creature_characteristics;
    CreatureState             creature_state;
    DirectionMove             creature_direction_move;
    cocos2d::Node*            currentlayer;//Current playing scene;
    /*Animations*/
    cocos2d::Animate*         animation_idle;


    bool                      isStatisticsShowing;
    bool                      isNewState;
};





/*INHERITATED CLASSES*/
class Enemy : public Creature{
public:
    Enemy(CreatureInfo info, cocos2d::Vec2 pos,cocos2d::Node* gameLayer,std::string id);
    virtual void update(float dt) override;
    /**
     * @return pointer to data of creature_parts of enemy object
    */
    inline std::vector<PartCreature> getPartsOfCreature() {return creature_parts;};
private:
};
class Player : public Creature{
public:
    Player(CreatureInfo info, cocos2d::Vec2 pos,cocos2d::Node* gameLayer,std::string id);
    virtual void update(float dt) override;
    /**
     * @return pointer to data of creature_parts of player object
    */
    inline std::vector<PartCreature> getPartsOfCreature() {return creature_parts;};
    inline int* getCurrentInteractedEnemy(){ return &currentInteractedEnemy; };
private:
    std::vector<Enemy*>* enemyNode;//All enemies which player can giveEffect//MAYBE NOT NEED
    int  currentInteractedEnemy;//current available enemy for giveEffect
};