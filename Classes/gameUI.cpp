#include "gameUI.h"
#include "GameLayer.h"

////////////////////////////////*GameUI class*/////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
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

void GameUI::update(float dt,TypeUI type, void* Layer){
    switch(type){
        case TypeUI::CONTROL_BALL:{
            break;
        }
    }
}
void GameUI::updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    
}
void GameUI::updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint i = 0; i < touch.size(); ++i){
        if (isTouch[i]){
            isTouch[i] = false;
            static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball)->setPosition(cocos2d::Director::getInstance()->getVisibleSize().width*0.15,
                                                                                          cocos2d::Director::getInstance()->getVisibleSize().height*0.15);
            Control_Ball::remove(Layer);
        }
    }
}
void GameUI::updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint i = 0; i < touch.size(); ++i){
        touchPosition2[i] = touchPosition[i] = touch[i]->getLocation(); 
        if (static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball)->getBoundingBox().containsPoint(touch[i]->getLocation())){
            static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball)->setPosition(touchPosition[i]);
            Control_Ball::create(Layer);
            isTouch[i] = true;
        }
    }
}
void GameUI::updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){

}

////////////////////////////////*Controle_Ball class*/////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<cocos2d::DrawNode*> GameUI::Control_Ball::pathEffect(5);
cocos2d::Vec2 GameUI::Control_Ball::directionPoint(0,0);
float GameUI::Control_Ball::part_radius = 0;
float GameUI::Control_Ball::angleDirection = 0;
bool  GameUI::Control_Ball::isMoving = false;

GameUI::Control_Ball::Control_Ball(){

}
GameUI::Control_Ball::~Control_Ball(){

}

void GameUI::Control_Ball::setPosPointOnCircle(cocos2d::Vec2& point_destination,cocos2d::Vec2 point_center){
   float cat1 = point_center.x - point_destination.x;
   float cat2 = point_center.y - point_destination.y;
   float angle  = atan2(cat1,cat2) + M_PI;

   angleDirection = setAngleToRadius(angle);

   point_destination.y -= (part_radius * (cos(angle)));
   point_destination.x -= (part_radius * (sin(angle))); 
}
float GameUI::Control_Ball::setAngleToRadius(float angle_radian){
    return angle_radian*(180/M_PI) - 180 < 0 ? angle_radian*(180/M_PI) + 180 : angle_radian*(180/M_PI) - 180; 
}
cocos2d::Vec2 GameUI::Control_Ball::setDirectionPointRelative(cocos2d::Vec2 endPoint,cocos2d::Vec2 startPoint){
    return startPoint - endPoint;
}
void GameUI::Control_Ball::remove(void* node){
    //Now character not moving
    isMoving = false;
    for (auto &particle : pathEffect){
        static_cast<cocos2d::Node*>(node)->removeChild(particle);
    }
}
void GameUI::Control_Ball::create(void* node){
    srand(time(0));
    //Now character moving
    isMoving = true;
    /*Start position of line effect*/
    cocos2d::Vec2 startPos = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width*0.15,
                                         cocos2d::Director::getInstance()->getVisibleSize().height*0.15);
    /*End position of line effect*/
    cocos2d::Vec2 endPos = GameUI::getTouchLocation(0);
    /*Cathetes of triangle where endPos it's point on circle
       and startPos it's center of circle
    */
    /*Set up speed limit*/
    int speedLimit = static_cast<GameLayer*>(node)->getPlayer()->getBodyMap()->speedLimit;
    
    float cat1 = endPos.x - startPos.x;
    float cat2 = endPos.y - startPos.y;
    /*radius(or distance bettween center endPos and startPos*/
    float radius = std::sqrt(pow(cat1,2)+pow(cat2,2));
    /*Steps bettween points on radius*/
    float stepBetPoint = radius / pathEffect.size();
    for (int i = 0; i < pathEffect.size(); ++i){
       /*Clear effect before draw a new one*/
        if (pathEffect[i] != nullptr){
            static_cast<cocos2d::Node*>(node)->removeChild(pathEffect[i]);   
        }
        /*Stop increase our effect if raduis more than 120*/
        if (radius <= 120)
            part_radius = stepBetPoint + i;
        /*Set up positions for each point of radius(part_radius)*/
        setPosPointOnCircle(startPos,endPos);
        /*Set up direcion point(vector) of movement */
        if (i == 3 &&
           (startPos.x - endPos.x) < speedLimit && (startPos.y - endPos.y) < speedLimit &&
           (startPos.x - endPos.x) > speedLimit*(-1) && (startPos.y - endPos.y) > speedLimit*(-1)){
            directionPoint = setDirectionPointRelative(startPos,endPos);
        }
        /*Init drawing stuff(particles)*/
        pathEffect[i] = cocos2d::DrawNode::create();
        pathEffect[i]->drawPoint(startPos,10+i*4.f,cocos2d::Color4F(196.f/255.f,59.f/255.f,121.f/255.f,0.2f + 0.2f*i));
        if (i == 4)
            pathEffect[i]->drawPoint(startPos,40.f,cocos2d::Color4F(255.f/255.f,255.f/255.f,255.f/255.f,1.f));
        static_cast<cocos2d::Node*>(node)->addChild(pathEffect[i],10+i);
    
    }
}