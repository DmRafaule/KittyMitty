#include "gameUI.h"
#include "GameLayer.h"

GameUI::GameUI(TypeUI type,void* layer){
    switch (type)
    {
    case TypeUI::CONTROL_BALL:{
        ball = cocos2d::Sprite::create("textures/player.png");
        ball->setScale(0.5f);
        ball->setPosition(cocos2d::Director::getInstance()->getVisibleSize().width*0.10,
                          cocos2d::Director::getInstance()->getVisibleSize().height*0.10);
        static_cast<GameLayer*>(layer)->addChild(ball,0,"ball");
        break;
    }
    }
}
GameUI::~GameUI(){

}