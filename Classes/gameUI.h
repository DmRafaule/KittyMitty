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
    std::vector<cocos2d::Sprite*> logos_sprites;
    bool clickForOpen;
    bool clickForCloseStatistics;

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
private:
    void setDirectionAttacke();
private:

    static cocos2d::Vec2  trembling;                    //This is a  diffrance bettween touchPointStart and toucPointEnd
    static cocos2d::Vec2  touchPointStart;              //Where player start touch on screen
    static cocos2d::Vec2  touchPointEnd;                //Where player stop touch screen
    bool   isRightPlaceForControle;                     //Detect is player finger on right half of screen
};