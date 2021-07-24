#pragma once

#include <cocos2d.h>
#include "Weapon.h"
#include "engEnums.hpp"


class Creature{
friend class PartCreature;
public:
    Creature(std::string texturePath,CreatureType creature_type,cocos2d::Vec2 pos,void* gameLayer,std::string id);
    virtual ~Creature();
    virtual void update(float dt) = 0;
    /*Clearers*/
    void removeSprite();
    void removeStatistics();
    /*Getters*/
    inline cocos2d::Sprite* getCreatureSprite() { return creature_sprite; };
    inline CreatureType getCreatureType() {return creature_type; };
    inline const CreatureCharacteristics* getCreatureCharacteristic() {return &creature_characteristics; };
    uint getPart(PartCreatureType part_type, PartCreatureField part_field);
    PartOrgan& getOrgan(PartCreatureType part_type, PartOrganType part_organ_type);
    void getStatistics();//Display information about creature node 
    inline Weapon* getWeapon() { return creature_weapon;}
    /*Setters*/
    void setPart(PartCreatureType part_type, PartCreatureStatus part_status, uint part_densityDef);
    void setOrgan(PartCreatureType part_type,PartOrganType part_organ_type,PartCreatureStatus status);
    inline CreatureCharacteristics* setCreatureCharacteristic() {return &creature_characteristics; };
    void setStatistics(DebugStatistics mode);//Init information about creature node 
    void setWeapon(WeaponType wMap );//Set creature_weapon to creature and current layer
protected:
    void showStatistics();
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
    CreatureType              creature_type;//Type of creature
    CreatureCharacteristics   creature_characteristics;
    bool                      isStatisticsShowing;
    void*                     currentlayer;//Current playing scene;
};





/*INHERITATED CLASSES*/
class Enemy : public Creature{
public:
    Enemy(std::string texturePath,CreatureType bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id);
    virtual void update(float dt) override;
    /**
     * @return pointer to data of creature_parts of enemy object
    */
    inline std::vector<PartCreature> getPartsOfCreature() {return creature_parts;};
private:
};
class Player : public Creature{
public:
    Player(std::string texturePath,CreatureType bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id);
    virtual void update(float dt) override;
    /**
     * @return pointer to data of creature_parts of player object
    */
    inline std::vector<PartCreature> getPartsOfCreature() {return creature_parts;};
    inline int* getCurrentInteractedEnemy(){ return &currentInteractedEnemy; };
private:
    std::vector<Enemy*>* enemyNode;//All enemies which player can giveEffect
    int  currentInteractedEnemy;//current available enemy for giveEffect
};