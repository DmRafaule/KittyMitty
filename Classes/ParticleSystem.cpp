#include "ParticleSystem.h"
#include "GameLayer.h"
#include "gameUI.h"

#include <time.h>

std::vector<cocos2d::DrawNode*> ParticleSystem::pathEffect(5);
float ParticleSystem::part_radius = 0;


ParticleSystem::ParticleSystem(){
   
}
ParticleSystem::~ParticleSystem(){
   
}
void ParticleSystem::setPosPointOnCircle(cocos2d::Vec2& point_destination,cocos2d::Vec2 point_center){
   float cat1 = point_center.x - point_destination.x;
   float cat2 = point_center.y - point_destination.y;
   float angle  = atan2(cat1,cat2) + M_PI;
         
   point_destination.y -= (part_radius * (cos(angle)));
   point_destination.x -= (part_radius * (sin(angle))); 
}

void ParticleSystem::removeEffect(Effects typeEf, void* node){
   switch (typeEf){
      case Effects::LINE_PATH:{
         for (auto &particle : pathEffect){
            static_cast<cocos2d::Node*>(node)->removeChild(particle);
         }
         break;
      }
   }
}
void ParticleSystem::createEffect(Effects typeEf,void* node){
   srand(time(0));
   switch (typeEf){
   case Effects::LINE_PATH:{
      /*Start position of line effect*/
      cocos2d::Vec2 startPos = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width*0.15,
                                           cocos2d::Director::getInstance()->getVisibleSize().height*0.15);
      /*End position of line effect*/
      cocos2d::Vec2 endPos = GameUI::getTouchLocation(0);
      /*Cathetes of triangle where endPos it's point on circle
         and startPos it's center of circle
      */
      float cat1 = endPos.x - startPos.x;
      float cat2 = endPos.y - startPos.y;
      /*radius(or distance bettween center endPos and startPos*/
      float radius = std::sqrt(pow(cat1,2)+pow(cat2,2));
      /*Steps bettween points on radius*/
      float stepBetPoint = radius / pathEffect.size();
      for (int i = 0; i < pathEffect.size(); ++i){
         /*Clear effect before draw a new one*/
         if (pathEffect[i] != nullptr)
            static_cast<cocos2d::Node*>(node)->removeChild(pathEffect[i]);   
         /*Stop increase our effect if raduis more than 120*/
         if (radius <= 120)
            part_radius = stepBetPoint + i;
         /*Set up positions for each point of radius(part_radius)*/
         setPosPointOnCircle(startPos,endPos);
         /*Init drawing stuff(particles)*/
         pathEffect[i] = cocos2d::DrawNode::create();
         pathEffect[i]->drawPoint(startPos,10+i*4.f,cocos2d::Color4F(196.f/255.f,59.f/255.f,121.f/255.f,0.2f + 0.2f*i));
         if (i == 4)
            pathEffect[i]->drawPoint(startPos,40.f,cocos2d::Color4F(255.f/255.f,255.f/255.f,255.f/255.f,1.f));
         static_cast<cocos2d::Node*>(node)->addChild(pathEffect[i],10+i);
         
      }
      break;
   }
   }
}
