#include <ui/CocosGUI.h>

#include "GameLayer.h"
#include "gameUI.h"
#include "Creature.h"


std::string                 SceneLayer::ui = "ui";
std::string                 SceneLayer::gamesession = "gamesession";
std::string                 SceneLayer::bg = "background";



cocos2d::Scene* GameLayer::createScene(){
    cocos2d::Scene* scene = GameLayer::createWithPhysics();
    cocos2d::Node* la = GameLayer::create();
    scene->addChild(la);
    /*Physics debug*/
    cocos2d::PhysicsWorld* ph = scene->getPhysicsWorld();
    ph->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);    
    
    return scene;
}
void GameLayer::menuCloseCallback(cocos2d::Ref* pSender){
    delete ctarg;
    delete ckeys;
    delete cattc;
    delete player;
    delete world;
    cocos2d::Director::getInstance()->end();
}

bool GameLayer::init(){

    if ( !cocos2d::Scene::init() )
        return false;
    initLayers();
    initWorld();
    intCreatures();
    initListeners();
    initUI();

    return true;
}
void GameLayer::initLayers(){
    cocos2d::Layer* gameSessionLayer = cocos2d::Layer::create();
    this->addChild(gameSessionLayer,SceneZOrder::MIDLEGROUND,SceneLayer::gamesession);
    cocos2d::Layer* UILayer = cocos2d::Layer::create();
    this->addChild(UILayer,SceneZOrder::USER_INTERFACE,SceneLayer::ui);
    cocos2d::Layer* BGLayer = cocos2d::Layer::create();
    this->addChild(BGLayer,SceneZOrder::BACKGROUND,SceneLayer::bg);

    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("textures/mainSheet.plist");
    this->getChildByName(SceneLayer::gamesession)->addChild(cocos2d::SpriteBatchNode::create("textures/mainSheet.png"));

    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("textures/worldObj/worldObjSheet.plist");
    this->getChildByName(SceneLayer::gamesession)->addChild(cocos2d::SpriteBatchNode::create("textures/worldObj/worldObjSheet.png"));
    
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("textures/animations/door/doorSheet.plist");
    this->getChildByName(SceneLayer::gamesession)->addChild(cocos2d::SpriteBatchNode::create("textures/animations/door/doorSheet.png"));

    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("textures/animations/lever/leverSheet.plist");
    this->getChildByName(SceneLayer::gamesession)->addChild(cocos2d::SpriteBatchNode::create("textures/animations/lever/leverSheet.png"));

    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("textures/animations/button/buttonSheet.plist");
    this->getChildByName(SceneLayer::gamesession)->addChild(cocos2d::SpriteBatchNode::create("textures/animations/button/buttonSheet.png"));
}
void GameLayer::initWorld(){

    WorldProperties::screenSize = cocos2d::Director::getInstance()->getVisibleSize();

    world = new World(0,this);
}
void GameLayer::intCreatures(){
        
    //Wrong position have to remove creature spawn point
    player = new Player(CreatureInfo::Type::KITTYMITTY,WorldProperties::creatureObj.find(CreatureInfo::Type::KITTYMITTY)->second.point,this->getChildByName(SceneLayer::gamesession),2);
    player->setWeapon((WeaponType)WorldProperties::creatureObj.find(CreatureInfo::Type::KITTYMITTY)->second.typeWepon); 

    
    /*Init camera. And set on player*/
    this->getChildByName(SceneLayer::gamesession)->runAction(
        cocos2d::Follow::createWithOffset(
            player->getCreatureSprite(),
            -100,-100,
            cocos2d::Rect(
                0,
                0,
                WorldProperties::mapSize.width  - WorldProperties::screenSize.width  - 64,
                WorldProperties::mapSize.height - WorldProperties::screenSize.height - 64
            )
        )
    );

    //Init player related fields
    for (auto & e : enemy)
        e->initPlayerDependenceFields();
}
void GameLayer::initUI(){
    ctarg = new ControlTargeting(player,this->getChildByName(SceneLayer::ui));
    ckeys = new ControlKeys(player,cocos2d::Vec2(0.15,0.1),this->getChildByName(SceneLayer::ui));
    cattc = new ControlAttc(player,this->getChildByName(SceneLayer::ui));
    shows = new ShowStats(player,&enemy,this->getChildByName(SceneLayer::gamesession));
}
void GameLayer::initListeners(){
    /*Init listeners*/
    /*Init listeners for graphycs*/
    auto listener = cocos2d::EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(GameLayer::touchBegan,this);
    listener->onTouchesEnded = CC_CALLBACK_2(GameLayer::touchEnded,this);
    listener->onTouchesMoved = CC_CALLBACK_2(GameLayer::touchMoved,this);
    listener->onTouchesCancelled = CC_CALLBACK_2(GameLayer::touchCanceled,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

    /*Init listeners for physics*/
    auto ph_listener = cocos2d::EventListenerPhysicsContact::create();
    ph_listener->onContactBegin = CC_CALLBACK_1(GameLayer::contactBegan,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(ph_listener,this);
    /*Start update this layer*/
    this->schedule(CC_SCHEDULE_SELECTOR(GameLayer::update),0.1f,CC_REPEAT_FOREVER,0);
}
void GameLayer::update(float dt){
    shows->update(dt);
    ctarg->update(dt);
    cattc->update(dt);
    ckeys->update(dt);

    player->update(dt);
    for (auto &i : enemy){
        i->update(dt);
    }
    world->update(dt);
}


bool GameLayer::touchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    for (auto& one_touch : touch){
        shows->updateTouchBegan(one_touch,event);
        ctarg->updateTouchBegan(one_touch,event);
        cattc->updateTouchBegan(one_touch,event);
        ckeys->updateTouchBegan(one_touch,event);
    }
    return true;
}
void GameLayer::touchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    for (auto& one_touch : touch){
        shows->updateTouchEnded(one_touch,event);
        ctarg->updateTouchEnded(one_touch,event);
        cattc->updateTouchEnded(one_touch,event);
        ckeys->updateTouchEnded(one_touch,event);
    }
}
void GameLayer::touchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    for (auto& one_touch : touch){
        shows->updateTouchMoved(one_touch,event);
        ctarg->updateTouchMoved(one_touch,event);
        cattc->updateTouchMoved(one_touch,event);
        ckeys->updateTouchMoved(one_touch,event);
    }
}
void GameLayer::touchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    for (auto& one_touch : touch){
        shows->updateTouchCanceled(one_touch,event);
        ctarg->updateTouchCanceled(one_touch,event);
        cattc->updateTouchCanceled(one_touch,event);
        ckeys->updateTouchCanceled(one_touch,event);
    }
}
bool GameLayer::contactBegan(cocos2d::PhysicsContact &contact){
    cocos2d::PhysicsBody *a = contact.getShapeA()->getBody();
    cocos2d::PhysicsBody *b = contact.getShapeB()->getBody();
    
    for (auto &e : enemy){
        if (e->isVisionEnable())
            if ((b->getCollisionBitmask() & a->getContactTestBitmask()) == e->getCreatureVisions()->getPhysicsBody()->getCollisionBitmask()){
                if ((a->getCollisionBitmask() & b->getContactTestBitmask()) == 1){
                    OUT("I see floor\n");
                }
                else if ((a->getCollisionBitmask() & b->getContactTestBitmask()) == 2){
                    OUT("I see player\n");
                }
                else if ((a->getCollisionBitmask() & b->getContactTestBitmask()) == 3){
                    OUT("I see wall\n");
                }
                else if ((a->getCollisionBitmask() & b->getContactTestBitmask()) == 4){
                    OUT("I see stair\n");
                }
                else if ((a->getCollisionBitmask() & b->getContactTestBitmask()) == 5){
                    OUT("I see roof\n");
                }
                return false;
            }
    }
    //Collide with ...
    switch ((b->getCollisionBitmask() & a->getContactTestBitmask())){
    case 1:{// floor
        if ((a->getCollisionBitmask() & b->getContactTestBitmask()) == 2){
            player->setCreatureState(CreatureInfo::State::LAND_ON);
            return true;
        }else{
            for (auto& e : enemy){
                if ((a->getCollisionBitmask() & b->getContactTestBitmask()) == e->getCreatureSprite()->getPhysicsBody()->getCollisionBitmask()){
                    e->setCreatureState(CreatureInfo::State::LAND_ON);
                    return true;
                }
            }
        }
    }
    case 2:{// creature
        //player->setCreatureState(CreatureInfo::State::GET_DAMMAGE);
        return false;
    }
    case 3:{// wall
        if ((a->getCollisionBitmask() & b->getContactTestBitmask()) == 2){
            player->setCreatureState(CreatureInfo::State::ON_WALL);
            return true;
        }else{
            for (auto& e : enemy){
                if ((a->getCollisionBitmask() & b->getContactTestBitmask()) == e->getCreatureSprite()->getPhysicsBody()->getCollisionBitmask()){
                    e->setCreatureState(CreatureInfo::State::ON_WALL);
                    return true;
                }
            }
        }
    }
    case 4:{// steps
        if ((a->getCollisionBitmask() & b->getContactTestBitmask()) == 2){
            player->setCreatureState(CreatureInfo::State::ON_STEPS);
            return true;
        }else{
            for (auto& e : enemy){
                if ((a->getCollisionBitmask() & b->getContactTestBitmask()) == e->getCreatureSprite()->getPhysicsBody()->getCollisionBitmask()){
                    e->setCreatureState(CreatureInfo::State::ON_STEPS);
                    return true;
                }
            }
        }
    }
    case 5:{// roof
        if ((a->getCollisionBitmask() & b->getContactTestBitmask()) == 2){
            player->setCreatureState(CreatureInfo::State::TAKE_ROOF);
            return true;
        }else{
            for (auto& e : enemy){
                if ((a->getCollisionBitmask() & b->getContactTestBitmask()) == e->getCreatureSprite()->getPhysicsBody()->getCollisionBitmask()){
                    e->setCreatureState(CreatureInfo::State::TAKE_ROOF);
                    return true;
                }
            }
        }
    }
    default:{// other
        return false;
    }
    }
    return false;
}