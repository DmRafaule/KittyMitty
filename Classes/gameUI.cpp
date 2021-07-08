#include "gameUI.h"
#include "GameLayer.h"

////////////////////////////////*GameUI class*/////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//Here make some bitfield choise using | or &
GameUI::GameUI(){

}
GameUI::~GameUI(){

}


////////////////////////////////*Controle_Ball class*/////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
/*Init static members*/
cocos2d::Vec2 ControlBall::directionPoint(0,0);
float ControlBall::part_radius = 0;
float ControlBall::angleDirection = 0;
bool  ControlBall::isMoving = false;

ControlBall::ControlBall(void* layer){
    pathEffect.resize(5);
    isControlBall = false;
    ballDefaultPosition = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width*0.15,
                                        cocos2d::Director::getInstance()->getVisibleSize().height*0.15);
    auto ball = cocos2d::Sprite::create("textures/player.png");
    ball->setScale(1.3f);
    ball->setPosition(ballDefaultPosition);
    static_cast<GameLayer*>(layer)->addChild(ball,Layer::USER_INTERFACE,LayerChild::ball);
}
ControlBall::~ControlBall(){

}
void ControlBall::update(float dt,void* Layer){

}
void ControlBall::updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint iterator = 0; iterator < touch.size(); ++iterator){}
}
void ControlBall::updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint iterator = 0; iterator < touch.size(); ++iterator){
    /*Ended touch for control ball*/
    if (isControlBall &&
        static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball)->getBoundingBox().containsPoint(touch[iterator]->getLocation())){
        isControlBall = false;
        static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball)->setPosition(getBallDefaultPosition());
        removeEffect(Layer);
    }
    }
}
void ControlBall::updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint iterator = 0; iterator < touch.size(); ++iterator){
    touchPoint = touch[iterator]->getLocation();
    if (static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball)->getBoundingBox().containsPoint(touch[iterator]->getLocation())){
        static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball)->setPosition(touch[iterator]->getLocation());
        createEffect(Layer);
        isControlBall = true;
    }
    }
}
void ControlBall::updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint iterator = 0; iterator < touch.size(); ++iterator){}
}


void ControlBall::setPosPointOnCircle(cocos2d::Vec2& point_destination,cocos2d::Vec2 point_center){
   float cat1 = point_center.x - point_destination.x;
   float cat2 = point_center.y - point_destination.y;
   float angle  = atan2(cat1,cat2) + M_PI;

   angleDirection = setAngleToRadius(angle);

   point_destination.y -= (part_radius * (cos(angle)));
   point_destination.x -= (part_radius * (sin(angle))); 
}
float ControlBall::setAngleToRadius(float angle_radian){
    return angle_radian*(180/M_PI) - 180 < 0 ? angle_radian*(180/M_PI) + 180 : angle_radian*(180/M_PI) - 180; 
}
cocos2d::Vec2 ControlBall::setDirectionPointRelative(cocos2d::Vec2 touchPointEnd,cocos2d::Vec2 touchPointStart){
    return touchPointStart - touchPointEnd;
}
void ControlBall::removeEffect(void* node){
    //Now character not moving
    isMoving = false;
    for (auto &particle : pathEffect){
        static_cast<cocos2d::Node*>(node)->removeChild(particle);
    }
}
void ControlBall::createEffect(void* node){
    srand(time(0));
    //Now character moving
    isMoving = true;
    /*Start position of line effect*/
    cocos2d::Vec2 startPos = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width*0.15,
                                         cocos2d::Director::getInstance()->getVisibleSize().height*0.15);
    /*End position of line effect*/
    cocos2d::Vec2 endPos = touchPoint;
    /*Cathetes of triangle where endPos it's point on circle
       and startPos it's center of circle
    */
    /*Set up speed limit*/
    int speedLimit = static_cast<GameLayer*>(node)->getPlayer()->getCreatureSpeed();
    
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


///////////////////////////////////////////////////////////////*Control_Attc class*////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Init static members*/
cocos2d::Vec2 ControlAttc::trembling(0,0);
cocos2d::Vec2  ControlAttc::touchPointStart(0,0);
cocos2d::Vec2  ControlAttc::touchPointEnd(0,0);
DirectionAttacke ControlAttc::direction_of_attacke = DirectionAttacke::BOTTOMLEFT_TO_TOPRIGHT;
bool  ControlAttc::isAttacke(false);

ControlAttc::ControlAttc(void* layer){
    pathEffect.resize(5);
    isRightPlaceForControle = false;

    auto thSpr = cocos2d::Sprite::create("textures/player.png");
    thSpr->setVisible(false);
    thSpr->setScale(1.3f);
    thSpr->setColor(cocos2d::Color3B::ORANGE);
    static_cast<GameLayer*>(layer)->addChild(thSpr,Layer::USER_INTERFACE,LayerChild::ball_attacke);
}
ControlAttc::~ControlAttc(){

}
void ControlAttc::update(float dt, void* layer){
    
}
void ControlAttc::updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint iterator = 0; iterator < touch.size(); ++iterator){
    /*On right part of screen*/
    if ( touch[iterator]->getLocation().x > cocos2d::Director::getInstance()->getVisibleSize().width/2){
        isRightPlaceForControle = true;
        //Find start position of touch
        touchPointStart = touch[iterator]->getLocation();
    }
    }
}
void ControlAttc::updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint iterator = 0; iterator < touch.size(); ++iterator){
    //If we realy was on right half of screen
    if (isRightPlaceForControle){
        isRightPlaceForControle = false;
        //Find end position of touch
        touchPointEnd = touchPoint;
        static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball_attacke)->setVisible(false);
        //Calulate attacke direction
        setDirectionAttacke();
    }
    }
}
void ControlAttc::updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint iterator = 0; iterator < touch.size(); ++iterator){
    if (isRightPlaceForControle){
        touchPoint = touch[iterator]->getLocation();
        static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball_attacke)->setPosition(touchPoint);
        static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball_attacke)->setVisible(true);
    }
    }
}
void ControlAttc::updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint iterator = 0; iterator < touch.size(); ++iterator){}
}
void ControlAttc::setDirectionAttacke(){
    /*Trembling it's a differance which not causes some if statements*/
    trembling = cocos2d::Vec2(touchPointStart.x - touchPointEnd.x,touchPointStart.y - touchPointEnd.y);
    trembling.x < 0 ? trembling.x *= -1 : trembling.x *= 1 ;
    trembling.y < 0 ? trembling.y *= -1 : trembling.y *= 1 ;

    /*Implement middle attacke left/right*/
    if (trembling.y < 50){
        if (touchPointStart.x > touchPointEnd.x)
            direction_of_attacke = DirectionAttacke::RIGHT_TO_LEFT;
        else if (touchPointStart.x < touchPointEnd.x)
            direction_of_attacke = DirectionAttacke::LEFT_TO_RIGHT;
    }
    if (trembling.x < 50){
        if (touchPointStart.y < touchPointEnd.y)
            direction_of_attacke = DirectionAttacke::DOWN_TO_TOP;
        else if (touchPointStart.y > touchPointEnd.y)
            direction_of_attacke = DirectionAttacke::TOP_TO_DOWN;
    }
    if (trembling.x >= 50 && trembling.y >= 50){
         if (touchPointStart.x < touchPointEnd.x && touchPointStart.y < touchPointEnd.y)
            direction_of_attacke = DirectionAttacke::BOTTOMLEFT_TO_TOPRIGHT;
        else if (touchPointStart.x > touchPointEnd.x && touchPointStart.y > touchPointEnd.y)
            direction_of_attacke = DirectionAttacke::TOPRIGHT_TO_BOTTOMLEFT;
        else if (touchPointStart.x < touchPointEnd.x && touchPointStart.y > touchPointEnd.y)
            direction_of_attacke = DirectionAttacke::TOPLEFT_TO_BOTTOMRIGHT;
        else 
            direction_of_attacke = DirectionAttacke::BOTTOMRIGHT_TO_TOPLEFT;
    }
    isAttacke = true;
}


void ControlAttc::createEffect( void* node){
    //float lenght_path = std::sqrt(pow(trembling.x,2)+pow(trembling.y,2));
    //float step_path = lenght_path / pathEffect.size();
    //for (uint i = 0; i < pathEffect.size(); ++i){
    //    cocos2d::Vec2 particlePos = cocos2d::Vec2(touchPointStart.x + step_path * i,touchPointStart.y + step_path * i);
    //    /*Clear effect before draw a new one*/
    //    if (pathEffect[i] != nullptr){
    //        static_cast<cocos2d::Node*>(node)->removeChild(pathEffect[i]);   
    //    }
//
    //    pathEffect[i] = cocos2d::DrawNode::create();
    //    pathEffect[i]->drawLine(touchPointStart,touchPointEnd,cocos2d::Color4F::BLUE);
    //    static_cast<cocos2d::Node*>(node)->addChild(pathEffect[i],10+i);
    //}
}
void ControlAttc::removeEffect( void* node){
    for (auto &particle : pathEffect){
        static_cast<cocos2d::Node*>(node)->removeChild(particle);
    }
}