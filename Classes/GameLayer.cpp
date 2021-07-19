#include <ui/CocosGUI.h>

#include "GameLayer.h"
#include "gameUI.h"
#include "Creature.h"


std::string LayerChild::player = "player";
std::string LayerChild::ball = "ball";
std::string LayerChild::ball_attacke = "ball_attacke";
std::string LayerChild::enemy = "enemy";
std::string LayerChild::text = "text";

const unsigned int WorldTiming::SLOW = 10;
const unsigned int WorldTiming::NORM = 1;
const float WorldTiming::FAST = 0.1;
const float WorldTiming::RSLOW = -10;
const float WorldTiming::RNORM = -1;
const float WorldTiming::RFAST = -0.1;

cocos2d::Scene* GameLayer::createScene(){
    cocos2d::Scene* scene = GameLayer::create();
    
    return scene;
}
void GameLayer::menuCloseCallback(cocos2d::Ref* pSender){
    
    delete ckeys;
    delete cattc;
    delete player;
    cocos2d::Director::getInstance()->end();
}

bool GameLayer::init(){

    if ( !cocos2d::Scene::init() )
        return false;
    cocos2d::Layer* gameSessionLayer = cocos2d::Layer::create();
    cocos2d::Layer* UILayer = cocos2d::Layer::create();
    this->addChild(gameSessionLayer,Layer::MIDLEGROUND,"gamesession");
    this->addChild(UILayer,Layer::USER_INTERFACE,"ui");
    
    level = cocos2d::TMXTiledMap::create("world/area0/level0.tmx");
    level->setScale(2);
    this->getChildByName("gamesession")->addChild(level,1);

    initVarsAndObj();
    initListeners();
    initUI();

    return true;
}
void GameLayer::initUI(){
    if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
        shows = new ShowStats(this);
        ckeys = new ControlKeys(cocos2d::Vec2(0.15,0.1),this);
        cattc = new ControlAttc(this);
    }
    else if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX){
        shows = new ShowStats(this);
        ckeys = new ControlKeys(cocos2d::Vec2(0.15,0.1),this);
        cattc = new ControlAttc(this);
    }
}
void GameLayer::initListeners(){
    /*Init listeners*/
    auto listener = cocos2d::EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(GameLayer::touchBegan,this);
    listener->onTouchesEnded = CC_CALLBACK_2(GameLayer::touchEnded,this);
    listener->onTouchesMoved = CC_CALLBACK_2(GameLayer::touchMoved,this);
    listener->onTouchesCancelled = CC_CALLBACK_2(GameLayer::touchCanceled,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    /*Start update this layer*/
    this->schedule(CC_SCHEDULE_SELECTOR(GameLayer::update),0.1f,CC_REPEAT_FOREVER,0);
}
void GameLayer::initVarsAndObj(){
    visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("textures/mainSheet.plist");
    spriteSheet = cocos2d::SpriteBatchNode::create("textures/mainSheet.png");
    this->getChildByName("gamesession")->addChild(spriteSheet);

    Enemy* e;
    LayerChild::enemy = "enemy0";
    e = new Enemy("enemy.png",CreatureType::HUMANOID,cocos2d::Vec2(500,100),this,LayerChild::enemy);
    e->getCreatureSprite()->runAction(cocos2d::RepeatForever::create(cocos2d::Sequence::create(cocos2d::MoveBy::create(5.f,cocos2d::Vec2(300,0)),
                                                                cocos2d::MoveBy::create(5.f,cocos2d::Vec2(-300,0)),
                                                                nullptr)));
    e->setWeapon(WeaponType::SWORD);
    enemy.push_back(e);

    LayerChild::enemy = "enemy1";
    e = new Enemy("enemy.png",CreatureType::HUMANOID,cocos2d::Vec2(100,100),this,LayerChild::enemy);
    e->getCreatureSprite()->runAction(cocos2d::RepeatForever::create(cocos2d::Sequence::create(cocos2d::MoveBy::create(5.f,cocos2d::Vec2(0,200)),
                                                                cocos2d::MoveBy::create(5.f,cocos2d::Vec2(0,-200)),
                                                                nullptr)));
    e->setWeapon(WeaponType::AXE);
    enemy.push_back(e);

    LayerChild::enemy = "enemy2";
    e = new Enemy("enemy.png",CreatureType::HUMANOID,cocos2d::Vec2(550,300),this,LayerChild::enemy);
    e->getCreatureSprite()->runAction(cocos2d::RepeatForever::create(cocos2d::Sequence::create(cocos2d::MoveBy::create(5.f,cocos2d::Vec2(300,-100)),
                                                                cocos2d::MoveBy::create(5.f,cocos2d::Vec2(-300,100)),
                                                                nullptr)));
    e->setWeapon(WeaponType::SPEAR);
    enemy.push_back(e);

    LayerChild::enemy = "enemy3";
    e = new Enemy("enemy.png",CreatureType::HUMANOID,cocos2d::Vec2(300,300),this,LayerChild::enemy);
    e->setWeapon(WeaponType::SWORD);
    enemy.push_back(e);


    player = new Player("kittymitty.png",CreatureType::HUMANOID,cocos2d::Vec2(100,100),this,LayerChild::player);
    player->setWeapon(WeaponType::SWORD); 
    /*Init camera. And set on player*/
    
    this->getChildByName("gamesession")->runAction(cocos2d::Follow::createWithOffset(player->getCreatureSprite(),-100,-100,cocos2d::Rect(0,0,1600,1600)));
}



void GameLayer::update(float dt){
    shows->update(dt,this);
    cattc->update(dt,this);
    player->update(dt);
    for (auto &i : enemy)
        i->update(dt);
}

bool GameLayer::touchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    shows->updateTouchBegan(touch,event,this);
    cattc->updateTouchBegan(touch,event,this);
    return true;
}
void GameLayer::touchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    cattc->updateTouchEnded(touch,event,this);
}
void GameLayer::touchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    cattc->updateTouchMoved(touch,event,this);
}
void GameLayer::touchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    cattc->updateTouchCanceled(touch,event,this);
}
