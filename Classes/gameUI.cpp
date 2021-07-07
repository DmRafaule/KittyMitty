#include "gameUI.h"
#include "GameLayer.h"

////////////////////////////////*GameUI class*/////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//Here make some bitfield choise using | or &
GameUI::GameUI(TypeUI type,void* layer){
    
    switch (type){
    case TypeUI::CONTROL_IN_GAMESESSION:{
        cBall = new Control_Ball(layer);
        cAttc = new Control_Attc(layer);
        break;
    }
    
    }
}
GameUI::~GameUI(){

}

void GameUI::update(float dt,TypeUI type, void* Layer){
    switch(type){
        case TypeUI::CONTROL_IN_GAMESESSION:{
            cBall->update(dt,Layer);
            cAttc->update(dt,Layer);
            break;
        }
    }
}
void GameUI::updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint i = 0; i < touch.size(); ++i){
        cBall->updateTouchBegan(touch,event,Layer,i);
        cAttc->updateTouchBegan(touch,event,Layer,i);
    }
}
void GameUI::updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint i = 0; i < touch.size(); ++i){
        cBall->updateTouchEnded(touch,event,Layer,i);
        cAttc->updateTouchEnded(touch,event,Layer,i);
    }
}
void GameUI::updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint i = 0; i < touch.size(); ++i){
        cBall->updateTouchMoved(touch,event,Layer,i);
        cAttc->updateTouchMoved(touch,event,Layer,i);
    }
}
void GameUI::updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint i = 0; i < touch.size(); ++i){
        cBall->updateTouchMoved(touch,event,Layer,i);
        cAttc->updateTouchMoved(touch,event,Layer,i);
    }
}

////////////////////////////////*Controle_Ball class*/////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
/*Init static members*/
std::vector<cocos2d::DrawNode*> GameUI::Control_Ball::pathEffect(5);
cocos2d::Vec2 GameUI::Control_Ball::directionPoint(0,0);
cocos2d::Vec2  GameUI::Control_Ball::touchPoint(0,0);
float GameUI::Control_Ball::part_radius = 0;
float GameUI::Control_Ball::angleDirection = 0;
bool  GameUI::Control_Ball::isMoving = false;

GameUI::Control_Ball::Control_Ball(void* layer){
    isControlBall = false;
    ballDefaultPosition = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width*0.15,
                                        cocos2d::Director::getInstance()->getVisibleSize().height*0.15);
    auto ball = cocos2d::Sprite::create("textures/player.png");
    ball->setScale(1.3f);
    ball->setPosition(ballDefaultPosition);
    static_cast<GameLayer*>(layer)->addChild(ball,Layer::USER_INTERFACE,LayerChild::ball);
}
GameUI::Control_Ball::~Control_Ball(){

}
void GameUI::Control_Ball::update(float dt,void* Layer){

}
void GameUI::Control_Ball::updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer, uint iterator){
    
}
void GameUI::Control_Ball::updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer, uint iterator){
    /*Ended touch for control ball*/
    if (isControlBall &&
        static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball)->getBoundingBox().containsPoint(touch[iterator]->getLocation())){
        isControlBall = false;
        static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball)->setPosition(getBallDefaultPosition());
        Control_Ball::removeEffect(Layer);
    }
}
void GameUI::Control_Ball::updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer, uint iterator){
    touchPoint = touch[iterator]->getLocation();
    if (static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball)->getBoundingBox().containsPoint(touch[iterator]->getLocation())){
        static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball)->setPosition(touch[iterator]->getLocation());
        Control_Ball::createEffect(Layer);
        isControlBall = true;
    }
}
void GameUI::Control_Ball::updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer, uint iterator){
    
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
cocos2d::Vec2 GameUI::Control_Ball::setDirectionPointRelative(cocos2d::Vec2 touchPointEnd,cocos2d::Vec2 touchPointStart){
    return touchPointStart - touchPointEnd;
}
void GameUI::Control_Ball::removeEffect(void* node){
    //Now character not moving
    isMoving = false;
    for (auto &particle : pathEffect){
        static_cast<cocos2d::Node*>(node)->removeChild(particle);
    }
}
void GameUI::Control_Ball::createEffect(void* node){
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


///////////////////////////////////////////////////////////////*Control_Attc class*////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Init static members*/
cocos2d::Vec2  GameUI::Control_Attc::touchPoint(0,0);
cocos2d::Vec2 GameUI::Control_Attc::trembling(0,0);
cocos2d::Vec2  GameUI::Control_Attc::touchPointStart(0,0);
cocos2d::Vec2  GameUI::Control_Attc::touchPointEnd(0,0);
std::vector<cocos2d::DrawNode*> GameUI::Control_Attc::pathEffect(15);

GameUI::Control_Attc::Control_Attc(void* layer){
    isControlAttake = false;
    auto thSpr = cocos2d::Sprite::create("textures/player.png");
    thSpr->setVisible(false);
    thSpr->setScale(1.3f);
    thSpr->setColor(cocos2d::Color3B::ORANGE);
    static_cast<GameLayer*>(layer)->addChild(thSpr,Layer::USER_INTERFACE,LayerChild::ball_attacke);
}
GameUI::Control_Attc::~Control_Attc(){

}
void GameUI::Control_Attc::update(float dt, void* layer){
    
}
void GameUI::Control_Attc::updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer, uint iterator){
    /*On right part of screen*/
    if ( touch[iterator]->getLocation().x > cocos2d::Director::getInstance()->getVisibleSize().width/2){
        isControlAttake = true;
        //Find start position of touch
        touchPointStart = touch[iterator]->getLocation();
    }
    
}
void GameUI::Control_Attc::updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer, uint iterator){
    //If we realy was on right half of screen
    if (isControlAttake){
        isControlAttake = false;
        //Find end position of touch
        touchPointEnd = touchPoint;
        static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball_attacke)->setVisible(false);
        //Calulate attacke direction
        setDirectionAttacke();
    }
}
void GameUI::Control_Attc::updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer, uint iterator){
    if (isControlAttake){
        touchPoint = touch[iterator]->getLocation();
        static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball_attacke)->setPosition(touchPoint);
        static_cast<GameLayer*>(Layer)->getChildByName(LayerChild::ball_attacke)->setVisible(true);
    }
}
void GameUI::Control_Attc::updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer, uint iterator){

}
void GameUI::Control_Attc::setDirectionAttacke(){
    /*Trembling it's a differance which not causes some if statements*/
    trembling = cocos2d::Vec2(touchPointStart.x - touchPointEnd.x,touchPointStart.y - touchPointEnd.y);
    trembling.x < 0 ? trembling.x *= -1 : trembling.x *= 1 ;
    trembling.y < 0 ? trembling.y *= -1 : trembling.y *= 1 ;

    /*Implement middle attacke left/right*/
    if (trembling.y < 50){
        if (touchPointStart.x > touchPointEnd.x){
            direction_of_attacke = DirectionAttacke::RIGHT_TO_LEFT;
            printf("right to left\n");
        }
        else if (touchPointStart.x < touchPointEnd.x){
            direction_of_attacke = DirectionAttacke::LEFT_TO_RIGHT;
            printf("left to right\n");
        }
    }
    if (trembling.x < 50){
        if (touchPointStart.y < touchPointEnd.y){
            direction_of_attacke = DirectionAttacke::DOWN_TO_TOP;
            printf("down to top\n");
        }
        else if (touchPointStart.y > touchPointEnd.y){
            direction_of_attacke = DirectionAttacke::TOP_TO_DOWN;
            printf("top to down\n");
        }
    }
    if (trembling.x >= 50 && trembling.y >= 50){
         if (touchPointStart.x < touchPointEnd.x && touchPointStart.y < touchPointEnd.y){
            direction_of_attacke = DirectionAttacke::BOTTOMLEFT_TO_TOPRIGHT;
            printf("bottomleft to topright\n");
        }
        else if (touchPointStart.x > touchPointEnd.x && touchPointStart.y > touchPointEnd.y){
            direction_of_attacke = DirectionAttacke::TOPRIGHT_TO_BOTTOMLEFT;
            printf("topright to bottomleft\n");
        }
        else if (touchPointStart.x < touchPointEnd.x && touchPointStart.y > touchPointEnd.y){
            direction_of_attacke = DirectionAttacke::TOPLEFT_TO_BOTTOMRIGHT;
            printf("topleft to bottomright\n");
        }
        else {
            direction_of_attacke = DirectionAttacke::BOTTOMRIGHT_TO_TOPLEFT;
            printf("buttomright to topleft\n");
        }
    }
}


void GameUI::Control_Attc::createEffect( void* node){
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
void GameUI::Control_Attc::removeEffect( void* node){
    for (auto &particle : pathEffect){
        static_cast<cocos2d::Node*>(node)->removeChild(particle);
    }
}