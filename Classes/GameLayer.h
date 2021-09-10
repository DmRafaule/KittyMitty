/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef GAME_LAYER
#define GAME_LAYER

#include <cocos2d.h>
#include "gameUI.h"
#include "World.h"
#include "Player.h"
#include "engMacros.hpp"
#include "engEnums.hpp"


class GameLayer : public cocos2d::Scene{
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(GameLayer);
    void menuCloseCallback(cocos2d::Ref* pSender);

    
    virtual bool init();

    inline Player* getPlayer(){ return player;};
    inline std::vector<Enemy*>* getEnemy(){ return &enemy;};
    inline World* getWorld(){ return world;};//MAYBE NOT NEED
private:
    /**
     * initialize level using tmx file
    */
    void initWorld();
    /**
     * initialize user interface(for phones it will be controle ball, screen action and some buttons) for desktop it will be keyboard and mouse
    */
    void initUI();
    /**
     * initialize all event listeners (for keyboard, touch screen, mouse,physics event )
    */
    void initListeners();
        bool touchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event);
        void touchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event);
        void touchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event);
        void touchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event);
        bool contactBegan(cocos2d::PhysicsContact &contact);
    /* initialize creatures and main herojects*/
    void intCreatures();
    /* Init scenes layers gamesession and ui*/
    void initLayers();
    /* Update all entyties*/
    void updateSlow(float dt);
    void updateFast(float dt);

private:
    //UI
    ShowStats*        shows;
    ControlKeys*      ckeys;
    ControlAttc*      cattc;

    /*Game entyties*/
    std::vector<Enemy*> enemy;
    Player* player;
    World*  world;
};

#endif // GAME_LAYER
