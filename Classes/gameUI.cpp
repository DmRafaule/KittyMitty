#include "gameUI.h"
#include "GameLayer.h"
#include "Enemy.h"
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
    this->clickForCloseStatistics = false;
}
ShowStats::~ShowStats(){}
void ShowStats::update(float dt){}
void ShowStats::updateTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    //This is will execute twice so ...     
    if (!clickForOpen){
        clickForOpen = true;
        /*This is converting coordinates (expand them to global)*/
        cocos2d::Vec2 pos = currentLayer->getChildByName(SceneLayer::gamesession)->convertTouchToNodeSpace(touch);
        //Clear before draw new one
        if (clickForCloseStatistics){
            // Click on player
            if (creature->getCreatureSprite()->getBoundingBox().containsPoint(pos)){
                clickForCloseStatistics = false;
                creature->removeStatistics(currentLayer->getChildByName(SceneLayer::ui));
                for (Enemy*& enemy : *(creatureE))
                    enemy->removeStatistics(currentLayer->getChildByName(SceneLayer::gamesession));
            }
            //Click on enemies
            for (Enemy*& enemy : *(creatureE))
                if (enemy->getCreatureSprite()->getBoundingBox().containsPoint(pos)){
                    clickForCloseStatistics = false;
                    enemy->removeStatistics(currentLayer->getChildByName(SceneLayer::gamesession));
                    creature->removeStatistics(currentLayer->getChildByName(SceneLayer::ui));
                }
        }
        else{
            if (creature->getCreatureSprite()->getBoundingBox().containsPoint(pos)){
                clickForCloseStatistics = true;
                creature->initStatistics(currentLayer->getChildByName(SceneLayer::ui));
            }
            for (Enemy*& enemy : *(creatureE)){
                if (enemy->getCreatureSprite()->getBoundingBox().containsPoint(pos)){
                    clickForCloseStatistics = true;
                    enemy->initStatistics(currentLayer->getChildByName(SceneLayer::gamesession));
                }
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
void ShowStats::createEffect(){
    logos_sprites.resize(11);
    logos_sprites.shrink_to_fit();
    logos_sprites.at(0) = cocos2d::Sprite::createWithSpriteFrameName("");
}
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


    button_interact = cocos2d::Sprite::createWithSpriteFrameName("interactB.png");
    button_interact->getTexture()->setTexParameters(tpar);
    button_interact->setScale(3);
    button_interact->setPosition(cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width*offset.x,
                                               cocos2d::Director::getInstance()->getVisibleSize().height*offset.y + button_interact->getBoundingBox().size.height * 1.5));
    currentLayer->addChild(button_interact);

    button_jump = cocos2d::Sprite::createWithSpriteFrameName("controlBJump.png");
    button_jump->getTexture()->setTexParameters(tpar);
    button_jump->setScale(3);
    button_jump->setPosition(cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width - button_jump->getBoundingBox().size.width,
                                           cocos2d::Director::getInstance()->getVisibleSize().height*offset.y));
    currentLayer->addChild(button_jump);

    button_jump = cocos2d::Sprite::createWithSpriteFrameName("controlBJump.png");
    button_jump->getTexture()->setTexParameters(tpar);
    button_jump->setScale(3);
    button_jump->setPosition(cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width - button_jump->getBoundingBox().size.width,
                                           cocos2d::Director::getInstance()->getVisibleSize().height*offset.y));
    currentLayer->addChild(button_jump);

    button_menu = cocos2d::Sprite::createWithSpriteFrameName("controlBMenu.png");
    button_menu->getTexture()->setTexParameters(tpar);
    button_menu->setScale(3);
    button_menu->setPosition(cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width  - button_menu->getBoundingBox().size.width,
                                           cocos2d::Director::getInstance()->getVisibleSize().height - button_menu->getBoundingBox().size.height));
    currentLayer->addChild(button_menu);
}
ControlKeys::~ControlKeys(){}

void ControlKeys::update(float dt){}
void ControlKeys::updateTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
     
    if (button_left->getBoundingBox().containsPoint(touch->getLocation())){
        //Here add action when touch and change frame on pushed
        //button_left->setSpriteFrame("controlBJump.png");
        //On the ground
        if (creature->getCreatureInfo()->state == CreatureInfo::State::IDLE ||
            creature->getCreatureInfo()->state == CreatureInfo::State::STAND_UP)
            creature->setCreatureState(CreatureInfo::State::START_RUN);
        //In air
        else if (creature->getCreatureInfo()->state == CreatureInfo::State::IN_FALL ||
                 creature->getCreatureInfo()->state == CreatureInfo::State::IN_JUMP)
            creature->setCreatureState(CreatureInfo::State::SOARING);
        //On steps
        else if (creature->getCreatureInfo()->state == CreatureInfo::State::ON_STEPS)
            creature->setCreatureState(CreatureInfo::State::MOVE_BY_STEPS);
        //On wall
        else if (creature->getCreatureInfo()->state == CreatureInfo::State::ON_WALL)
            creature->setCreatureState(CreatureInfo::State::LETGO);
        creature->getCreatureInfo()->dmove = CreatureInfo::DMove::LEFT;
    }
    if (button_right->getBoundingBox().containsPoint(touch->getLocation())){
        //On the ground
        if (creature->getCreatureInfo()->state == CreatureInfo::State::IDLE ||
            creature->getCreatureInfo()->state == CreatureInfo::State::STAND_UP)
            creature->setCreatureState(CreatureInfo::State::START_RUN);
        //In air
        else if (creature->getCreatureInfo()->state == CreatureInfo::State::IN_FALL ||
                 creature->getCreatureInfo()->state == CreatureInfo::State::IN_JUMP)
            creature->setCreatureState(CreatureInfo::State::SOARING);
        //On steps
        else if (creature->getCreatureInfo()->state == CreatureInfo::State::ON_STEPS)
            creature->setCreatureState(CreatureInfo::State::MOVE_BY_STEPS);
        //On wall
        else if (creature->getCreatureInfo()->state == CreatureInfo::State::ON_WALL)
            creature->setCreatureState(CreatureInfo::State::LETGO);
        creature->getCreatureInfo()->dmove = CreatureInfo::DMove::RIGHT;
    }
    if (button_interact->getBoundingBox().containsPoint(touch->getLocation())){
        creature->setCreatureState(CreatureInfo::State::INTERACTING);
    }
    if (button_jump->getBoundingBox().containsPoint(touch->getLocation()) &&
        creature->getCreatureInfo()->characteristic.stamina >= 5 && 
        creature->getCreatureInfo()->characteristic.current_jump_ability_num <= creature->getCreatureInfo()->characteristic.jump_ability){
        //Jump from wall
        if (creature->getCreatureInfo()->state == CreatureInfo::State::ON_WALL){
            creature->setCreatureState(CreatureInfo::State::JUMP_FROM_WALL);
        }
        //Jump from ground
        else if (creature->getCreatureInfo()->state != CreatureInfo::State::JUMP_FROM_WALL){
            creature->setCreatureState(CreatureInfo::State::IN_JUMP);
            creature->getCreatureInfo()->dmove = CreatureInfo::DMove::TOP;
        }
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
    if (button_interact->getBoundingBox().containsPoint(touch->getLocation())){
        
    }
    if (button_jump->getBoundingBox().containsPoint(touch->getLocation())){
    
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

ControlAttc::ControlAttc(Creature* target, cocos2d::Node* layer){
    this->creature = target;
    this->currentLayer = layer;
    this->isRightPlaceForControle = false;
}
ControlAttc::~ControlAttc(){}
void ControlAttc::update(float dt){}
void ControlAttc::updateTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    /*On right part of screen*/
    if ( touch->getLocation().x > WorldProperties::screenSize.width/2){
        isRightPlaceForControle = true;
        //Find start position of touch
        touchPointStart = touch->getLocation();
        touchPointEnd = touch->getLocation();
    }
}
void ControlAttc::updateTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event){
    //If we realy was on right half of screen
    if (isRightPlaceForControle){
        isRightPlaceForControle = false;
        //Calulate attacke direction Only if lenght of touch line more than ...
        if (std::abs(touchPointEnd.x - touchPointStart.x) > WorldProperties::screenSize.width/5 ||
            std::abs(touchPointEnd.y - touchPointStart.y) > WorldProperties::screenSize.height/4){
            setDirectionAttacke();
            creature->setCreatureState(CreatureInfo::State::ATTACK);
        }
    }
}
void ControlAttc::updateTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event){
    if (isRightPlaceForControle){
        touchPointEnd = touch->getLocation();
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
            creature->getCreatureInfo()->dattack = TypeAttacke::PLAYER_RIGHT_TO_LEFT;
        else if (touchPointStart.x < touchPointEnd.x)
            creature->getCreatureInfo()->dattack = TypeAttacke::PLAYER_LEFT_TO_RIGHT;
    }
    if (trembling.x < 50){
        if (touchPointStart.y < touchPointEnd.y)
            creature->getCreatureInfo()->dattack = TypeAttacke::PLAYER_DOWN_TO_TOP;
        else if (touchPointStart.y > touchPointEnd.y)
            creature->getCreatureInfo()->dattack = TypeAttacke::PLAYER_TOP_TO_DOWN;
    }
}


void ControlAttc::createEffect(){}
void ControlAttc::removeEffect(){}