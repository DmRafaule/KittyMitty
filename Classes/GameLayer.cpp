#include <ui/CocosGUI.h>

#include "GameLayer.h"
#include "gameUI.h"
#include "Creature.h"


std::string LayerChild::player = "player";
std::string LayerChild::ball = "ball";
std::string LayerChild::ball_attacke = "ball_attacke";

cocos2d::Scene* GameLayer::createScene(){
    return GameLayer::create();
}
void GameLayer::menuCloseCallback(cocos2d::Ref* pSender){
    //Close the cocos2d-x game scene and quit the application
    cocos2d::Director::getInstance()->end();
    delete control;
}

bool GameLayer::init(){

    if ( !cocos2d::Scene::init() )
        return false;

    
    Creature player("textures/player.png",BodyMap(1,2,3,4,1),cocos2d::Vec2(400,200),this,LayerChild::player);

    initVars();
    initListeners();
    initUI();


    return true;
}
void GameLayer::initUI(){
    if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
        control = new GameUI(TypeUI::CONTROL_BALL,this);
        control = new GameUI(TypeUI::ATTACKE_BALL,this);
    }
    else if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX){
        control = new GameUI(TypeUI::CONTROL_BALL,this);
        control = new GameUI(TypeUI::ATTACKE_BALL,this);
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
void GameLayer::initVars(){
    touchPosition.resize(2);
}



void GameLayer::update(float dt){
    
    
}

bool GameLayer::touchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    
    return true;
}
void GameLayer::touchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    this->getChildByName(LayerChild::ball)->setPosition(control->getDefaultPosition(TypeUI::CONTROL_BALL));
    this->getChildByName(LayerChild::ball_attacke)->setPosition(control->getDefaultPosition(TypeUI::ATTACKE_BALL));
}
void GameLayer::touchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    for (uint i = 0; i < touch.size(); ++i){
        touchPosition[i] = touch[i]->getLocation();
        if (this->getChildByName(LayerChild::ball)->getBoundingBox().containsPoint(touch[i]->getLocation()))
            this->getChildByName(LayerChild::ball)->setPosition(touchPosition[i]);
        if (this->getChildByName(LayerChild::ball_attacke)->getBoundingBox().containsPoint(touch[i]->getLocation()))
            this->getChildByName(LayerChild::ball_attacke)->setPosition(touchPosition[i]);
    }
}
void GameLayer::touchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){

}