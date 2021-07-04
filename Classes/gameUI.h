#pragma once

#include <cocos2d.h>

enum TypeUI{
    CONTROL_BALL,
    ATTACKE_BALL,
    CONTROL_KEYS,
};

class GameUI{
public:
    GameUI(TypeUI type,void* layer);
    ~GameUI();
    cocos2d::Vec2 getDefaultPosition(TypeUI type);
private:
    cocos2d::Sprite* ball;
    cocos2d::Sprite* ball_attacke;
    cocos2d::Vec2 ballDefaultPosition;
    cocos2d::Vec2 ball_attackeDefaultPosition;
};