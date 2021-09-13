#include "Player.h"

///////////////////////////////////////////////////////*Player class*///////////////////////////////////////////////////////
Player::Player(CreatureInfo::Type type,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id) :
    Creature(type,pos,gameLayer,id){
    
    currentInteractedEnemy = -1;
    creature_info.characteristic.stamina_regeneration_counter = 0;
}
void Player::setPlayerPosition(float x, float y){
    creature_sprite->setPosition(x,y);
    if (isWeaponSet)
        creature_weapon->getSprite()->setPosition(x,y);
    if (isStatisticsShowing)
        creature_statistics->setPosition(x,y);
}
void Player::update(float dt){
    showStatistics(DebugStatistics::ACTUAL_GAME);
    if (isNewState){
        updateCurrentState();
    }
    updatePermament();
}
void Player::remove(){
    
}
void Player::initStatistics(cocos2d::Node* layer){
    if (!isStatisticsShowing){
        isStatisticsShowing = true;
        //Create sprite obj
        creature_statistics = cocos2d::Node::create(); 
        auto lable = cocos2d::Label::createWithTTF("","fonts/arial.ttf",18,cocos2d::Size::ZERO);
        auto top_part_icon = cocos2d::Sprite::createWithSpriteFrameName("TopPart.png");
        auto midle_part_icon = cocos2d::Sprite::createWithSpriteFrameName("MiddlePart.png");
        auto bottom_part_icon = cocos2d::Sprite::createWithSpriteFrameName("BottomPart.png");
        auto cross_icon = cocos2d::Sprite::createWithSpriteFrameName("CrossIcon.png");
        auto armor_icon = cocos2d::Sprite::createWithSpriteFrameName("ArmorIcon.png");
        auto cross_icon1 = cocos2d::Sprite::createWithSpriteFrameName("CrossIcon.png");
        auto armor_icon1 = cocos2d::Sprite::createWithSpriteFrameName("ArmorIcon.png");
        auto cross_icon2 = cocos2d::Sprite::createWithSpriteFrameName("CrossIcon.png");
        auto armor_icon2 = cocos2d::Sprite::createWithSpriteFrameName("ArmorIcon.png");
        auto blood_icon = cocos2d::Sprite::createWithSpriteFrameName("BloodEmptyIcon.png");
        auto stamina_icon = cocos2d::Sprite::createWithSpriteFrameName("StaminaIcon.png");
        cocos2d::Texture2D::TexParams tpar = {
            cocos2d::backend::SamplerFilter::NEAREST,
            cocos2d::backend::SamplerFilter::NEAREST,
            cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE,
            cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE
        };
        //Set up parameters sprite obj
        top_part_icon->getTexture()->setTexParameters(tpar);
        top_part_icon->setScale(5);
        top_part_icon->setPosition(-50,50);

        midle_part_icon->getTexture()->setTexParameters(tpar);
        midle_part_icon->setScale(5);
        midle_part_icon->setPosition(-50,20);

        bottom_part_icon->getTexture()->setTexParameters(tpar);
        bottom_part_icon->setScale(5);
        bottom_part_icon->setPosition(-50,-10);

        armor_icon->getTexture()->setTexParameters(tpar);
        armor_icon->setScale(5);
        armor_icon->setPosition(-10,50);

        cross_icon->getTexture()->setTexParameters(tpar);
        cross_icon->setScale(3);
        cross_icon->setPosition(10,50);

        armor_icon1->getTexture()->setTexParameters(tpar);
        armor_icon1->setScale(5);
        armor_icon1->setPosition(-10,20);

        cross_icon1->getTexture()->setTexParameters(tpar);
        cross_icon1->setScale(3);
        cross_icon1->setPosition(10,20);

        armor_icon2->getTexture()->setTexParameters(tpar);
        armor_icon2->setScale(5);
        armor_icon2->setPosition(-10,-10);

        cross_icon2->getTexture()->setTexParameters(tpar);
        cross_icon2->setScale(3);
        cross_icon2->setPosition(10,-10);

        blood_icon->getTexture()->setTexParameters(tpar);
        blood_icon->setScale(5);
        blood_icon->setPosition(-50,-40);

        stamina_icon->getTexture()->setTexParameters(tpar);
        stamina_icon->setScale(5);
        stamina_icon->setPosition(-50,-70);

        //Adding sprite objects to main 'creature_statistics node'
        creature_statistics->addChild(lable,SceneZOrder::USER_INTERFACE,"lable");
        creature_statistics->addChild(top_part_icon,SceneZOrder::USER_INTERFACE,"top_part_icon");
        creature_statistics->addChild(midle_part_icon,SceneZOrder::USER_INTERFACE,"midle_part_icon");
        creature_statistics->addChild(bottom_part_icon,SceneZOrder::USER_INTERFACE,"bottom_part_icon");
        creature_statistics->addChild(armor_icon,SceneZOrder::USER_INTERFACE,"armor_icon");
        creature_statistics->addChild(cross_icon,SceneZOrder::USER_INTERFACE,"cross_icon");
        creature_statistics->addChild(armor_icon1,SceneZOrder::USER_INTERFACE,"armor_icon");
        creature_statistics->addChild(cross_icon1,SceneZOrder::USER_INTERFACE,"cross_icon");
        creature_statistics->addChild(armor_icon2,SceneZOrder::USER_INTERFACE,"armor_icon");
        creature_statistics->addChild(cross_icon2,SceneZOrder::USER_INTERFACE,"cross_icon");
        creature_statistics->addChild(blood_icon,SceneZOrder::USER_INTERFACE,"blood_icon");
        creature_statistics->addChild(stamina_icon,SceneZOrder::USER_INTERFACE,"stamina_icon");
        //Set node defauld position
        creature_statistics->setPosition(WorldProperties::screenSize.width * 0.1,WorldProperties::screenSize.height * 0.75);
        //Add creature_statistics node to layer
        layer->addChild(creature_statistics,SceneZOrder::USER_INTERFACE);
    }
    else{
        isStatisticsShowing = false;
        layer->removeChild(creature_statistics);
    }
}
void Player::showStatistics(DebugStatistics type){
    /*For statistics*/
    if (isStatisticsShowing){
        setStatistics(type);
    }
}
void Player::initEnemyContainer(std::vector<Enemy*> enemies){
    this->enemyNode = &enemies;
}