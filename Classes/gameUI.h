#pragma once

#include <cocos2d.h>

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
    bool forPlayer;
    bool forEnemy;
};
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
    
    static cocos2d::Vec2 directionPoint;//Position on circle of point in pathEffect
    static float   part_radius;//Radius of each point contained in pathEffect
    static float   angleDirection;//Angle of point in pathEffect
    static bool    isMoving;//Is player interact with ball controle
    
    cocos2d::Vec2  ballDefaultPosition;
    bool           isControlBall;
};
class ControlAttc : public GameUI{
public:/*Inherite func*/    
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
    inline static const bool getAttacke() { return isAttacke; };
    inline static void setAttacke(bool statusAttacke) { isAttacke = statusAttacke; };
    inline static const DirectionAttacke getDirectionAttacke(){ return direction_of_attacke; };
private:
    void setDirectionAttacke();
private:

    static cocos2d::Vec2  trembling;//This is a  diffrance bettween touchPointStart and toucPointEnd
    static cocos2d::Vec2  touchPointStart;
    static cocos2d::Vec2  touchPointEnd;
    static DirectionAttacke direction_of_attacke;//Which direction of attacke player made
    static bool  isAttacke;//Is this UI object activated
    bool   isRightPlaceForControle;
};