#pragma once

#include <cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "engEnums.hpp"

class GameUIPhone{
public:
    /**
     * @param type which GUI will be created
     * @param layer To which layer(scene) this GUI will be attached
     * In common just create GUI
    */
    GameUIPhone();
    virtual ~GameUIPhone();
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
class ShowStats   : public GameUIPhone{
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
class ControlTargeting : public GameUIPhone{
public:
    ControlTargeting(void* layer);
    virtual ~ControlTargeting();
    virtual void update(float dt,void* Layer) override;
        virtual void updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
        virtual void updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
        virtual void updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
        virtual void updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
    virtual void createEffect( void* node) override;
    virtual void removeEffect( void* node) override;

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
    cocos2d::Sprite* targetingB;
    bool clickForOpen;
};
/**
 * @brief for controling player by some graphycs buttons
 * jumps moves run here(for android)
*/
class ControlKeys : public GameUIPhone{
public:
    ControlKeys(cocos2d::Vec2 offset, void* layer);
    virtual ~ControlKeys();
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
    cocos2d::Sprite* button_left;
    cocos2d::Sprite* button_right;
    cocos2d::Sprite* button_jump;
    cocos2d::Vec2 offset;
    static cocos2d::Vec2 directionPoint;
    static bool isMoving;
    static DirectionMove directionMove;
};
/**
 * @brief
 * To enable for player attack using different movements(for android)
*/
class ControlAttc : public GameUIPhone{
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