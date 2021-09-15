#include "Weapon.h"
#include "Enemy.h"
#include "Creature.h"
#include "engMacros.hpp"
#include "gameUI.h"

Weapon::Weapon(std::string weapon_sprite_path,Creature* weapon_owner){
   weapon_sprite = cocos2d::Sprite::createWithSpriteFrameName(weapon_sprite_path);
   this->weapon_owner = weapon_owner;
   weapon_sprite->setAnchorPoint(weapon_sprite->getPosition().ANCHOR_MIDDLE_BOTTOM);
   target_status = "";
   cocos2d::Texture2D::TexParams tpar = {
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE
    };
   weapon_sprite->getTexture()->setTexParameters(tpar);
   isAttack = false;
}
Weapon::~Weapon(){}
void Weapon::updateAttackAnimation(){
   isAttack = true;
   /*Get weapon owner's direction of movement */
   CreatureInfo::DMove dirctionMove = *(weapon_owner_dirmove);
   /*For running action*/
   float angleOfAttacke;
   //where weapon will be, related of owner
   weapon_sprite->setPosition(weapon_owner->getCreatureSprite()->getPosition());

   /*Here make some animations/actions and calculate hit box of weapon*/ 
   switch(weapon_owner->getCreatureInfo()->dattack){
      case TypeAttacke::PLAYER_TOP_TO_DOWN:{
         weapon_sprite->setRotation(0);
         if (dirctionMove == CreatureInfo::DMove::RIGHT){
            weapon_sprite->setFlippedX(false);
            angleOfAttacke = 180;
         }
         else if (dirctionMove == CreatureInfo::DMove::LEFT){
            weapon_sprite->setFlippedX(true);
            angleOfAttacke = -180;
         }
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::RotateTo::create(0.3,angleOfAttacke),
                                                            cocos2d::RotateTo::create(0,0),
                                                            nullptr));
         break;
      }
      case TypeAttacke::PLAYER_DOWN_TO_TOP:{
         weapon_sprite->setRotation(180);
         if (dirctionMove == CreatureInfo::DMove::RIGHT){
            weapon_sprite->setFlippedX(true);
            angleOfAttacke = 0;
         }
         else if (dirctionMove == CreatureInfo::DMove::LEFT){
            weapon_sprite->setFlippedX(false);
            angleOfAttacke = 360;
         }
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::RotateTo::create(0.1,angleOfAttacke),
                                                            cocos2d::RotateTo::create(0,0),
                                                            nullptr));
         break;
      }
      case TypeAttacke::PLAYER_LEFT_TO_RIGHT:{
         weapon_sprite->setFlippedX(false);
         float forMoveBack;
         float forMoveForward;
         weapon_sprite->setRotation(90);

         forMoveBack = -25;
         forMoveForward = 50;
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Sequence::create(cocos2d::MoveBy::create(0.1,cocos2d::Vec2(forMoveBack,0)),cocos2d::MoveBy::create(0.1,cocos2d::Vec2(forMoveForward,0)),nullptr),
                                                            cocos2d::MoveTo::create(0,weapon_owner->getCreatureSprite()->getPosition()),
                                                            cocos2d::RotateTo::create(0,0),
                                                            nullptr));
         break;
      }
      case TypeAttacke::PLAYER_RIGHT_TO_LEFT:{
         weapon_sprite->setFlippedX(true);
         float forMoveBack;
         float forMoveForward;
         weapon_sprite->setRotation(270);

         forMoveBack = 25;
         forMoveForward = -50;
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Sequence::create(cocos2d::MoveBy::create(0.1,cocos2d::Vec2(forMoveBack,0)),cocos2d::MoveBy::create(0.1,cocos2d::Vec2(forMoveForward,0)),nullptr),
                                                            cocos2d::MoveTo::create(0,weapon_owner->getCreatureSprite()->getPosition()),
                                                            cocos2d::RotateTo::create(0,0),
                                                            nullptr));
         break;
      }
      case TypeAttacke::TOP_TO_DOWN:{
         weapon_sprite->setRotation(0);
         if (dirctionMove == CreatureInfo::DMove::RIGHT){
            weapon_sprite->setFlippedX(false);
            angleOfAttacke = 180;
         }
         else if (dirctionMove == CreatureInfo::DMove::LEFT){
            weapon_sprite->setFlippedX(true);
            angleOfAttacke = -180;
         }
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::MoveBy::create(1,cocos2d::Vec2(0,55)),
                                                            cocos2d::RotateTo::create(0.3,angleOfAttacke),
                                                            cocos2d::RotateTo::create(0,0),
                                                            nullptr));
         break;
      }
      case TypeAttacke::DOWN_TO_TOP:{
         weapon_sprite->setRotation(180);
         if (dirctionMove == CreatureInfo::DMove::RIGHT){
            weapon_sprite->setFlippedX(true);
            angleOfAttacke = 0;
         }
         else if (dirctionMove == CreatureInfo::DMove::LEFT){
            weapon_sprite->setFlippedX(false);
            angleOfAttacke = 360;
         }
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::MoveBy::create(1,cocos2d::Vec2(0,-55)),
                                                            cocos2d::RotateTo::create(0.1,angleOfAttacke),
                                                            cocos2d::RotateTo::create(0,0),
                                                            nullptr));
         break;
      }
      case TypeAttacke::LEFT_TO_RIGHT:{
         weapon_sprite->setFlippedX(false);
         float forMoveBack;
         float forMoveForward;
         weapon_sprite->setRotation(90);

         forMoveBack = -25;
         forMoveForward = 50;
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Sequence::create(cocos2d::MoveBy::create(0.3,cocos2d::Vec2(forMoveBack,0)),cocos2d::MoveBy::create(0.5,cocos2d::Vec2(forMoveForward,0)),nullptr),
                                                            cocos2d::MoveTo::create(0,weapon_owner->getCreatureSprite()->getPosition()),
                                                            cocos2d::RotateTo::create(0,0),
                                                            nullptr));
         break;
      }
      case TypeAttacke::RIGHT_TO_LEFT:{
         weapon_sprite->setFlippedX(true);
         float forMoveBack;
         float forMoveForward;
         weapon_sprite->setRotation(270);

         forMoveBack = 25;
         forMoveForward = -50;
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Sequence::create(cocos2d::MoveBy::create(0.3,cocos2d::Vec2(forMoveBack,0)),cocos2d::MoveBy::create(0.5,cocos2d::Vec2(forMoveForward,0)),nullptr),
                                                            cocos2d::MoveTo::create(0,weapon_owner->getCreatureSprite()->getPosition()),
                                                            cocos2d::RotateTo::create(0,0),
                                                            nullptr));
         break;
      }
   }
}
void Weapon::update(){
   weapon_sprite->runAction(cocos2d::MoveTo::create(0.1,weapon_owner->getCreatureSprite()->getPosition()));
   weapon_sprite->getPhysicsBody()->setVelocity(weapon_owner->getCreatureSprite()->getPhysicsBody()->getVelocity());
}

void Weapon::giveEffect(Creature* target){
   setIntergralityTo(target, defineTargetingPart());
}
void Weapon::takeEffect(){
   weapon_owner->getCreatureInfo()->characteristic.stamina = weapon_owner->getCreatureInfo()->characteristic.stamina;
   if (weapon_owner->getCreatureInfo()->characteristic.stamina > 0)
      weapon_owner->getCreatureInfo()->characteristic.stamina -= 10;
   else
      weapon_owner->getCreatureInfo()->characteristic.stamina = 0;
}
PartCreatureType Weapon::defineTargetingPart(){
   PartCreatureType part_type;
   switch (weapon_owner->getCreatureInfo()->dattack){
   case TypeAttacke::PLAYER_TOP_TO_DOWN:{
      part_type = PartCreatureType::TOP;
      target_part = "TopPart";
      break;
   }
   case TypeAttacke::PLAYER_DOWN_TO_TOP:{
      part_type = PartCreatureType::BOTTOM;
      target_part = "BottomPart";
      break;
   }
   case TypeAttacke::PLAYER_LEFT_TO_RIGHT:{
      part_type = PartCreatureType::MIDDLE;
      target_part = "MiddlePart";
      break;
   }
   case TypeAttacke::PLAYER_RIGHT_TO_LEFT:{
      part_type = PartCreatureType::MIDDLE;
      target_part = "MiddlePart";
      break;
   }
   case TypeAttacke::TOP_TO_DOWN:{
      part_type = PartCreatureType::TOP;
      target_part = "TopPart";
      break;
   }
   case TypeAttacke::DOWN_TO_TOP:{
      part_type = PartCreatureType::BOTTOM;
      target_part = "BottomPart";
      break;
   }
   case TypeAttacke::LEFT_TO_RIGHT:{
      part_type = PartCreatureType::MIDDLE;
      target_part = "MiddlePart";
      break;
   }
   case TypeAttacke::RIGHT_TO_LEFT:{
      part_type = PartCreatureType::MIDDLE;
      target_part = "MiddlePart";
      break;
   }
   }
   return part_type;
}
void Weapon::setIntergralityTo(Creature* target, const PartCreatureType& part_type){
   int newIntegrality = target->getPartCreature()->find(part_type)->second->integrality - weapon_caracteristics.weapon_power;
   if (newIntegrality <= 0) {
      newIntegrality = 0;
      target_status = "Dead";
      target->getPartCreature()->find(part_type)->second->status = PartCreatureStatus::DEAD;
   }
   else if (newIntegrality <= target->getPartCreature()->find(part_type)->second->maxIntegrality/2){
      target_status = "HardWonded"; 
      target->getPartCreature()->find(part_type)->second->status = PartCreatureStatus::HARD_WONDED;
   }
   else{
      target_status = "Wonded";
      target->getPartCreature()->find(part_type)->second->status = PartCreatureStatus::WONDED;
   }

   if (part_type == PartCreatureType::TOP)
      target->getCreatureInfo()->frameNameForTopPart = target_part + target_status + ".png";
   else if (part_type == PartCreatureType::MIDDLE)
      target->getCreatureInfo()->frameNameForMiddlePart = target_part + target_status + ".png";
   else 
      target->getCreatureInfo()->frameNameForBottomPart = target_part + target_status + ".png";

   if (target->getCreatureInfo()->isStatisticsShowing && target->mode == DebugStatistics::ACTUAL_GAME)//For avoiding mem corruption when statistics obj not created
      static_cast<cocos2d::Sprite*>(target->getCreatureStatistics()->getChildByTag((int)part_type))->setSpriteFrame(target_part + target_status + ".png");
   target->getPartCreature()->find(part_type)->second->integrality = newIntegrality;
}

Sword::Sword(std::string weapon_sprite_path,Creature* weapon_owner):
   Weapon(weapon_sprite_path,weapon_owner){
   this->weapon_owner_dirmove = &(weapon_owner->getCreatureInfo()->dmove);
   weapon_caracteristics.weapon_power = 1;
   weapon_caracteristics.weapon_mass = 20;
   weapon_caracteristics.weapon_range = 80;
   weapon_physic_body = cocos2d::PhysicsBody::createEdgeBox(cocos2d::Size(weapon_sprite->getBoundingBox().size.width/2,weapon_sprite->getBoundingBox().size.height));
   weapon_physic_body->setDynamic(false);
   weapon_physic_body->setGravityEnable(false);
   weapon_physic_body->setCollisionBitmask(weapon_owner->getCreatureSprite()->getPhysicsBody()->getCollisionBitmask());
   weapon_physic_body->setContactTestBitmask(0xFF);
   weapon_physic_body->setMass(weapon_caracteristics.weapon_mass);
   weapon_sprite->setPhysicsBody(weapon_physic_body);
   weapon_sprite->setScale(5);
}

Axe::Axe(std::string weapon_sprite_path, Creature* weapon_owner):
   Weapon(weapon_sprite_path,weapon_owner){
   this->weapon_owner_dirmove = &(weapon_owner->getCreatureInfo()->dmove);
   weapon_caracteristics.weapon_power = 1;
   weapon_caracteristics.weapon_mass = 20;
   weapon_caracteristics.weapon_range = 60;
   weapon_physic_body = cocos2d::PhysicsBody::createEdgeBox(cocos2d::Size(weapon_sprite->getBoundingBox().size.width/2,weapon_sprite->getBoundingBox().size.height/2));
   weapon_physic_body->setDynamic(false);
   weapon_physic_body->setGravityEnable(false);
   weapon_physic_body->setCollisionBitmask(weapon_owner->getCreatureSprite()->getPhysicsBody()->getCollisionBitmask());
   weapon_physic_body->setContactTestBitmask(0xFF);
   weapon_physic_body->setMass(weapon_caracteristics.weapon_mass);
   weapon_sprite->setPhysicsBody(weapon_physic_body);
   weapon_sprite->setScale(5);
}

Spear::Spear(std::string weapon_sprite_path, Creature* weapon_owner) :
   Weapon(weapon_sprite_path,weapon_owner){
   this->weapon_owner_dirmove = &(weapon_owner->getCreatureInfo()->dmove);
   weapon_caracteristics.weapon_power = 1;
   weapon_caracteristics.weapon_mass = 20;
   weapon_caracteristics.weapon_range = 100;//Work on ranges
   weapon_physic_body = cocos2d::PhysicsBody::createEdgeBox(cocos2d::Size(weapon_sprite->getBoundingBox().size.width/2,weapon_sprite->getBoundingBox().size.height));
   weapon_physic_body->setDynamic(false);
   weapon_physic_body->setGravityEnable(false);
   weapon_physic_body->setCollisionBitmask(weapon_owner->getCreatureSprite()->getPhysicsBody()->getCollisionBitmask());
   weapon_physic_body->setContactTestBitmask(0xFF);
   weapon_physic_body->setMass(weapon_caracteristics.weapon_mass);
   weapon_sprite->setPhysicsBody(weapon_physic_body);
   weapon_sprite->setScaleX(5);
   weapon_sprite->setScaleY(8);
}
