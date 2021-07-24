#include <ui/CocosGUI.h>

#include "GameLayer.h"
#include "gameUI.h"
#include "Creature.h"


std::string SceneEntities::player = "player";
std::string SceneEntities::ball = "ball";
std::string SceneEntities::ball_attacke = "ball_attacke";
std::vector<std::string> SceneEntities::enemy(0);
std::string SceneEntities::text = "text";
std::string SceneEntities::ui = "ui";
std::string SceneEntities::gamesession = "gamesession";


cocos2d::Scene* GameLayer::createScene(){
    cocos2d::Scene* scene = GameLayer::createWithPhysics();
    cocos2d::Node* la = GameLayer::create();
    scene->addChild(la);
    /*Physics debug*/
    //cocos2d::PhysicsWorld* ph = scene->getPhysicsWorld();
    //ph->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);    
    
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
    initLevel("world/area0/level0.tmx");
    intCreatures();
    initListeners();
    initUI();

    return true;
}
void GameLayer::initLayers(){
    cocos2d::Layer* gameSessionLayer = cocos2d::Layer::create();
    this->addChild(gameSessionLayer,ZLevel::MIDLEGROUND,SceneEntities::gamesession);
    cocos2d::Layer* UILayer = cocos2d::Layer::create();
    this->addChild(UILayer,ZLevel::USER_INTERFACE,SceneEntities::ui);
}
void GameLayer::initLevel(std::string level_path){
    world = new World(level_path,this);
}
void GameLayer::intCreatures(){
    visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("textures/mainSheet.plist");
    spriteSheet = cocos2d::SpriteBatchNode::create("textures/mainSheet.png");
    this->getChildByName(SceneEntities::gamesession)->addChild(spriteSheet);

    Enemy* e;
    SceneEntities::enemy.push_back("enemy0");
    e = new Enemy("enemy.png",CreatureType::HUMANOID,cocos2d::Vec2(500,160),this,SceneEntities::enemy.back());
    e->setWeapon(WeaponType::SPEAR);
    enemy.push_back(e);


    player = new Player("kittymitty.png",CreatureType::HUMANOID,cocos2d::Vec2(100,160),this,SceneEntities::player);
    player->setWeapon(WeaponType::SWORD); 
    /*Init camera. And set on player*/
    
    this->getChildByName(SceneEntities::gamesession)->runAction(cocos2d::Follow::createWithOffset(player->getCreatureSprite(),-100,-100,cocos2d::Rect(0,0,2500,3200)));
}
void GameLayer::initUI(){
    ctarg = new ControlTargeting(player,this->getChildByName(SceneEntities::ui));
    ckeys = new ControlKeys(player,cocos2d::Vec2(0.15,0.1),this->getChildByName(SceneEntities::ui));
    cattc = new ControlAttc(player,this->getChildByName(SceneEntities::ui));
    shows = new ShowStats(player,&enemy,this->getChildByName(SceneEntities::gamesession));
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
    //Check if body was collided
    if (a->getCategoryBitmask() & b->getCollisionBitmask() == 0 ||
        b->getCategoryBitmask() & a->getCollisionBitmask() == 0){
        
        return false;
    }
    
    return true;
}