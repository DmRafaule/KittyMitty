#include "gameUI.h"
#include "GameLayer.h"
#include "ParticleSystem.h"

std::vector<cocos2d::Vec2> GameUI::touchPosition2(2);

GameUI::GameUI(TypeUI type,void* layer){
    touchPosition.resize(2);
    isTouch.resize(2);
    isTouch[0] = false;
    isTouch[1] = false;
    
    switch (type){
    case TypeUI::CONTROL_BALL:{
        ballDefaultPosition = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width*0.15,
                                            cocos2d::Director::getInstance()->getVisibleSize().height*0.15);
        ball = cocos2d::Sprite::create("textures/player.png");
        ball->setScale(1.3f);
        ball->setPosition(ballDefaultPosition);
        static_cast<GameLayer*>(layer)->addChild(ball,Layer::USER_INTERFACE,LayerChild::ball);

        break;
    }
    
    }
}
GameUI::~GameUI(){

}

void GameUI::update(float dt,void* Layer){
    
}
void GameUI::updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    
}
void GameUI::updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint i = 0; i < touch.size(); ++i){
        if (isTouch[i]){
            isTouch[i] = false;
            static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball)->setPosition(cocos2d::Director::getInstance()->getVisibleSize().width*0.15,
                                                                                          cocos2d::Director::getInstance()->getVisibleSize().height*0.15);
            ParticleSystem::removeEffect(Effects::LINE_PATH,Layer);
        }
    }
}
void GameUI::updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint i = 0; i < touch.size(); ++i){
        touchPosition2[i] = touchPosition[i] = touch[i]->getLocation(); 
        if (static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball)->getBoundingBox().containsPoint(touch[i]->getLocation())){
            static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball)->setPosition(touchPosition[i]);
            ParticleSystem::createEffect(Effects::LINE_PATH,Layer);
            isTouch[i] = true;
        }
    }
}