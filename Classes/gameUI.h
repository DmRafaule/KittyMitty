#pragma once

#include <cocos2d.h>
#include "Creature.h"

enum TypeUI{
    CONTROL_IN_GAMESESSION,
    CONTROL_KEYS,
};
enum DirectionAttacke{
    TOP_TO_DOWN,
    DOWN_TO_TOP,
    LEFT_TO_RIGHT,
    RIGHT_TO_LEFT,
    TOPLEFT_TO_BOTTOMRIGHT,
    TOPRIGHT_TO_BOTTOMLEFT,
    BOTTOMLEFT_TO_TOPRIGHT,
    BOTTOMRIGHT_TO_TOPLEFT,
};

class GameUI{
public:
    /**
     * @param type which GUI will be created
     * @param layer To which layer(scene) this GUI will be attached
     * In common just create GUI
    */
    GameUI(TypeUI type,void* layer);
    ~GameUI();
    /**
     * Update GUI by the time 
     * @param dt -> elapsed time
     * @param type -> which GUI will be updated
     * @param Layer -> for which node will be applayed changes
    */
    void update(float dt,TypeUI type, void* Layer);
        // Will be called in touchBegan function in any of Node(Layer)
        void updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer);
        // Will be called in touchEnded function in any of Node(Layer)
        void updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer);
        // Will be called in touchMoved function in any of Node(Layer)
        void updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer);
        // Will be called in touchCanceled function in any of Node(Layer)
        void updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer);

public:
    class Control_Ball{
    public:
        Control_Ball(void* layer);
        ~Control_Ball();
        void update(float dt,void* Layer);
            // Will be called in touchBegan function in any of Node(Layer)
            void updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer, uint iterator);
            // Will be called in touchEnded function in any of Node(Layer)
            void updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer, uint iterator);
            // Will be called in touchMoved function in any of Node(Layer)
            void updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer, uint iterator);
            // Will be called in touchCanceled function in any of Node(Layer)
            void updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer, uint iterator);
        /**
         * Create line path effect
         * @param node wich layer should render this eff
        */
        static void createEffect( void* node);
        /**
         * Create line path effect
         * @param node wich layer should remove this eff
        */
        static void removeEffect( void* node);
        /**
         * @return angle bettween touch point and center of ball control
        */
        static inline const float getAngle(){ return angleDirection; };
        /**
         * @return status of interaction with ball controller
        */
        static inline const bool getMoving(){ return isMoving; };
        /**
         * @return direction of movement for to ball Sprite
        */
        static inline const cocos2d::Vec2 getDirection(){ return directionPoint; };
        inline const cocos2d::Vec2 getBallDefaultPosition() { return ballDefaultPosition; };
    private:
           /** Set position of point related to pointNode like pointNode is a center of circle
           @param point_destination is a point will be changed to new position depends on pos of pointNOde and angle of texture of point node
           @param point_center is a center of entity node which have to have some kind of position and angle 
           **/
        static void setPosPointOnCircle(cocos2d::Vec2& point_destination,cocos2d::Vec2 point_center);
        /**
         * transform radians to degrees
        */
        static float setAngleToRadius(float angle_radian);
        /**
         * Calculated direction of movement relative
         * @param endPoint center of circle controle
         * @param startPoint touch location
         * @return point on circle
        */
        static cocos2d::Vec2 setDirectionPointRelative(cocos2d::Vec2 endPoint,cocos2d::Vec2 startPoint);
    private:
        static std::vector<cocos2d::DrawNode*> pathEffect;
        static cocos2d::Vec2 directionPoint;//Position on circle of point in pathEffect
        static cocos2d::Vec2  touchPoint;
        static float   part_radius;//Radius of each point contained in pathEffect
        static float   angleDirection;//Angle of point in pathEffect
        static bool    isMoving;//Is player interact with ball controle
        
        cocos2d::Vec2  ballDefaultPosition;
        bool           isControlBall;
    } *cBall;
    class Control_Attc{
    public:
        Control_Attc(void* layer);
        ~Control_Attc();
        void update(float dt, void* layer);
            // Will be called in touchBegan function in any of Node(Layer)
            void updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer, uint iterator);
            // Will be called in touchEnded function in any of Node(Layer)
            void updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer, uint iterator);
            // Will be called in touchMoved function in any of Node(Layer)
            void updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer, uint iterator);
            // Will be called in touchCanceled function in any of Node(Layer)
            void updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer, uint iterator);
        /**
         * Create line path effect
         * @param node wich layer should render this eff
        */
        static void createEffect( void* node);
        /**
         * Create line path effect
         * @param node wich layer should remove this eff
        */
        static void removeEffect( void* node);
    private:
        void setDirectionAttacke();
        inline const DirectionAttacke getDirectionAttacke(){ return direction_of_attacke; };
    private:
        static cocos2d::Vec2  touchPoint;
        cocos2d::Vec2 startPoint;
        cocos2d::Vec2 endPoint;
        DirectionAttacke direction_of_attacke;
        bool   isControlAttake;
    } *cAttc;
private:

};