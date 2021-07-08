#include <ui/CocosGUI.h>

#include "GameLayer.h"
#include "gameUI.h"
#include "Creature.h"


std::string LayerChild::player = "player";
std::string LayerChild::ball = "ball";
std::string LayerChild::ball_attacke = "ball_attacke";
std::string LayerChild::enemy = "enemy";

cocos2d::Scene* GameLayer::createScene(){
    return GameLayer::create();
}
void GameLayer::menuCloseCallback(cocos2d::Ref* pSender){
    
    delete cball;
    delete cattc;
    delete player;
    cocos2d::Director::getInstance()->end();
}

bool GameLayer::init(){

    if ( !cocos2d::Scene::init() )
        return false;

    for (int i = 0; i < 4; ++i){
        LayerChild::enemy = "enemy" + std::to_string(i);
        Enemy* e = new Enemy("textures/player.png",BodyMap(1,2,3,4,1,30),cocos2d::Vec2(500+50*i,100+75*i),this,LayerChild::enemy);
        LayerChild::enemy = "";
        enemy.push_back(e);
    }
    player = new Player("textures/player.png",BodyMap(1,2,3,4,1,30),cocos2d::Vec2(400,200),this,LayerChild::player);

    initVarsAndObj();
    initListeners();
    initUI();

  

    return true;
}
void GameLayer::initUI(){
    if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
        
        cball = new ControlBall(this);
        cattc = new ControlAttc(this);
    }
    else if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX){
        
        cball = new ControlBall(this);
        cattc = new ControlAttc(this);
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
    cball->update(dt,this);
    cattc->update(dt,this);
    player->update(dt);
    for (auto &i : enemy)
        i->update(dt);
}

bool GameLayer::touchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    cball->updateTouchBegan(touch,event,this);
    cattc->updateTouchBegan(touch,event,this);
    return true;
}
void GameLayer::touchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    cball->updateTouchEnded(touch,event,this);
    cattc->updateTouchEnded(touch,event,this);
}
void GameLayer::touchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    cball->updateTouchMoved(touch,event,this);
    cattc->updateTouchMoved(touch,event,this);
}
void GameLayer::touchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    cball->updateTouchCanceled(touch,event,this);
    cattc->updateTouchCanceled(touch,event,this);
}