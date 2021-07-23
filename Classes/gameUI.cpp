#include "gameUI.h"
#include "Creature.h"
#include "GameLayer.h"

////////////////////////////////*GameUIPhone class*/////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//Here make some bitfield choise using | or &
GameUIPhone::GameUIPhone(){}
GameUIPhone::~GameUIPhone(){}


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

///////////////////////////////////////////////////////////////*ControlKey class*////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cocos2d::Vec2 ControlKeys::directionPoint(0,0);
DirectionMove ControlKeys::directionMove = DirectionMove::LEFT;
bool  ControlKeys::isMoving = false;

ControlKeys::ControlKeys(cocos2d::Vec2 offset, void* layer){
    this->offset = offset;
    button_left = cocos2d::Sprite::createWithSpriteFrameName("controlB.png");
    cocos2d::Texture2D::TexParams tpar = {
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE
    };
    button_left->getTexture()->setTexParameters(tpar);
    button_left->setScale(3);
    button_left->setPosition(cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width*offset.x,
                                           cocos2d::Director::getInstance()->getVisibleSize().height*offset.y));
    
    static_cast<GameLayer*>(layer)->getChildByName(SceneEntities::ui)->addChild(button_left);

    button_right = cocos2d::Sprite::createWithSpriteFrameName("controlB.png");
    button_right->getTexture()->setTexParameters(tpar);
    button_right->setScale(3);
    button_right->setFlippedX(true);
    button_right->setPosition(cocos2d::Vec2(button_left->getPosition().x + button_right->getBoundingBox().size.width*2,button_left->getPosition().y));
    static_cast<GameLayer*>(layer)->getChildByName(SceneEntities::ui)->addChild(button_right);

    button_jump = cocos2d::Sprite::createWithSpriteFrameName("controlBJump.png");
    button_jump->getTexture()->setTexParameters(tpar);
    button_jump->setScale(3);
    button_jump->setPosition(cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width - button_jump->getBoundingBox().size.width*2,
                                           cocos2d::Director::getInstance()->getVisibleSize().height*offset.y));
    static_cast<GameLayer*>(layer)->getChildByName(SceneEntities::ui)->addChild(button_jump);
}
ControlKeys::~ControlKeys(){}

void ControlKeys::update(float dt,void* Layer){}
void ControlKeys::updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint iterator = 0; iterator < touch.size(); ++iterator){
        if (button_left->getBoundingBox().containsPoint(touch[iterator]->getLocation())){
            isMoving = true;
            directionMove = DirectionMove::LEFT;
            float sp = static_cast<GameLayer*>(Layer)->getPlayer()->getCreatureSpeed();
            directionPoint = cocos2d::Vec2(sp*-1,0);
            static_cast<GameLayer*>(Layer)->getPlayer()->getWeapon()->getSprite()->setFlippedX(true);
        }
        if (button_right->getBoundingBox().containsPoint(touch[iterator]->getLocation())){
            isMoving = true;
            directionMove = DirectionMove::RIGHT;
            float sp = static_cast<GameLayer*>(Layer)->getPlayer()->getCreatureSpeed();
            directionPoint = cocos2d::Vec2(sp,0);
            static_cast<GameLayer*>(Layer)->getPlayer()->getWeapon()->getSprite()->setFlippedX(false);
        }
        if (button_jump->getBoundingBox().containsPoint(touch[iterator]->getLocation())){
            isMoving = true;
            directionMove = DirectionMove::TOP;
            float sp = static_cast<GameLayer*>(Layer)->getPlayer()->getCreatureSpeed();
            
            directionPoint = cocos2d::Vec2(0,150);
        }
    }
}
void ControlKeys::updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    //HERE MAKE SOME INERTION IN AIR
    //if (directionMove == DirectionMove::LEFT || directionMove == DirectionMove::RIGHT)
        //static_cast<GameLayer*>(Layer)->getPlayer()->getCreatureSprite()->getPhysicsBody()->setVelocity(cocos2d::Vec2(0,static_cast<GameLayer*>(Layer)->getPlayer()->getCreatureSprite()->getPhysicsBody()->getVelocity().y));
    isMoving = false;
}
void ControlKeys::updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){}
void ControlKeys::updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    isMoving = false;
}
void ControlKeys::createEffect( void* node){}
void ControlKeys::removeEffect( void* node){}
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
    targetingB = cocos2d::Sprite::createWithSpriteFrameName("targeting.png");
    cocos2d::Size vs = cocos2d::Director::getInstance()->getVisibleSize();
    targetingB->setPosition(cocos2d::Vec2(vs.width - targetingB->getBoundingBox().size.width*3,
                                          vs.height - targetingB->getBoundingBox().size.height*3));
    cocos2d::Texture2D::TexParams tpar = {
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE
    };
    targetingB->getTexture()->setTexParameters(tpar);
    targetingB->setScale(3);                                        
    static_cast<GameLayer*>(layer)->getChildByName(SceneEntities::ui)->addChild(targetingB);
}
ControlTargeting::~ControlTargeting(){}
void ControlTargeting::update(float dt,void* Layer){}
void ControlTargeting::updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){
    for (uint iterator = 0; iterator < touch.size(); ++iterator){
        if (targetingB->getBoundingBox().containsPoint(touch[iterator]->getLocation())){
            if (clickForOpen){
                setTarget(DirectionAttacke::TOP_TO_DOWN,Layer);
                clickForOpen = false;
            }
            else{
                unsetTarget(Layer);
                clickForOpen = true;
            }
            //This was done because loop iterate twice on the same location of touch so no targeting
            touch[iterator]->setTouchInfo(touch[iterator]->getID(),-1000,-1000);
        }
    }
}
void ControlTargeting::updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){}
void ControlTargeting::updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){}
void ControlTargeting::updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer){}
void ControlTargeting::createEffect( void* node){}
void ControlTargeting::removeEffect( void* node){}

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
    static_cast<GameLayer*>(currentLayer)->getChildByName(SceneEntities::ui)->addChild(targetChoser,ZLevel::USER_INTERFACE,"tHead");

    /*This attacks will trigged ability attacke a upper torse*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingUpperTorse.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(cocos2d::Vec2(posOfTargeting.x, posOfTargeting.y - 64));
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&,currentLayer](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        ControlTargeting::target = PartCreatureType::UPPER_TORSE;
        unsetTarget(currentLayer);
    });
    static_cast<GameLayer*>(currentLayer)->getChildByName(SceneEntities::ui)->addChild(targetChoser,ZLevel::USER_INTERFACE,"tUpperT");
    /*This attacks will trigged ability attacke a bottom torse*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingBottomTorse.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(cocos2d::Vec2(posOfTargeting.x,posOfTargeting.y - 128));
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&,currentLayer](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        ControlTargeting::target = PartCreatureType::BUTTOM_TORSE;
        unsetTarget(currentLayer);
    });
    static_cast<GameLayer*>(currentLayer)->getChildByName(SceneEntities::ui)->addChild(targetChoser,ZLevel::USER_INTERFACE,"tBottomT");
    /*This attacks will trigged ability attacke a left leg*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingLeg.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(cocos2d::Vec2(posOfTargeting.x - 32,posOfTargeting.y - 192));
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&,currentLayer](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        ControlTargeting::target = PartCreatureType::LEG_LEFT;
        unsetTarget(currentLayer);
    });
    static_cast<GameLayer*>(currentLayer)->getChildByName(SceneEntities::ui)->addChild(targetChoser,ZLevel::USER_INTERFACE,"tLegL");
    /*This attacks will trigged ability attacke a right leg*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingLeg.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(cocos2d::Vec2(posOfTargeting.x + 32,posOfTargeting.y - 192));
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&,currentLayer](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        ControlTargeting::target = PartCreatureType::LEG_RIGHT;
        unsetTarget(currentLayer);
    });
    static_cast<GameLayer*>(currentLayer)->getChildByName(SceneEntities::ui)->addChild(targetChoser,ZLevel::USER_INTERFACE,"tLegR");
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
    static_cast<GameLayer*>(currentLayer)->getChildByName(SceneEntities::ui)->addChild(targetChoser,ZLevel::USER_INTERFACE,"tHandL");
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
    static_cast<GameLayer*>(currentLayer)->getChildByName(SceneEntities::ui)->addChild(targetChoser,ZLevel::USER_INTERFACE,"tHandR");
    
}
void ControlTargeting::unsetTarget(void* currentLayer){
    static_cast<GameLayer*>(currentLayer)->getChildByName(SceneEntities::ui)->removeChildByName("tHead");
    static_cast<GameLayer*>(currentLayer)->getChildByName(SceneEntities::ui)->removeChildByName("tUpperT");
    static_cast<GameLayer*>(currentLayer)->getChildByName(SceneEntities::ui)->removeChildByName("tHandL");
    static_cast<GameLayer*>(currentLayer)->getChildByName(SceneEntities::ui)->removeChildByName("tHandR");
    static_cast<GameLayer*>(currentLayer)->getChildByName(SceneEntities::ui)->removeChildByName("tBottomT");
    static_cast<GameLayer*>(currentLayer)->getChildByName(SceneEntities::ui)->removeChildByName("tLegR");
    static_cast<GameLayer*>(currentLayer)->getChildByName(SceneEntities::ui)->removeChildByName("tLegL");
}