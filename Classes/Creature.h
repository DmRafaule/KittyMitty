#pragma once

#include <cocos2d.h>
#include "Weapon.h"

enum CreatureType : uint{
    HUMANOID,
    INSECT,
    ANIMAL,
    TREE,
    FISH,
};

enum PartCreatureType : uint{
    HEAD,
    UPPER_TORSE,
    BUTTOM_TORSE,
    HAND,
    LEG,
};
enum PartCreatureStatus : uint{
    NORMAL,
    CUTTED,
    WONDED,
};
enum PartCreatureField : uint{
    STATUS,
    DENSITY,
    PENETRATION,
};


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
    inline uint getCreatureSpeed() { return creature_speed; };
    inline uint getCreatureBlood() {return creature_blood; };
    inline uint getCreatureStamina() {return creature_stamina; };
    uint getPart(PartCreatureType part_type, PartCreatureField part_field);
    void getStatistics();//Display information about creature node 
    /*Setters*/
    void setPart(PartCreatureType part_type, PartCreatureStatus part_status, uint part_density, uint part_penetration);
    void setCreatureSpeed(uint creature_speed);
    void setCreatureBlood(uint creature_blood);
    void setCreatureStamina(uint creature_stamina);
    void setStatistics();//Init information about creature node 
    void setWeapon(WeaponType wMap );//Set weapon to creature and current layer
protected:
    /*This is part of body(*/
    class PartCreature{
    friend class Creature;
    public:
        PartCreature(PartCreatureType part_type);
        /*Getters*/
        inline PartCreatureStatus getStatus(){ return part_status; };
        inline PartCreatureType getType(){ return part_type; };
        inline uint getDensity(){ return part_density; };
        inline uint getPenetration(){ return part_penetration; };
    private:
        /*Property related to part of creature*/
        PartCreatureStatus part_status;
        PartCreatureType   part_type;
        uint               part_density;//Can be concatenated (for exm armor,shild etc.)
        uint               part_penetration;
    };
protected:
    /*Properties related to whole creature*/
    std::vector<PartCreature> creature_parts;//Container for holding info about creature_parts of creature 
    cocos2d::Sprite*          creature_sprite;//Container for holding sprite creature
    cocos2d::Label*           creature_statistics;
    Weapon*                   weapon1;
    CreatureType              creature_type;//Type of creature
    uint                      creature_speed;//Creature speed
    uint                      creature_blood;//How many blood liquid in creature
    uint                      creature_stamina;//How long creature can fight efficient
    void*                     currentlayer;//Current plaing scene;
    bool                      isStatisticsShowing;
};





/*INHERITATED CLASSES*/
class Enemy : public Creature{
public:
    Enemy(std::string texturePath,CreatureType bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id);
    virtual void update(float dt) override;
    //virtual void attachWeapon(std::string wMap ) override;
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
    //virtual void attachWeapon(std::string wMap ) override;
    /**
     * @return pointer to data of creature_parts of player object
    */
    inline std::vector<PartCreature> getPartsOfCreature() {return creature_parts;};
private:
    std::vector<Enemy*>* enemyNode;//All enemies which player can interact
    cocos2d::Rect interaction_radius;//How far interacion could happend
    int  currentInteractedEnemy;//current available enemy for interact
};