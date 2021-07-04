#include "gameUI.h"
#include "GameLayer.h"

GameUI::GameUI(TypeUI type,void* layer){
    switch (type)
    {
    case TypeUI::CONTROL_BALL:{
        ballDefaultPosition = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width*0.10,
                                            cocos2d::Director::getInstance()->getVisibleSize().height*0.10);
        ball = cocos2d::Sprite::create("textures/player.png");
        ball->setScale(1.3f);
        ball->setPosition(ballDefaultPosition);
        static_cast<GameLayer*>(layer)->addChild(ball,0,LayerChild::ball);
        break;
    }
    case TypeUI::ATTACKE_BALL:{
        ball_attackeDefaultPosition = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width*0.90,
                                                    cocos2d::Director::getInstance()->getVisibleSize().height*0.10);
        ball_attacke = cocos2d::Sprite::create("textures/player.png");
        ball_attacke->setScale(1.3f);
        ball_attacke->setPosition(ball_attackeDefaultPosition);
        static_cast<GameLayer*>(layer)->addChild(ball_attacke,0,LayerChild::ball_attacke);
        break;
    }
    }
}
GameUI::~GameUI(){

}

cocos2d::Vec2 GameUI::getDefaultPosition(TypeUI type){
    switch(type){
        case (TypeUI::CONTROL_BALL):{
            return cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width*0.10,
                                            cocos2d::Director::getInstance()->getVisibleSize().height*0.10);
        }
        case (TypeUI::ATTACKE_BALL):{
            return ball_attackeDefaultPosition;
        }
    }
}