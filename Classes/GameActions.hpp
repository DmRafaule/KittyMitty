#pragma once 

#include <cocos2d.h>

/**
 * @brief Class wich define velocity for action moveBy
*/
class PhysicMoveBy : public cocos2d::MoveBy{
public:
   PhysicMoveBy();
   virtual ~PhysicMoveBy();
   static PhysicMoveBy* create(float duration, const cocos2d::Vec2& velocity);
   virtual void update(float dt) override;
   virtual PhysicMoveBy* clone() const override;
private:
   void initVelocity(const cocos2d::Vec2& velocity);
   cocos2d::Vec2 _velocity;
};
class PhysicRotateBy : public cocos2d::RotateBy{
public:
   PhysicRotateBy();
   virtual ~PhysicRotateBy();
   static PhysicRotateBy* create(float duration, const float& angularVelocity);
   virtual void update(float dt) override;
   virtual PhysicRotateBy* clone() const override;
private:
   void initVelocity(const float& angularVelocity);
   float _angularVelocity;
};