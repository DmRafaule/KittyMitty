#include "GameActions.hpp"

PhysicMoveBy::PhysicMoveBy(){}
PhysicMoveBy::~PhysicMoveBy(){}
void PhysicMoveBy::update(float dt){
   if (_target)
    {
#if CC_ENABLE_STACKABLE_ACTIONS
        cocos2d::Vec3 currentPos = _target->getPosition3D();
        cocos2d::Vec3 diff = currentPos - _previousPosition;
        _startPosition = _startPosition + diff;
        cocos2d::Vec3 newPos =  _startPosition + (_positionDelta * dt);
        _target->setPosition3D(newPos);
        _target->getPhysicsBody()->setVelocity(cocos2d::Vec2(_velocity));//Define how fast platform is
        _previousPosition = newPos;
#else
        _target->setPosition3D(_startPosition + _positionDelta * dt);
#endif // CC_ENABLE_STACKABLE_ACTIONS
    }
}
PhysicMoveBy* PhysicMoveBy::clone() const{
   // no copy constructor
   return PhysicMoveBy::create(_duration, _velocity);
}
PhysicMoveBy* PhysicMoveBy::create(float duration, const cocos2d::Vec2& velocity){
   PhysicMoveBy *ret = new PhysicMoveBy();
   ret->initVelocity(velocity);
   
   if (ret && ret->initWithDuration(duration, velocity)){
       ret->autorelease();
       return ret;
   }
   
   delete ret;
   return nullptr;
}
void PhysicMoveBy::initVelocity(const cocos2d::Vec2& velocity){
   this->_velocity = velocity;
}

PhysicRotateBy::PhysicRotateBy(){}
PhysicRotateBy::~PhysicRotateBy(){}
PhysicRotateBy* PhysicRotateBy::create(float duration, const float& angularVelocity){
   PhysicRotateBy *ret = new PhysicRotateBy();
   ret->initVelocity(angularVelocity);
   
   if (ret && ret->initWithDuration(duration, angularVelocity)){
       ret->autorelease();
       return ret;
   }
   
   delete ret;
   return nullptr;
}
void PhysicRotateBy::update(float dt){
   if (_target)
    {
        if(_is3D)
        {
            cocos2d::Vec3 v;
            v.x = _startAngle.x + _deltaAngle.x * dt;
            v.y = _startAngle.y + _deltaAngle.y * dt;
            v.z = _startAngle.z + _deltaAngle.z * dt;
            _target->setRotation3D(v);
        }
        else
        {
#if CC_USE_PHYSICS
            _target->getPhysicsBody()->setAngularVelocity(_angularVelocity);
            if (_startAngle.x == _startAngle.y && _deltaAngle.x == _deltaAngle.y)
            {
                _target->setRotation(_startAngle.x + _deltaAngle.x * dt);
            }
            else
            {
                _target->setRotationSkewX(_startAngle.x + _deltaAngle.x * dt);
                _target->setRotationSkewY(_startAngle.y + _deltaAngle.y * dt);
            }
#else
            _target->setRotationSkewX(_startAngle.x + _deltaAngle.x * dt);
            _target->setRotationSkewY(_startAngle.y + _deltaAngle.y * dt);
#endif // CC_USE_PHYSICS
        }
   }
}
PhysicRotateBy* PhysicRotateBy::clone() const{
   // no copy constructor
    auto a = new (std::nothrow) PhysicRotateBy();
    if(_is3D)
        a->initWithDuration(_duration, _deltaAngle);
    else
        a->initWithDuration(_duration, _deltaAngle.x, _deltaAngle.y);
    a->autorelease();
    return a;
}
void PhysicRotateBy::initVelocity(const float& angularVelocity){
   this->_angularVelocity = angularVelocity;
}