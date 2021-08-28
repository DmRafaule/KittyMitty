#pragma once

#include <cocos2d.h>
#include "Weapon.h"
#include "engEnums.hpp"
#include <map>
#include <queue>

class Creature{
friend class PartCreature;
public:
    Creature(CreatureInfo::Type type ,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id);
    virtual ~Creature();
    virtual void update(float dt) = 0;
    /*Clearers*/
    virtual void remove() = 0;
    /*Getters*/
    inline const CreatureInfo* getCreatureInfo() { return &creature_info; };
    inline cocos2d::Sprite* getCreatureSprite() { return creature_sprite; };
    uint getPart(PartCreatureType part_type, PartCreatureField part_field);
    PartOrgan& getOrgan(PartCreatureType part_type, PartOrganType part_organ_type);
    void getStatistics();//Display information about creature node 
    inline Weapon* getWeapon() { return creature_weapon;};
    /*Setters*/
    void setCreatureState(CreatureInfo::State creature_state);
    inline CreatureInfo* setCreatureInfo() { return &creature_info; };
    void setPart(PartCreatureType part_type, PartCreatureStatus part_status, uint part_densityDef);
    void setOrgan(PartCreatureType part_type,PartOrganType part_organ_type,PartCreatureStatus status);
    void setStatistics(DebugStatistics mode);//Init information about creature node 
    void setWeapon(WeaponType wMap );//Set creature_weapon to creature and current layer
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
    /*Properties related to whole creature*/
    std::vector<PartCreature> creature_parts;//Container for holding info about creature_parts of creature 
    cocos2d::PhysicsBody*     creature_physic_body;
    cocos2d::Sprite*          creature_sprite;//Container for holding sprite creature
    cocos2d::Label*           creature_statistics;
    Weapon*                   creature_weapon;
    CreatureInfo              creature_info;
    std::multimap<std::string,cocos2d::Animate*> animations;//Animations
    cocos2d::Node*            currentLayer;//Current playing scene;

    int                       indentificator;
    bool                      isStatisticsShowing;
    bool                      isNewState;
};

/*INHERITATED CLASSES*/
class Enemy : public Creature{
public:
    Enemy(CreatureInfo::Type type, cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id);
    virtual void update(float dt) override;
    virtual void remove() override;
    /**
     * @return pointer to data of creature_parts of enemy object
    */
    inline std::vector<PartCreature> getPartsOfCreature() {return creature_parts;};
    /**
     * @brief init fields which related to player and cant be assigned in constructor
    */
    void initPlayerDependenceFields();
    void setAI(int typeAI, std::string typeBehaviorPattern);
    inline const cocos2d::Node* getCreatureVisions() { return creature_vision; };
    inline const bool isVisionEnable() { return isVision; };
    inline uint64_t& getMemory() { return creature_memorySensors; };
    inline const Sensor::TypeSensor getActiveSensor() { return creature_currentSensor; };
private:
    /* Define init vison pattern and set upd looking obj for Image recognition*/
    void updateVision();
        /* Set up queue for looking some objects in vision*/
        void setVisionPattern(std::queue<Sensor> pattern);
        /* Create vision obj 'creature vision' for future detecting in updateTouchBegan*/
        void setLookAt(const Sensor& look);
    /* Pack states into queue and then extract them from it*/
    void updateBehavior(float dt);
        /* First pack States into Behavior pattern then unpack them in to creature_state*/
        void packBehaviorStates(float dt);
            /* Define which direction creature will be use*/
            void defineDirection();
            /* Define state for making new decision*/
            BehaviorPattern defineBehavior();
            /* Assign new behavior pattern and previose BP*/
            void setBehaviorPattern(BehaviorPattern newBP);
        /* Set up new state for creature_state from queue*/
        void unpackBehaviorState(float dt);
private:
    cocos2d::Node* player;          //Date about player node
    cocos2d::Node* creature_vision; //Represent vision object for interacting with other world obj 
    std::queue<BehaviorState> creature_behaviorStates;  //Pack of expanded states for update them in updateCurrentState one by one
    std::queue<Sensor> creature_visionPattern;          //Pack of where vision object will be and which size it can be
    BehaviorPattern creature_behaviorPattern;           //Pattern represent in witch queue and what kind of state will be in creature_behaviorStates
    BehaviorPattern creature_previosBehaviorPattern;    //Previose behavior pattern
    Sensor::TypeSensor creature_currentSensor;          //Represent current poping sensors from queue
    uint64_t creature_memorySensors;//Bit field for remember wich sensors are active
    bool isVision;// For optimaing collision updates
    bool sawPlayer;//If it saw creature it will never stop
};

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