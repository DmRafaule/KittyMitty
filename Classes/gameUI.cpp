#include "gameUI.h"
#include "Creature.h"
#include "GameLayer.h"

////////////////////////////////*GameUI class*/////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//Here make some bitfield choise using | or &
GameUI::GameUI(){}
GameUI::~GameUI(){}


////////////////////////////////*ShowStats class*/////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
ShowStats::ShowStats(void* layer){
    forPlayer   = false;
    forEnemy    = false;
    doubleDelay = 0;
    doubleCount = 0;
    isdouble = false;
}
ShowStats::~ShowStats(){}
void ShowStats::update(float dt,void* Layer){
    if (isdouble){
        doubleDelay+=dt;
        if (doubleDelay > 0.5){
            doubleDelay = 0;
            doubleCount = 0;
            isdouble = false;
        }
    }
}
void ShowStats::updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint iterator = 0; iterator < touch.size(); ++iterator){
        /*This is converting coordinates (expand them to global)*/
        cocos2d::Vec2 pos = static_cast<GameLayer*>(Layer)->getChildByName(SceneEntities::gamesession)->convertTouchToNodeSpace(touch[iterator]);
        /*Double tap for player*/
        if (static_cast<GameLayer*>(Layer)->getChildByName(SceneEntities::gamesession)->getChildByName(SceneEntities::player)->getBoundingBox().containsPoint(pos) && doubleDelay <= 0.5){
            doubleCount++;
            isdouble = true;
            if (doubleCount == 2){
                doubleCount = 0;
                static_cast<GameLayer*>(Layer)->getPlayer()->getStatistics();
            }
        }
        for (Enemy*& enemy : *(static_cast<GameLayer*>(Layer)->getEnemy())){
            /*Double tap for enemies*/
            if (enemy->getCreatureSprite()->getBoundingBox().containsPoint(pos) && doubleDelay <= 0.5){
                doubleCount++;
                isdouble = true;
                if (doubleCount == 2){
                    doubleCount = 0;
                    enemy->getStatistics();
                }
            }
        }
    }
}
void ShowStats::updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){}
void ShowStats::updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){}
void ShowStats::updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){}
void ShowStats::createEffect( void* node){}
void ShowStats::removeEffect( void* node){}
////////////////////////////////*Controle_Ball class*/////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
/*Init static members*/
cocos2d::Vec2 ControlBall::directionPoint(0,0);
float ControlBall::part_radius = 0;
float ControlBall::angleDirection = 0;
bool  ControlBall::isMoving = false;

ControlBall::ControlBall(void* layer){
    isControlBall = false;
    pathEffect.resize(5);
    ballDefaultPosition = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width*0.25,
                                        cocos2d::Director::getInstance()->getVisibleSize().height*0.25);
    auto ball = cocos2d::Sprite::create("textures/player.png");
    ball->setScale(1.3f);
    ball->setPosition(ballDefaultPosition);
    static_cast<GameLayer*>(layer)->addChild(ball,ZLevel::USER_INTERFACE,SceneEntities::ball);
}
ControlBall::~ControlBall(){}
void ControlBall::update(float dt,void* Layer){}
void ControlBall::updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){}
void ControlBall::updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint iterator = 0; iterator < touch.size(); ++iterator){
    /*Ended touch for control ball*/
    if (isControlBall &&
        static_cast<GameLayer*>(Layer)->getChildByName(SceneEntities::ball)->getBoundingBox().containsPoint(touch[iterator]->getLocation())){
        isControlBall = false;
        static_cast<GameLayer*>(Layer)->getChildByName(SceneEntities::ball)->setPosition(getBallDefaultPosition());
        removeEffect(Layer);
    }
    }
}
void ControlBall::updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint iterator = 0; iterator < touch.size(); ++iterator){
    touchPoint = touch[iterator]->getLocation();
    cocos2d::Vec2 pos = static_cast<GameLayer*>(Layer)->convertToNodeSpace(touch[iterator]->getLocation());
    if (static_cast<GameLayer*>(Layer)->getChildByName(SceneEntities::ball)->getBoundingBox().containsPoint(touch[iterator]->getLocation())){
        static_cast<GameLayer*>(Layer)->getChildByName(SceneEntities::ball)->setPosition(touch[iterator]->getLocation());
        createEffect(Layer);
        isControlBall = true;
    }
    }
}
void ControlBall::updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    
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
        static_cast<GameLayer*>(node)->removeChild(particle);
    }
}
void ControlBall::createEffect(void* node){
    srand(time(0));
    //Now character moving
    isMoving = true;
    /*Start position of line effect*/
    cocos2d::Vec2 startPos = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width * 0.25,
                                         cocos2d::Director::getInstance()->getVisibleSize().height  * 0.25);
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
        if (isControlBall)
            pathEffect[i]->clear();
        //static_cast<GameLayer*>(node)->removeChild(pathEffect[i]);   
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
        static_cast<GameLayer*>(node)->addChild(pathEffect[i],10+i);
    
    }
}

///////////////////////////////////////////////////////////////*ControlKey class*////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cocos2d::Vec2 ControlKeys::directionPoint(0,0);
DirectionMove ControlKeys::directionMove = DirectionMove::LEFT;
bool  ControlKeys::isMoving = false;

ControlKeys::ControlKeys(cocos2d::Vec2 offset, void* layer){
    this->offset = offset;
    button_left = cocos2d::ui::Button::create("textures/controlB.png");
    cocos2d::Texture2D::TexParams tpar = {
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE
    };
    button_left->getRendererNormal()->getTexture()->setTexParameters(tpar);
    button_left->setScale(2);
    button_left->setPosition(cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width*offset.x,
                                           cocos2d::Director::getInstance()->getVisibleSize().height*offset.y));
    button_left->addTouchEventListener([&,layer](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType event){
        if (event == cocos2d::ui::Widget::TouchEventType::BEGAN){
            isMoving = true;
            directionMove = DirectionMove::LEFT;
            float sp = static_cast<GameLayer*>(layer)->getPlayer()->getCreatureSpeed();
            directionPoint = cocos2d::Vec2(sp * -1,0);
            static_cast<GameLayer*>(layer)->getPlayer()->getWeapon()->getSprite()->setFlippedX(true);
        }
        else if (event == cocos2d::ui::Widget::TouchEventType::ENDED){
            isMoving = false;
        }
        //For fix bug(auto move)
        else if (event == cocos2d::ui::Widget::TouchEventType::CANCELED){
            isMoving = false;
        }
    });
    
    static_cast<GameLayer*>(layer)->getChildByName("ui")->addChild(button_left);

    button_right = cocos2d::ui::Button::create("textures/controlB.png");
    button_right->getRendererNormal()->getTexture()->setTexParameters(tpar);
    button_right->setScale(2);
    button_right->setFlippedX(true);
    button_right->setPosition(cocos2d::Vec2(button_left->getPosition().x + button_right->getBoundingBox().size.width*2,button_left->getPosition().y));
    button_right->addTouchEventListener([&,layer](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType event){
        if (event == cocos2d::ui::Widget::TouchEventType::BEGAN){
            isMoving = true;
            directionMove = DirectionMove::RIGHT;
            directionPoint = cocos2d::Vec2(static_cast<GameLayer*>(layer)->getPlayer()->getCreatureSpeed(),0);
            static_cast<GameLayer*>(layer)->getPlayer()->getWeapon()->getSprite()->setFlippedX(false);
        }
        else if (event == cocos2d::ui::Widget::TouchEventType::ENDED){
            isMoving = false;
        }
        else if (event == cocos2d::ui::Widget::TouchEventType::CANCELED){
            isMoving = false;
        }
    });
    
    static_cast<GameLayer*>(layer)->getChildByName("ui")->addChild(button_right);
}
ControlKeys::~ControlKeys(){}

///////////////////////////////////////////////////////////////*ControlAttc class*////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Init static members*/
cocos2d::Vec2 ControlAttc::trembling(0,0);
cocos2d::Vec2  ControlAttc::touchPointStart(0,0);
cocos2d::Vec2  ControlAttc::touchPointEnd(0,0);
DirectionAttacke ControlAttc::direction_of_attacke = DirectionAttacke::BOTTOMLEFT_TO_TOPRIGHT;
bool  ControlAttc::isAttacke(false);

ControlAttc::ControlAttc(void* layer){
    isRightPlaceForControle = false;
}
ControlAttc::~ControlAttc(){}
void ControlAttc::update(float dt, void* layer){}
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
        //Calulate attacke direction Only if lenght of touch line more than ...
        if (std::abs(touchPointEnd.x - touchPointStart.x) > cocos2d::Director::getInstance()->getVisibleSize().width/6 ||
            std::abs(touchPointEnd.y - touchPointStart.y) > cocos2d::Director::getInstance()->getVisibleSize().height/4){
            setDirectionAttacke();
        }
    }
    }
}
void ControlAttc::updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint iterator = 0; iterator < touch.size(); ++iterator){
    if (isRightPlaceForControle){
        touchPoint = touch[iterator]->getLocation();
    }
    }
}
void ControlAttc::updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){}
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


void ControlAttc::createEffect( void* node){}
void ControlAttc::removeEffect( void* node){}


///////////////////////////////////////////////////////////////*Class ControlTargeting*////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Init static members*/
PartCreatureType ControlTargeting::target = PartCreatureType::HEAD;
cocos2d::Vec2 ControlTargeting::offset = cocos2d::Vec2(0.1,0.9);

ControlTargeting::ControlTargeting(void* layer){
    clickForOpen = true;
    cocos2d::ui::Button* targetingB = cocos2d::ui::Button::create("textures/targeting.png");
    cocos2d::Size vs = cocos2d::Director::getInstance()->getVisibleSize();
    targetingB->setPosition(cocos2d::Vec2(vs.width - targetingB->getBoundingBox().size.width,
                                          vs.height - targetingB->getBoundingBox().size.height));
    cocos2d::Texture2D::TexParams tpar = {
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE
    };
    targetingB->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetingB->setScale(2);
    targetingB->addTouchEventListener([&,layer](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
            if (type == cocos2d::ui::Widget::TouchEventType::BEGAN){
                if (clickForOpen){
                    setTarget(DirectionAttacke::TOP_TO_DOWN,layer);
                    clickForOpen = false;
                }
                else {
                    unsetTarget(layer);
                    clickForOpen = true;
                }
            }
            else if (type == cocos2d::ui::Widget::TouchEventType::ENDED){

            }
        });                                          
    static_cast<GameLayer*>(layer)->getChildByName("ui")->addChild(targetingB);
}
ControlTargeting::~ControlTargeting(){}

void ControlTargeting::setTarget(DirectionAttacke direction, void* currentLayer){
    /*Remove our buttons before drew new one*/
    unsetTarget(currentLayer);
    cocos2d::Size vs = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 posOfTargeting = cocos2d::Vec2(vs.width*offset.x,vs.height*offset.y);
    cocos2d::ui::Button* targetChoser;
    /*To avoid blur effect*/
    cocos2d::Texture2D::TexParams tpar = {
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE
    };
    /*This attacks will trigged ability attacke a head*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingHead.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(posOfTargeting);
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&,currentLayer](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN){
            ControlTargeting::target = PartCreatureType::HEAD;
            unsetTarget(currentLayer);
        }
    });
    static_cast<GameLayer*>(currentLayer)->getChildByName("ui")->addChild(targetChoser,ZLevel::USER_INTERFACE,"tHead");

    /*This attacks will trigged ability attacke a upper torse*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingUpperTorse.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(cocos2d::Vec2(posOfTargeting.x, posOfTargeting.y - 64));
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&,currentLayer](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        ControlTargeting::target = PartCreatureType::UPPER_TORSE;
        unsetTarget(currentLayer);
    });
    static_cast<GameLayer*>(currentLayer)->getChildByName("ui")->addChild(targetChoser,ZLevel::USER_INTERFACE,"tUpperT");
    /*This attacks will trigged ability attacke a bottom torse*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingBottomTorse.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(cocos2d::Vec2(posOfTargeting.x,posOfTargeting.y - 128));
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&,currentLayer](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        ControlTargeting::target = PartCreatureType::BUTTOM_TORSE;
        unsetTarget(currentLayer);
    });
    static_cast<GameLayer*>(currentLayer)->getChildByName("ui")->addChild(targetChoser,ZLevel::USER_INTERFACE,"tBottomT");
    /*This attacks will trigged ability attacke a left leg*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingLeg.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(cocos2d::Vec2(posOfTargeting.x - 32,posOfTargeting.y - 192));
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&,currentLayer](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        ControlTargeting::target = PartCreatureType::LEG_LEFT;
        unsetTarget(currentLayer);
    });
    static_cast<GameLayer*>(currentLayer)->getChildByName("ui")->addChild(targetChoser,ZLevel::USER_INTERFACE,"tLegL");
    /*This attacks will trigged ability attacke a right leg*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingLeg.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(cocos2d::Vec2(posOfTargeting.x + 32,posOfTargeting.y - 192));
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&,currentLayer](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        ControlTargeting::target = PartCreatureType::LEG_RIGHT;
        unsetTarget(currentLayer);
    });
    static_cast<GameLayer*>(currentLayer)->getChildByName("ui")->addChild(targetChoser,ZLevel::USER_INTERFACE,"tLegR");
    /*This attacks will trigged ability attacke a left hand*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingHand.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(cocos2d::Vec2(posOfTargeting.x - 64,
                                            posOfTargeting.y - 64));
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&,currentLayer](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        ControlTargeting::target = PartCreatureType::HAND_LEFT;
        unsetTarget(currentLayer);
    });
    static_cast<GameLayer*>(currentLayer)->getChildByName("ui")->addChild(targetChoser,ZLevel::USER_INTERFACE,"tHandL");
    /*This attacks will trigged ability attacke a right hand*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingHand.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(cocos2d::Vec2(posOfTargeting.x + 64,
                                            posOfTargeting.y - 64));
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&,currentLayer](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        ControlTargeting::target = PartCreatureType::HAND_RIGHT;
        unsetTarget(currentLayer);
    });
    static_cast<GameLayer*>(currentLayer)->getChildByName("ui")->addChild(targetChoser,ZLevel::USER_INTERFACE,"tHandR");
    
}
void ControlTargeting::unsetTarget(void* currentLayer){
    static_cast<GameLayer*>(currentLayer)->getChildByName("ui")->removeChildByName("tHead");
    static_cast<GameLayer*>(currentLayer)->getChildByName("ui")->removeChildByName("tUpperT");
    static_cast<GameLayer*>(currentLayer)->getChildByName("ui")->removeChildByName("tHandL");
    static_cast<GameLayer*>(currentLayer)->getChildByName("ui")->removeChildByName("tHandR");
    static_cast<GameLayer*>(currentLayer)->getChildByName("ui")->removeChildByName("tBottomT");
    static_cast<GameLayer*>(currentLayer)->getChildByName("ui")->removeChildByName("tLegR");
    static_cast<GameLayer*>(currentLayer)->getChildByName("ui")->removeChildByName("tLegL");
}