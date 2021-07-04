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
    void update(float dt,void* Layer);
    void updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer);
    void updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer);
    void updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer);
    static inline cocos2d::Vec2 getTouchLocation(int i) {return touchPosition2[i];};
private:
    cocos2d::Sprite* ball;
    cocos2d::Vec2 ballDefaultPosition;

    std::vector<cocos2d::Vec2> touchPosition;
    std::vector<bool>   isTouch;
    static std::vector<cocos2d::Vec2> touchPosition2;
};