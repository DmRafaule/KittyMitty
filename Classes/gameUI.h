#pragma once

#include <cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "engEnums.hpp"

class GameUI{
public:
    /**
     * @param type which GUI will be created
     * @param layer To which layer(scene) this GUI will be attached
     * In common just create GUI
    */
    GameUI();
    virtual ~GameUI();
    /**
     * Update GUI by the time 
     * @param dt -> elapsed time
     * @param type -> which GUI will be updated
     * @param Layer -> for which node will be applayed changes
    */
    virtual void update(float dt, void* Layer) = 0;
        // Will be called in touchBegan function in any of Node(Layer)
        virtual void updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) = 0;
        // Will be called in touchEnded function in any of Node(Layer)
        virtual void updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) = 0;
        // Will be called in touchMoved function in any of Node(Layer)
        virtual void updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) = 0;
        // Will be called in touchCanceled function in any of Node(Layer)
        virtual void updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) = 0;
    /**
     * Create line path effect
     * @param node wich layer should render this eff
    */
    virtual void createEffect( void* node) = 0;
    /**
     * Create line path effect
     * @param node wich layer should remove this eff
    */
    virtual void removeEffect( void* node) = 0;
protected:
    std::vector<cocos2d::DrawNode*> pathEffect;
    cocos2d::Vec2  touchPoint;
};

/**
 * @brief
 * For display statistics about entities(only logic true of false(do something))
*/
class ShowStats   : public GameUI{
public:
    ShowStats(void* layer);
    virtual ~ShowStats();
    virtual void update(float dt,void* Layer) override;
        virtual void updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
        virtual void updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
        virtual void updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
        virtual void updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
    virtual void createEffect( void* node) override;
    virtual void removeEffect( void* node) override;
private:
    float doubleDelay;
    int doubleCount;
    bool isdouble;
    bool forPlayer;
    bool forEnemy;

};
/**
 * @brief
 * For make a decition of which part of body player can attack
*/
class ControlTargeting{
public:
    ControlTargeting(void* layer);
    virtual ~ControlTargeting();

    /**
     * @return chosen target by player
    */
    static inline PartCreatureType getTarget(){ return target; };
    /**
     * @brief set some ui(one buttom for each part of body) for choosing target
    */
    static void setTarget(DirectionAttacke direction, void* currentLayer);
private:
    /**
     * @brief unset all created buttons (created by setTarget)
    */
    static void unsetTarget(void* currentLayer);
private:
    static PartCreatureType target;//Which target(part of body) will be attacked
    static cocos2d::Vec2 offset;
    bool clickForOpen;
};
/**
 * @brief
 * Give player control on kittymitty(for android)
 * IN DEVELOPMENT
*/
class ControlBall : public GameUI{
public:
    ControlBall(void* layer);
    virtual ~ControlBall();

    virtual void update(float dt,void* Layer) override;
        virtual void updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
        virtual void updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
        virtual void updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
        virtual void updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
    virtual void createEffect( void* node) override;
    virtual void removeEffect( void* node) override;
    /**
     * @return angle bettween touch point and center of ball control
    */
    static inline const float getAngle(){ return angleDirection; };
    /**
     * @return status of interaction with ball controller
    */
    static inline const bool getMoving(){ return isMoving; };
    /**
     * @return direction of movement for ball Sprite
    */
    static inline const cocos2d::Vec2 getDirection(){ return directionPoint; };
    /**
     * @return ball default position
    */
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
    
    static cocos2d::Vec2 directionPoint;//Position on circle of point in pathEffect
    static float   part_radius;//Radius of each point contained in pathEffect
    static float   angleDirection;//Angle of point in pathEffect
    static bool    isMoving;//Is player interact with ball controle
    
    cocos2d::Vec2  ballDefaultPosition;
    bool           isControlBall;//Is player using control ball. If so clear and render some gui effect
};
/**
 * @brief for controling player by some graphycs buttons
 * jumps moves run here(for android)
*/
class ControlKeys{
public:
    ControlKeys(cocos2d::Vec2 offset, void* layer);
    virtual ~ControlKeys();
    /**
     * @return angle bettween touch point and center of ball control
    */
    static inline const DirectionMove getDirectionMove(){ return directionMove; };
    /**
     * @return status of interaction with ball controller
    */
    static inline const bool getMoving(){ return isMoving; };
    /**
     * @return direction of movement for ball Sprite
    */
    static inline const cocos2d::Vec2 getDirection(){ return directionPoint; };
private:
    cocos2d::ui::Button* button_left;
    cocos2d::ui::Button* button_right;
    cocos2d::Vec2 offset;
    static cocos2d::Vec2 directionPoint;
    static bool isMoving;
    static DirectionMove directionMove;
};
/**
 * @brief
 * To enable for player attack using different movements(for android)
*/
class ControlAttc : public GameUI{
public:
    /*Inherite func*/    
    ControlAttc(void* layer);
    virtual ~ControlAttc();
    virtual void update(float dt, void* layer) override;
        virtual void updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
        virtual void updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
        virtual void updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
        virtual void updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;

    virtual void createEffect( void* node) override;
    virtual void removeEffect( void* node) override;
public:
    /**
     * @return status of attack
    */
    inline static const bool getAttacke() { return isAttacke; };
    /**
     * set up status of attack
    */
    inline static void setAttacke(bool statusAttacke) { isAttacke = statusAttacke; };
    /**
     * @return direction of attack
    */
    inline static const DirectionAttacke getDirectionAttacke(){ return direction_of_attacke; };
private:
    void setDirectionAttacke();
private:

    static cocos2d::Vec2  trembling;                    //This is a  diffrance bettween touchPointStart and toucPointEnd
    static cocos2d::Vec2  touchPointStart;              //Where player start touch on screen
    static cocos2d::Vec2  touchPointEnd;                //Where player stop touch screen
    static DirectionAttacke direction_of_attacke;       //Which direction of attacke player made
    static bool  isAttacke;                             //Is this UI object activated
    bool   isRightPlaceForControle;                     //Detect is player finger on right half of screen
};