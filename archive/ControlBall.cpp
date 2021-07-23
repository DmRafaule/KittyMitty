/**
 * @brief
 * Give player control on kittymitty(for android)
 * IN DEVELOPMENT
*/
class ControlBall : public GameUIPhone{
public:
    ControlBall(void* layer);
    virtual ~ControlBall();

    virtual void update(float dt,void* Layer) override;
        virtual void updateTouchBegan(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
        virtual void updateTouchEnded(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
        virtual void updateTouchMoved(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
        virtual void updateTouchCanceled(std::vector<cocos2d::Touch*> touch,cocos2d::Event* event,void* Layer) override;
    virtual void createEffect( void* node) override;
    virtual void removeEffect( void* node) override;
    /**
     * @return angle bettween touch point and center of ball control
    */
    static inline const float getAngle(){ return angleDirection; };
    /**
     * @return status of interaction with ball controller
    */
    static inline const bool getMoving(){ return isMoving; };
    /**
     * @return direction of movement for ball Sprite
    */
    static inline const cocos2d::Vec2 getDirection(){ return directionPoint; };
    /**
     * @return ball default position
    */
    inline const cocos2d::Vec2 getBallDefaultPosition() { return ballDefaultPosition; };
private:
   /** Set position of point related to pointNode like pointNode is a center of circle
       @param point_destination is a point will be changed to new position depends on pos of pointNOde and angle of texture of point node
       @param point_center is a center of entity node which have to have some kind of position and angle 
       **/
    static void setPosPointOnCircle(cocos2d::Vec2& point_destination,cocos2d::Vec2 point_center);
    /**
     * transform radians to degrees
    */
    static float setAngleToRadius(float angle_radian);
    /**
     * Calculated direction of movement relative
     * @param endPoint center of circle controle
     * @param startPoint touch location
     * @return point on circle
    */
    static cocos2d::Vec2 setDirectionPointRelative(cocos2d::Vec2 endPoint,cocos2d::Vec2 startPoint);
private:
    
    static cocos2d::Vec2 directionPoint;//Position on circle of point in pathEffect
    static float   part_radius;//Radius of each point contained in pathEffect
    static float   angleDirection;//Angle of point in pathEffect
    static bool    isMoving;//Is player giveEffect with ball controle
    
    cocos2d::Vec2  ballDefaultPosition;
    bool           isControlBall;//Is player using control ball. If so clear and render some gui effect
};


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
