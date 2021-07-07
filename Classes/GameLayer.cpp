#include <ui/CocosGUI.h>

#include "GameLayer.h"
#include "gameUI.h"
#include "Creature.h"


std::string LayerChild::player = "player";
std::string LayerChild::ball = "ball";
std::string LayerChild::ball_attacke = "ball_attacke";
std::string LayerChild::enemy = "enemy 1";

cocos2d::Scene* GameLayer::createScene(){
    return GameLayer::create();
}
void GameLayer::menuCloseCallback(cocos2d::Ref* pSender){
    //Close the cocos2d-x game scene and quit the application
    delete control;
    delete player;
    cocos2d::Director::getInstance()->end();
}

bool GameLayer::init(){

    if ( !cocos2d::Scene::init() )
        return false;

    player = new Player("textures/player.png",BodyMap(1,2,3,4,1,30),cocos2d::Vec2(400,200),this,LayerChild::player);
    enemy  = new Enemy ("textures/player.png",BodyMap(1,2,3,4,1,30),cocos2d::Vec2(600,200),this,LayerChild::enemy);
    
    initVarsAndObj();
    initListeners();
    initUI();


    return true;
}
void GameLayer::initUI(){
    if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
        control = new GameUI(TypeUI::CONTROL_IN_GAMESESSION,this);
    }
    else if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX){
        control = new GameUI(TypeUI::CONTROL_IN_GAMESESSION,this);
    }
}
void GameLayer::initListeners(){
    /*Init listeners*/
    auto listener = cocos2d::EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(GameLayer::touchBegan,this);
    listener->onTouchesEnded = CC_CALLBACK_2(GameLayer::touchEnded,this);
    listener->onTouchesMoved = CC_CALLBACK_2(GameLayer::touchMoved,this);
    listener->onTouchesCancelled = CC_CALLBACK_2(GameLayer::touchCanceled,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    /*Start update this layer*/
    this->schedule(CC_SCHEDULE_SELECTOR(GameLayer::update),0.1f,CC_REPEAT_FOREVER,0);
}
void GameLayer::initVarsAndObj(){
    visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    
}



void GameLayer::update(float dt){
    control->update(dt,TypeUI::CONTROL_IN_GAMESESSION,this);
    player->update(dt);
    enemy->update(dt);
}

bool GameLayer::touchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    control->updateTouchBegan(touch,event,this);
    return true;
}
void GameLayer::touchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    control->updateTouchEnded(touch,event,this);
}
void GameLayer::touchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    control->updateTouchMoved(touch,event,this);
}
void GameLayer::touchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    control->updateTouchCanceled(touch,event,this);
}