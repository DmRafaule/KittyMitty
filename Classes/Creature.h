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
    ~Creature();
    void update(float dt);
    inline BodyMap* getBodyMap(){ return body_map;};
private:
private:
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