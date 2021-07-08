#pragma once

#include <cocos2d.h>

enum CreatureType{
    HUMANOID,
    INSECT,
    ANIMAL,
    TREE,
    FISH,
};
enum PartCreatureType{
    HEAD,
    UPPER_TORSE,
    BUTTOM_TORSE,
    HAND,
    LEG,
};
enum PartCreatureStatus{
    NORMAL,
    CUTTED,
    WONDED,
};


class Creature{
friend class PartCreature;
public:
    Creature(std::string texturePath,CreatureType type,cocos2d::Vec2 pos,void* gameLayer,std::string id);
    virtual ~Creature();
    virtual void update(float dt) = 0;
    inline cocos2d::Sprite* getCreatureSprite() { return creature_sprite; };
    inline uint getCreatureSpeed() { return creature_speed; };
    inline CreatureType getCreatureType() {return type;};
protected:
    /*This is part of body(*/
    class PartCreature{
    friend class Creature;
    public:
        PartCreature(PartCreatureType type);
        inline PartCreatureStatus getStatus(){ return status; };
        inline PartCreatureType getType(){ return type; }; 
    private:
        /*Property related to part of creature*/
        PartCreatureStatus status;
        PartCreatureType   type;
    };
protected:
    /*Properties related to whole creature*/
    std::vector<PartCreature> parts;//Container for holding info about parts of creature 
    cocos2d::Sprite* creature_sprite;//Container for holding sprite creature
    CreatureType type;//Type of creature
    uint creature_speed;//Creature speed
};





/*INHERITATED CLASSES*/
class Enemy : public Creature{
public:
    Enemy(std::string texturePath,CreatureType bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id);
    virtual void update(float dt) override;
    /**
     * @return pointer to data of parts of enemy object
    */
    inline std::vector<PartCreature> getPartsOfCreature() {return parts;};
private:
};
class Player : public Creature{
public:
    Player(std::string texturePath,CreatureType bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id);
    virtual void update(float dt) override;
    /**
     * @return pointer to data of parts of player object
    */
    inline std::vector<PartCreature> getPartsOfCreature() {return parts;};
private:
    /**
     * @param target which creature object should display
    */
    void getStatusOfPartsEnemy(std::vector<PartCreature> target); 
private:
    std::vector<Enemy*> enemyNode;//All enemies which player can interact
    cocos2d::Rect interaction_radius;//How far interacion could happend
    int  currentInteractedEnemy;//current available enemy for interact
    void* currentlayer;//Current plaing scene;
};