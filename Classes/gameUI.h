#pragma once

#include <cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "engEnums.hpp"

class Enemy;
class Creature;

class GameUIPhone{
public:
    GameUIPhone();
    virtual ~GameUIPhone();
    /**
     * Update GUI by the time 
     * @param dt -> elapsed time
    */
    virtual void update(float dt) = 0;
        // Will be called in touchBegan function in any of Node(Layer)
        virtual void updateTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event) = 0;
        // Will be called in touchEnded function in any of Node(Layer)
        virtual void updateTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event) = 0;
        // Will be called in touchMoved function in any of Node(Layer)
        virtual void updateTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event) = 0;
        // Will be called in touchCanceled function in any of Node(Layer)
        virtual void updateTouchCanceled(cocos2d::Touch* touch,cocos2d::Event* event) = 0;
    /**
     * Create line path effect
     * @param node wich layer should render this eff
    */
    virtual void createEffect() = 0;
    /**
     * Create line path effect
     * @param node wich layer should remove this eff
    */
    virtual void removeEffect() = 0;
protected:
    std::vector<cocos2d::DrawNode*> pathEffect;
    cocos2d::Node*  currentLayer;
    Creature*       creature;
};

/**
 * @brief
 * For display statistics about entities(only logic true of false(do something))
*/
class ShowStats   : public GameUIPhone{
public:
    ShowStats(Creature* target, std::vector<Enemy*>* targetE, cocos2d::Node* layer);
    virtual ~ShowStats();
    virtual void update(float dt) override;
        virtual void updateTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event) override;
        virtual void updateTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event) override;
        virtual void updateTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event) override;
        virtual void updateTouchCanceled(cocos2d::Touch* touch,cocos2d::Event* event) override;
    virtual void createEffect() override;
    virtual void removeEffect() override;
private:
    std::vector<Enemy*>* creatureE;
    bool clickForOpen;

};
/**
 * @brief
 * For make a decition of which part of body player can attack
*/
class ControlTargeting : public GameUIPhone{
public:
    ControlTargeting(Creature* target,cocos2d::Node* layer);
    virtual ~ControlTargeting();
    virtual void update(float dt) override;
        virtual void updateTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event) override;
        virtual void updateTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event) override;
        virtual void updateTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event) override;
        virtual void updateTouchCanceled(cocos2d::Touch* touch,cocos2d::Event* event) override;
    virtual void createEffect() override;
    virtual void removeEffect() override;

    /**
     * @return chosen target by player
    */
    static inline PartCreatureType getTarget(){ return target; };
    /**
     * @brief set some ui(one buttom for each part of body) for choosing target
    */
    void setTarget(DirectionAttacke direction);
private:
    /**
     * @brief unset all created buttons (created by setTarget)
    */
    void unsetTarget();
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
    ControlKeys(Creature* target, cocos2d::Vec2 offset, cocos2d::Node* layer);
    virtual ~ControlKeys();
    virtual void update(float dt) override;
        virtual void updateTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event) override;
        virtual void updateTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event) override;
        virtual void updateTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event) override;
        virtual void updateTouchCanceled(cocos2d::Touch* touch,cocos2d::Event* event) override;
    virtual void createEffect() override;
    virtual void removeEffect() override;
private:
    cocos2d::Sprite* button_left;
    cocos2d::Sprite* button_right;
    cocos2d::Sprite* button_jump;
    cocos2d::Sprite* button_attack;
    cocos2d::Sprite* button_interact;
    cocos2d::Vec2 offset;
};
/**
 * @brief
 * To enable for player attack using different movements(for android)
*/
class ControlAttc : public GameUIPhone{
public:
    /*Inherite func*/    
    ControlAttc(Creature* target, cocos2d::Node* layer);
    virtual ~ControlAttc();
    virtual void update(float dt) override;
        virtual void updateTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event) override;
        virtual void updateTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event) override;
        virtual void updateTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event) override;
        virtual void updateTouchCanceled(cocos2d::Touch* touch,cocos2d::Event* event) override;
    virtual void createEffect() override;
    virtual void removeEffect() override;
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
class ControlJump : public GameUIPhone{
public:
    /*Inherite func*/    
    ControlJump(Creature* target, cocos2d::Node* layer);
    virtual ~ControlJump();
    virtual void update(float dt) override;
        virtual void updateTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event) override;
        virtual void updateTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event) override;
        virtual void updateTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event) override;
        virtual void updateTouchCanceled(cocos2d::Touch* touch,cocos2d::Event* event) override;
    virtual void createEffect() override;
    virtual void removeEffect() override;
private:
    bool isRightPlaceForControle;
};