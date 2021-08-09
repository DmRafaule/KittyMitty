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
ShowStats::ShowStats(Creature* target,std::vector<Enemy*>* targetE, cocos2d::Node* layer){
    this->creature  = target;
    this->creatureE = targetE;
    this->currentLayer = layer;
    this->clickForOpen = false;
}
ShowStats::~ShowStats(){}
void ShowStats::update(float dt){}
void ShowStats::updateTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
     
        /*This is converting coordinates (expand them to global)*/
        if (!clickForOpen){
            clickForOpen = true;
            cocos2d::Vec2 pos = currentLayer->convertTouchToNodeSpace(touch);
            /*Double tap for player*/
            if (creature->getCreatureSprite()->getBoundingBox().containsPoint(pos)){
                    creature->getStatistics();
            }
            for (Enemy*& enemy : *(creatureE)){
                /*Double tap for enemies*/
                if (enemy->getCreatureSprite()->getBoundingBox().containsPoint(pos)){
                    enemy->getStatistics();
                }
            }
        }
        else{
            clickForOpen = false;
        }
}
void ShowStats::updateTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event){}
void ShowStats::updateTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event){}
void ShowStats::updateTouchCanceled(cocos2d::Touch* touch,cocos2d::Event* event){}
void ShowStats::createEffect(){}
void ShowStats::removeEffect(){}

///////////////////////////////////////////////////////////////*ControlKey class*////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ControlKeys::ControlKeys(Creature* target, cocos2d::Vec2 offset, cocos2d::Node* layer){
    this->offset = offset;
    this->creature = target;
    this->currentLayer = layer;
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
    
    currentLayer->addChild(button_left);

    button_right = cocos2d::Sprite::createWithSpriteFrameName("controlB.png");
    button_right->getTexture()->setTexParameters(tpar);
    button_right->setScale(3);
    button_right->setFlippedX(true);
    button_right->setPosition(cocos2d::Vec2(button_left->getPosition().x + button_right->getBoundingBox().size.width*2,button_left->getPosition().y));
    currentLayer->addChild(button_right);

    button_jump = cocos2d::Sprite::createWithSpriteFrameName("controlBJump.png");
    button_jump->getTexture()->setTexParameters(tpar);
    button_jump->setScale(3);
    button_jump->setPosition(cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width - button_jump->getBoundingBox().size.width,
                                           cocos2d::Director::getInstance()->getVisibleSize().height*offset.y));
    currentLayer->addChild(button_jump);

    button_attack = cocos2d::Sprite::createWithSpriteFrameName("attackB.png");
    button_attack->getTexture()->setTexParameters(tpar);
    button_attack->setScale(3);
    button_attack->setPosition(cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width - button_attack->getBoundingBox().size.width*2.5,
                                           cocos2d::Director::getInstance()->getVisibleSize().height*offset.y));
    currentLayer->addChild(button_attack);

    button_interact = cocos2d::Sprite::createWithSpriteFrameName("interactB.png");
    button_interact->getTexture()->setTexParameters(tpar);
    button_interact->setScale(3);
    button_interact->setPosition(cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width - button_interact->getBoundingBox().size.width,
                                               cocos2d::Director::getInstance()->getVisibleSize().height*offset.y + button_interact->getBoundingBox().size.height * 1.5));
    currentLayer->addChild(button_interact);
}
ControlKeys::~ControlKeys(){}

void ControlKeys::update(float dt){
    
}
void ControlKeys::updateTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
     
    if (button_left->getBoundingBox().containsPoint(touch->getLocation())){
        if (creature->getCreatureInfo()->state == CreatureInfo::State::IDLE ||
            creature->getCreatureInfo()->state == CreatureInfo::State::STAND_UP)
            creature->setCreatureState(CreatureInfo::State::START_RUN);
        else if (creature->getCreatureInfo()->state == CreatureInfo::State::IN_FALL ||
                 creature->getCreatureInfo()->state == CreatureInfo::State::IN_JUMP)
            creature->setCreatureState(CreatureInfo::State::SOARING);
        else if (creature->getCreatureInfo()->state == CreatureInfo::State::ON_STEPS)
            creature->setCreatureState(CreatureInfo::State::MOVE_BY_STEPS);
        creature->setCreatureInfo()->dmove = CreatureInfo::DMove::LEFT;
    }
    if (button_right->getBoundingBox().containsPoint(touch->getLocation())){
        if (creature->getCreatureInfo()->state == CreatureInfo::State::IDLE ||
            creature->getCreatureInfo()->state == CreatureInfo::State::STAND_UP)
            creature->setCreatureState(CreatureInfo::State::START_RUN);
        else if (creature->getCreatureInfo()->state == CreatureInfo::State::IN_FALL ||
                 creature->getCreatureInfo()->state == CreatureInfo::State::IN_JUMP)
            creature->setCreatureState(CreatureInfo::State::SOARING);
        else if (creature->getCreatureInfo()->state == CreatureInfo::State::ON_STEPS)
            creature->setCreatureState(CreatureInfo::State::MOVE_BY_STEPS);
        creature->setCreatureInfo()->dmove = CreatureInfo::DMove::RIGHT;
    }
    //Make jump if player have stamina and he is do not make more than 2 jumps
    if (button_jump->getBoundingBox().containsPoint(touch->getLocation()) &&
        creature->getCreatureInfo()->characteristic.stamina >= 5 && 
        creature->getCreatureInfo()->characteristic.current_jump_ability_num <= creature->getCreatureInfo()->characteristic.jump_ability){
        
        if (creature->getCreatureInfo()->state == CreatureInfo::State::ON_WALL){
            creature->setCreatureState(CreatureInfo::State::JUMP_FROM_WALL);
            
        }
        else if (creature->getCreatureInfo()->state != CreatureInfo::State::JUMP_FROM_WALL){
            creature->setCreatureState(CreatureInfo::State::IN_JUMP);
            creature->setCreatureInfo()->dmove = CreatureInfo::DMove::TOP;
        }
    }
    if (button_interact->getBoundingBox().containsPoint(touch->getLocation())){
        creature->setCreatureState(CreatureInfo::State::INTERACTING);
    }
    if (button_attack->getBoundingBox().containsPoint(touch->getLocation())){

    }
    
}
void ControlKeys::updateTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event){ 
    if (button_left->getBoundingBox().containsPoint(touch->getLocation())){
        if (creature->getCreatureInfo()->state == CreatureInfo::State::RUNNING)
            creature->setCreatureState(CreatureInfo::State::BRACKING);
        else if (creature->getCreatureInfo()->state == CreatureInfo::State::MOVE_BY_STEPS)
            creature->setCreatureState(CreatureInfo::State::ON_STEPS);
    }
    if (button_right->getBoundingBox().containsPoint(touch->getLocation())){
        if (creature->getCreatureInfo()->state == CreatureInfo::State::RUNNING)
            creature->setCreatureState(CreatureInfo::State::BRACKING);
        else if (creature->getCreatureInfo()->state == CreatureInfo::State::MOVE_BY_STEPS)
            creature->setCreatureState(CreatureInfo::State::ON_STEPS);
    }
    if (button_jump->getBoundingBox().containsPoint(touch->getLocation())){
        
    }
    if (button_interact->getBoundingBox().containsPoint(touch->getLocation())){
        
    }
    if (button_attack->getBoundingBox().containsPoint(touch->getLocation())){
        
    }
}
void ControlKeys::updateTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event){}
void ControlKeys::updateTouchCanceled(cocos2d::Touch* touch,cocos2d::Event* event){}

void ControlKeys::createEffect(){}
void ControlKeys::removeEffect(){}

///////////////////////////////////////////////////////////////*ControlAttc class*////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Init static members*/
cocos2d::Vec2 ControlAttc::trembling(0,0);
cocos2d::Vec2  ControlAttc::touchPointStart(0,0);
cocos2d::Vec2  ControlAttc::touchPointEnd(0,0);
DirectionAttacke ControlAttc::direction_of_attacke = DirectionAttacke::BOTTOMLEFT_TO_TOPRIGHT;
bool  ControlAttc::isAttacke(false);

ControlAttc::ControlAttc(Creature* target, cocos2d::Node* layer){
    this->creature = target;
    this->currentLayer = layer;
    this->isRightPlaceForControle = false;
}
ControlAttc::~ControlAttc(){}
void ControlAttc::update(float dt){}
void ControlAttc::updateTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
     
    /*On right part of screen*/
    if ( touch->getLocation().x > cocos2d::Director::getInstance()->getVisibleSize().width/2){
        isRightPlaceForControle = true;
        //Find start position of touch
        touchPointStart = touch->getLocation();
    }
}
void ControlAttc::updateTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event){
     
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
void ControlAttc::updateTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event){
     
    if (isRightPlaceForControle){
        touchPoint = touch->getLocation();
    }
}
void ControlAttc::updateTouchCanceled(cocos2d::Touch* touch,cocos2d::Event* event){}
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


void ControlAttc::createEffect(){}
void ControlAttc::removeEffect(){}


///////////////////////////////////////////////////////////////*Class ControlTargeting*////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Init static members*/
PartCreatureType ControlTargeting::target = PartCreatureType::HEAD;
cocos2d::Vec2 ControlTargeting::offset = cocos2d::Vec2(0.1,0.9);

ControlTargeting::ControlTargeting(Creature* target, cocos2d::Node* layer){
    this->creature = target;
    this->currentLayer = layer;
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
    currentLayer->addChild(targetingB);
}
ControlTargeting::~ControlTargeting(){}
void ControlTargeting::update(float dt){}
void ControlTargeting::updateTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
        if (targetingB->getBoundingBox().containsPoint(touch->getLocation())){
            if (clickForOpen){
                setTarget(DirectionAttacke::TOP_TO_DOWN);
                clickForOpen = false;
            }
            else{
                unsetTarget();
                clickForOpen = true;
            }
            //This was done because loop iterate twice on the same location of touch so no targeting
            touch->setTouchInfo(touch->getID(),-1000,-1000);
        }
}
void ControlTargeting::updateTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event){}
void ControlTargeting::updateTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event){}
void ControlTargeting::updateTouchCanceled(cocos2d::Touch* touch,cocos2d::Event* event){}
void ControlTargeting::createEffect(){}
void ControlTargeting::removeEffect(){}

void ControlTargeting::setTarget(DirectionAttacke direction){
    /*Remove our buttons before drew new one*/
    unsetTarget();
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
    targetChoser->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN){
            ControlTargeting::target = PartCreatureType::HEAD;
            unsetTarget();
        }
    });
    currentLayer->addChild(targetChoser,ZLevel::USER_INTERFACE,"tHead");

    /*This attacks will trigged ability attacke a upper torse*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingUpperTorse.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(cocos2d::Vec2(posOfTargeting.x, posOfTargeting.y - 64));
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        ControlTargeting::target = PartCreatureType::UPPER_TORSE;
        unsetTarget();
    });
    currentLayer->addChild(targetChoser,ZLevel::USER_INTERFACE,"tUpperT");
    /*This attacks will trigged ability attacke a bottom torse*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingBottomTorse.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(cocos2d::Vec2(posOfTargeting.x,posOfTargeting.y - 128));
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        ControlTargeting::target = PartCreatureType::BUTTOM_TORSE;
        unsetTarget();
    });
    currentLayer->addChild(targetChoser,ZLevel::USER_INTERFACE,"tBottomT");
    /*This attacks will trigged ability attacke a left leg*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingLeg.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(cocos2d::Vec2(posOfTargeting.x - 32,posOfTargeting.y - 192));
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        ControlTargeting::target = PartCreatureType::LEG_LEFT;
        unsetTarget();
    });
    currentLayer->addChild(targetChoser,ZLevel::USER_INTERFACE,"tLegL");
    /*This attacks will trigged ability attacke a right leg*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingLeg.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(cocos2d::Vec2(posOfTargeting.x + 32,posOfTargeting.y - 192));
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        ControlTargeting::target = PartCreatureType::LEG_RIGHT;
        unsetTarget();
    });
    currentLayer->addChild(targetChoser,ZLevel::USER_INTERFACE,"tLegR");
    /*This attacks will trigged ability attacke a left hand*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingHand.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(cocos2d::Vec2(posOfTargeting.x - 64,
                                            posOfTargeting.y - 64));
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        ControlTargeting::target = PartCreatureType::HAND_LEFT;
        unsetTarget();
    });
    currentLayer->addChild(targetChoser,ZLevel::USER_INTERFACE,"tHandL");
    /*This attacks will trigged ability attacke a right hand*/
    targetChoser = cocos2d::ui::Button::create("textures/targetingHand.png");
    targetChoser->getRendererNormal()->getTexture()->setTexParameters(tpar);
    targetChoser->setPosition(cocos2d::Vec2(posOfTargeting.x + 64,
                                            posOfTargeting.y - 64));
    targetChoser->setScale(5);
    targetChoser->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        ControlTargeting::target = PartCreatureType::HAND_RIGHT;
        unsetTarget();
    });
    currentLayer->addChild(targetChoser,ZLevel::USER_INTERFACE,"tHandR");
    
}
void ControlTargeting::unsetTarget(){
    currentLayer->removeChildByName("tHead");
    currentLayer->removeChildByName("tUpperT");
    currentLayer->removeChildByName("tHandL");
    currentLayer->removeChildByName("tHandR");
    currentLayer->removeChildByName("tBottomT");
    currentLayer->removeChildByName("tLegR");
    currentLayer->removeChildByName("tLegL");
}
