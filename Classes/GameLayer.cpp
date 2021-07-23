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
    e = new Enemy("enemy.png",CreatureType::HUMANOID,cocos2d::Vec2(500,160),this,SceneEntities::enemy.at(SceneEntities::enemy.size()-1));
    e->getCreatureSprite()->runAction(cocos2d::RepeatForever::create(cocos2d::Sequence::create(cocos2d::MoveBy::create(5.f,cocos2d::Vec2(300,0)),
                                                                cocos2d::MoveBy::create(5.f,cocos2d::Vec2(-300,0)),
                                                                nullptr)));
    e->setWeapon(WeaponType::SWORD);
    enemy.push_back(e);

    SceneEntities::enemy.push_back("enemy1");
    e = new Enemy("enemy.png",CreatureType::HUMANOID,cocos2d::Vec2(100,340),this,SceneEntities::enemy.at(SceneEntities::enemy.size()-1));
    e->getCreatureSprite()->runAction(cocos2d::RepeatForever::create(cocos2d::Sequence::create(cocos2d::MoveBy::create(5.f,cocos2d::Vec2(200,0)),
                                                                cocos2d::MoveBy::create(5.f,cocos2d::Vec2(-200,0)),
                                                                nullptr)));
    e->setWeapon(WeaponType::AXE);
    enemy.push_back(e);


    SceneEntities::enemy.push_back("enemy2");
    e = new Enemy("enemy.png",CreatureType::HUMANOID,cocos2d::Vec2(700,160),this,SceneEntities::enemy.at(SceneEntities::enemy.size()-1));
    e->setWeapon(WeaponType::SPEAR);
    enemy.push_back(e);


    player = new Player("kittymitty.png",CreatureType::HUMANOID,cocos2d::Vec2(100,160),this,SceneEntities::player);
    player->setWeapon(WeaponType::SWORD); 
    /*Init camera. And set on player*/
    
    this->getChildByName(SceneEntities::gamesession)->runAction(cocos2d::Follow::createWithOffset(player->getCreatureSprite(),-100,-100,cocos2d::Rect(0,0,2500,3200)));
}
void GameLayer::initUI(){
    ctarg = new ControlTargeting(this);
    ckeys = new ControlKeys(cocos2d::Vec2(0.15,0.1),this);
    cattc = new ControlAttc(this);
    shows = new ShowStats(this);
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
    shows->update(dt,this);
    ctarg->update(dt,this);
    cattc->update(dt,this);
    ckeys->update(dt,this);
    player->update(dt);
    for (auto &i : enemy){
        i->update(dt);
    }
    world->update(dt);
}


bool GameLayer::touchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    shows->updateTouchBegan(touch,event,this);
    ctarg->updateTouchBegan(touch,event,this);
    cattc->updateTouchBegan(touch,event,this);
    ckeys->updateTouchBegan(touch,event,this);
    return true;
}
void GameLayer::touchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    shows->updateTouchEnded(touch,event,this);
    ctarg->updateTouchEnded(touch,event,this);
    cattc->updateTouchEnded(touch,event,this);
    ckeys->updateTouchEnded(touch,event,this);
}
void GameLayer::touchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    shows->updateTouchMoved(touch,event,this);
    ctarg->updateTouchMoved(touch,event,this);
    cattc->updateTouchMoved(touch,event,this);
    ckeys->updateTouchMoved(touch,event,this);
}
void GameLayer::touchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event){
    shows->updateTouchCanceled(touch,event,this);
    ctarg->updateTouchCanceled(touch,event,this);
    cattc->updateTouchCanceled(touch,event,this);
    ckeys->updateTouchCanceled(touch,event,this);
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