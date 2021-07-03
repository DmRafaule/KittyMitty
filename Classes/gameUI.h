#pragma once

#include <cocos2d.h>

enum TypeUI{
    CONTROL_BALL,
    CONTROL_KEYS,
};

class GameUI{
public:
    GameUI(TypeUI type,void* layer);
    ~GameUI();
private:
    cocos2d::Sprite* ball;
};