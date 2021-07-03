#include <ui/CocosGUI.h>

#include "GameLayer.h"
#include "gameUI.h"
#include "Creature.h"

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

    Creature player("textures/player.png",BodyMap(1,2,3,4,1),cocos2d::Vec2(300,400),this,"player");

    initListeners();
    initUI();


    return true;
}
void GameLayer::initUI(){
    if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
        control = new GameUI(TypeUI::CONTROL_BALL,this);
    }
    else if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX){
        control = new GameUI(TypeUI::CONTROL_BALL,this);
    }
}
void GameLayer::initListeners(){
    /*Start update this layer*/
    this->schedule(CC_SCHEDULE_SELECTOR(GameLayer::update),0.1f,CC_REPEAT_FOREVER,0);
}



void GameLayer::update(float dt){
    
    
}
