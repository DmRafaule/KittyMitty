#pragma once

#include <cocos2d.h>

struct BodyMap{
    BodyMap(unsigned int heads,unsigned int torses,unsigned int hands,unsigned int tails,unsigned int legs,unsigned int speedLimit);
    unsigned int heads;
    unsigned int torses;
    unsigned int hands;
    unsigned int tails;
    unsigned int legs;
    unsigned int speedLimit;
};

class Creature{
public:
    Creature(std::string texturePath,BodyMap bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id);
    virtual ~Creature();
    virtual void update(float dt) = 0;
    inline BodyMap* getBodyMap(){ return body_map;};
    inline cocos2d::Sprite* getCreatureSprite() { return creature_sprite; };
protected:
    /*This is part of body(like head or leg)*/
    class PartCreature{
        friend class Creature;
    };
    /*This is layer of part(skin,bone,musle...)*/
    class LayerPart{
        friend class Creature;
    };
    BodyMap* body_map;
    cocos2d::Sprite* creature_sprite;
};
class Enemy : public Creature{
public:
    Enemy(std::string texturePath,BodyMap bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id);
    virtual void update(float dt) override;
    bool isAlive;
private:
};
class Player : public Creature{
public:
    Player(std::string texturePath,BodyMap bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id);
    virtual void update(float dt) override;
private:
    //All enemies which player can interact
    std::vector<Enemy*> enemyNode;
    //For interact with entity
    cocos2d::Rect interaction_radius;
    //Current plaing scene;
    int  currentInteractedEnemy;
    void* layer;
};