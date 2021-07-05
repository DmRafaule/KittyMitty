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
    void update(float dt,TypeUI type, void* Layer);
    void updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer);
    void updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer);
    void updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer);
    void updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer);
    static inline cocos2d::Vec2 getTouchLocation(int i) {return touchPosition2[i];};
private:
    class Control_Ball{
    public:
        Control_Ball();
        ~Control_Ball();

        static void create( void* node);
        static void remove( void* node);
        static inline const float getAngle(){ return angleDirection; };
        static inline const bool getMoving(){ return isMoving; };
    private:
           /** Set position of point related to pointNode like pointNode is a center of circle
           @param point_destination is a point will be changed to new position depends on pos of pointNOde and angle of texture of point node
           @param point_center is a center of entity node which have to have some kind of position and angle 
           **/
        static void setPosPointOnCircle(cocos2d::Vec2& point_destination,cocos2d::Vec2 point_center);
    private:
        static std::vector<cocos2d::DrawNode*> pathEffect;
        static float   part_radius;
        static float   angleDirection;
        static bool    isMoving;
    };
    cocos2d::Sprite* ball;
    cocos2d::Vec2 ballDefaultPosition;

    std::vector<cocos2d::Vec2> touchPosition;
    std::vector<bool>   isTouch;
    static std::vector<cocos2d::Vec2> touchPosition2;
};