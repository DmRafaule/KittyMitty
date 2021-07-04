#pragma once 

#include "cocos2d.h"

#include <vector>


enum Effects{
   LINE_PATH,
};


class ParticleSystem : public cocos2d::Scene{
public:
   ParticleSystem();
   ~ParticleSystem();
   
   static void createEffect(Effects typeEf, void* node);
   static void removeEffect(Effects typeEf, void* node);
private:
      /** Set position of point related to pointNode like pointNode is a center of circle
      @param point_destination is a point will be changed to new position depends on pos of pointNOde and angle of texture of point node
      @param point_center is a center of entity node which have to have some kind of position and angle 
      **/
   static void setPosPointOnCircle(cocos2d::Vec2& point_destination,cocos2d::Vec2 point_center);
   
public:
   static std::vector<cocos2d::DrawNode*> pathEffect;
   static float   part_radius;
};