#include "Weapon.h"
#include "Creature.h"
#include "engMacros.hpp"
#include "gameUI.h"

Weapon::Weapon(std::string weapon_sprite_path,cocos2d::Sprite* weapon_owner_sprite){
   weapon_sprite = cocos2d::Sprite::createWithSpriteFrameName(weapon_sprite_path);
   weapon_physic_body = cocos2d::PhysicsBody::createEdgeBox(weapon_sprite->getBoundingBox().size/2);
   weapon_physic_body->setDynamic(false);
   weapon_physic_body->setGravityEnable(false);
   weapon_physic_body->setMass(10);
   weapon_physic_body->setCollisionBitmask(weapon_owner_sprite->getPhysicsBody()->getCollisionBitmask());
   weapon_physic_body->setContactTestBitmask(0xFF);
   weapon_sprite->setPhysicsBody(weapon_physic_body);
   weapon_sprite->setAnchorPoint(weapon_sprite->getPosition().ANCHOR_MIDDLE_BOTTOM);
   weapon_sprite->setScale(5);
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
void Weapon::attacke(){
   isAttack = true;
   /*Get weapon owner's direction of movement */
   CreatureInfo::DMove dirctionMove = *(weapon_owner_dirmove);
   /*For running action*/
   float angleOfAttacke;
   //where weapon will be, related of owner
   weapon_sprite->setPosition(weapon_owner_sprite->getPosition());

   /*Here make some animations/actions and calculate hit box of weapon*/ 
   switch(ControlAttc::getDirectionAttacke()){
      case DirectionAttacke::TOP_TO_DOWN:{
         weapon_sprite->setRotation(0);
         if (dirctionMove == CreatureInfo::DMove::RIGHT){
            weapon_sprite->setFlippedX(false);
            angleOfAttacke = 180;
         }
         else if (dirctionMove == CreatureInfo::DMove::LEFT){
            weapon_sprite->setFlippedX(true);
            angleOfAttacke = -180;
         }
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::RotateTo::create(0.1,angleOfAttacke),
                                                            cocos2d::RotateTo::create(0,0),
                                                            nullptr));
         break;
      }
      case DirectionAttacke::DOWN_TO_TOP:{
         weapon_sprite->setRotation(180);
         if (dirctionMove == CreatureInfo::DMove::RIGHT){
            weapon_sprite->setFlippedX(false);
            angleOfAttacke = 0;
         }
         else if (dirctionMove == CreatureInfo::DMove::LEFT){
            weapon_sprite->setFlippedX(true);
            angleOfAttacke = 360;
         }
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::RotateTo::create(0.1,angleOfAttacke),
                                                            cocos2d::RotateTo::create(0,0),
                                                            nullptr));
         break;
      }
      case DirectionAttacke::LEFT_TO_RIGHT:{
         weapon_sprite->setFlippedX(false);
         float forMoveBack;
         float forMoveForward;
         weapon_sprite->setRotation(90);

         forMoveBack = -50;
         forMoveForward = 100;
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Sequence::create(cocos2d::MoveBy::create(0.3,cocos2d::Vec2(forMoveBack,0)),cocos2d::MoveBy::create(0.1,cocos2d::Vec2(forMoveForward,0)),nullptr),
                                                            cocos2d::MoveTo::create(0,weapon_owner_sprite->getPosition()),
                                                            nullptr));
         break;
      }
      case DirectionAttacke::RIGHT_TO_LEFT:{
         float forMoveBack;
         float forMoveForward;
         weapon_sprite->setFlippedX(true);
         weapon_sprite->setRotation(270);
         forMoveBack = 50;
         forMoveForward = -100;
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Sequence::create(cocos2d::MoveBy::create(0.3,cocos2d::Vec2(forMoveBack,0)),cocos2d::MoveBy::create(0.1,cocos2d::Vec2(forMoveForward,0)),nullptr),
                                                            cocos2d::MoveTo::create(0,weapon_owner_sprite->getPosition()),
                                                            nullptr));
         break;
      }
      case DirectionAttacke::BOTTOMLEFT_TO_TOPRIGHT:{
         weapon_sprite->setRotation(180);
         float sckewF;
         if (dirctionMove == CreatureInfo::DMove::RIGHT){
            weapon_sprite->setFlippedX(false);
            angleOfAttacke = 90;
            sckewF = 40;
         }
         else if (dirctionMove == CreatureInfo::DMove::LEFT){
            weapon_sprite->setFlippedX(true);
            angleOfAttacke = 330;
            sckewF = -40;
         }
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::RotateTo::create(0.1,angleOfAttacke),cocos2d::SkewBy::create(0.2,sckewF,0),nullptr),
                                                            cocos2d::Spawn::create(cocos2d::RotateTo::create(0.5,360),cocos2d::SkewBy::create(0.2,sckewF*-1,0),nullptr),
                                                            nullptr));
         break;
      }
      case DirectionAttacke::BOTTOMRIGHT_TO_TOPLEFT:{
         weapon_sprite->setRotation(180);
         float sckewF;
         if (dirctionMove == CreatureInfo::DMove::RIGHT){
            weapon_sprite->setFlippedX(false);
            angleOfAttacke = 90;
            sckewF = 40;
         }
         else if (dirctionMove == CreatureInfo::DMove::LEFT){
            weapon_sprite->setFlippedX(true);
            angleOfAttacke = 330;
            sckewF = -40;
         }
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::RotateTo::create(0.1,angleOfAttacke),cocos2d::SkewBy::create(0.2,sckewF,0),nullptr),
                                                            cocos2d::Spawn::create(cocos2d::RotateTo::create(0.5,360),cocos2d::SkewBy::create(0.2,sckewF*-1,0),nullptr),
                                                            nullptr));
         break;
      }
      case DirectionAttacke::TOPLEFT_TO_BOTTOMRIGHT:{
         weapon_sprite->setRotation(0);
         float sckewF;
         if (dirctionMove == CreatureInfo::DMove::RIGHT){
            weapon_sprite->setFlippedX(false);
            angleOfAttacke = 90;
            sckewF = 40;
         }
         else if (dirctionMove == CreatureInfo::DMove::LEFT){
            weapon_sprite->setFlippedX(true);
            angleOfAttacke = -90;
            sckewF = -40;
         }
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::RotateTo::create(0.1,angleOfAttacke),cocos2d::SkewBy::create(0.2,sckewF,0),nullptr),
                                                            cocos2d::Spawn::create(cocos2d::RotateTo::create(0,0),cocos2d::SkewBy::create(0.2,sckewF*-1,0),nullptr),
                                                            nullptr));
         break;
      }
      case DirectionAttacke::TOPRIGHT_TO_BOTTOMLEFT:{
         weapon_sprite->setRotation(0);
         float sckewF;
         if (dirctionMove == CreatureInfo::DMove::RIGHT){
            weapon_sprite->setFlippedX(false);
            angleOfAttacke = 90;
            sckewF = 40;
         }
         else if (dirctionMove == CreatureInfo::DMove::LEFT){
            weapon_sprite->setFlippedX(true);
            angleOfAttacke = -90;
            sckewF = -40;
         }
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::RotateTo::create(0.1,angleOfAttacke),cocos2d::SkewBy::create(0.2,sckewF,0),nullptr),
                                                            cocos2d::Spawn::create(cocos2d::RotateTo::create(0,0),cocos2d::SkewBy::create(0.2,sckewF*-1,0),nullptr),
                                                            nullptr));
         break;
      }
   }
}
void Weapon::update(){
   weapon_sprite->runAction(cocos2d::MoveTo::create(0.1,weapon_owner_sprite->getPosition()));
   weapon_sprite->getPhysicsBody()->setVelocity(weapon_owner_sprite->getPhysicsBody()->getVelocity());
}

void Weapon::giveEffect(Creature* target_creature){
   Creature* target = target_creature;
   /*Instead of this you have to put here attack commbo, not just direction of attack*/
   switch (ControlAttc::getDirectionAttacke()){
      case DirectionAttacke::TOP_TO_DOWN:{
         if (weapon_caracteristics.weapon_penetratingPower > target->getPartCreature()->find(PartCreatureType::TOP)->second->penetrationDef){
            int newDensity = target->getPartCreature()->find(PartCreatureType::TOP)->second->densityDef - weapon_caracteristics.weapon_cuttinPower;
            if (newDensity < 0) newDensity = 0;
            target->getPartCreature()->find(PartCreatureType::TOP)->second->status = PartCreatureStatus::WONDED;
            target->getPartCreature()->find(PartCreatureType::TOP)->second->densityDef = newDensity;
         }
         else if (weapon_caracteristics.weapon_cuttinPower > target->getPartCreature()->find(PartCreatureType::TOP)->second->crushingDef){
            int newCrushing   = target->getPartCreature()->find(PartCreatureType::TOP)->second->crushingDef - weapon_caracteristics.weapon_crushingPower;
            if (newCrushing < 0) newCrushing = 0;
            target->getPartCreature()->find(PartCreatureType::TOP)->second->status = PartCreatureStatus::WONDED;
            target->getPartCreature()->find(PartCreatureType::TOP)->second->crushingDef = newCrushing;
         }
         else{
            //Some miss debuffs
         }
         break;
      }
      case DirectionAttacke::DOWN_TO_TOP:{
         if (weapon_caracteristics.weapon_penetratingPower > target->getPartCreature()->find(PartCreatureType::BOTTOM)->second->penetrationDef){
            int newDensity = target->getPartCreature()->find(PartCreatureType::BOTTOM)->second->densityDef - weapon_caracteristics.weapon_cuttinPower;
            if (newDensity < 0) newDensity = 0;
            target->getPartCreature()->find(PartCreatureType::BOTTOM)->second->status = PartCreatureStatus::WONDED;
            target->getPartCreature()->find(PartCreatureType::BOTTOM)->second->densityDef = newDensity;
         }
         else if (weapon_caracteristics.weapon_cuttinPower > target->getPartCreature()->find(PartCreatureType::BOTTOM)->second->crushingDef){
            int newCrushing   = target->getPartCreature()->find(PartCreatureType::BOTTOM)->second->crushingDef - weapon_caracteristics.weapon_crushingPower;
            if (newCrushing < 0) newCrushing = 0;
            target->getPartCreature()->find(PartCreatureType::BOTTOM)->second->status = PartCreatureStatus::WONDED;
            target->getPartCreature()->find(PartCreatureType::BOTTOM)->second->crushingDef = newCrushing;
         }
         else{
            //Some miss debuffs
         }
         break;
      }
      case DirectionAttacke::LEFT_TO_RIGHT:{
         if (weapon_caracteristics.weapon_penetratingPower > target->getPartCreature()->find(PartCreatureType::MIDDLE)->second->penetrationDef){
            int newDensity = target->getPartCreature()->find(PartCreatureType::MIDDLE)->second->densityDef - weapon_caracteristics.weapon_cuttinPower;
            if (newDensity < 0) newDensity = 0;
            target->getPartCreature()->find(PartCreatureType::MIDDLE)->second->status = PartCreatureStatus::WONDED;
            target->getPartCreature()->find(PartCreatureType::MIDDLE)->second->densityDef = newDensity;
         }
         else if (weapon_caracteristics.weapon_cuttinPower > target->getPartCreature()->find(PartCreatureType::MIDDLE)->second->crushingDef){
            int newCrushing   = target->getPartCreature()->find(PartCreatureType::MIDDLE)->second->crushingDef - weapon_caracteristics.weapon_crushingPower;
            if (newCrushing < 0) newCrushing = 0;
            target->getPartCreature()->find(PartCreatureType::MIDDLE)->second->status = PartCreatureStatus::WONDED;
            target->getPartCreature()->find(PartCreatureType::MIDDLE)->second->crushingDef = newCrushing;
         }
         else{
            //Some miss debuffs
         }
         break;
      }
      case DirectionAttacke::RIGHT_TO_LEFT:{
         if (weapon_caracteristics.weapon_penetratingPower > target->getPartCreature()->find(PartCreatureType::MIDDLE)->second->penetrationDef){
            int newDensity = target->getPartCreature()->find(PartCreatureType::MIDDLE)->second->densityDef - weapon_caracteristics.weapon_cuttinPower;
            if (newDensity < 0) newDensity = 0;
            target->getPartCreature()->find(PartCreatureType::MIDDLE)->second->status = PartCreatureStatus::WONDED;
            target->getPartCreature()->find(PartCreatureType::MIDDLE)->second->densityDef = newDensity;
         }
         else if (weapon_caracteristics.weapon_cuttinPower > target->getPartCreature()->find(PartCreatureType::MIDDLE)->second->crushingDef){
            int newCrushing   = target->getPartCreature()->find(PartCreatureType::MIDDLE)->second->crushingDef - weapon_caracteristics.weapon_crushingPower;
            if (newCrushing < 0) newCrushing = 0;
            target->getPartCreature()->find(PartCreatureType::MIDDLE)->second->status = PartCreatureStatus::WONDED;
            target->getPartCreature()->find(PartCreatureType::MIDDLE)->second->crushingDef = newCrushing;
         }
         else{
            //Some miss debuffs
         }
         break;
      }
   }
}
void Weapon::takeEffect(Creature* owner){
   owner->getCreatureInfo()->characteristic.stamina = owner->getCreatureInfo()->characteristic.stamina;
   owner->getCreatureInfo()->characteristic.stamina -= 10;//Here make some effects
}
void Weapon::setCaracteristics(uint w_cutP,uint w_penP,uint w_crushP,uint w_sol,uint w_mass = 10){
   this->weapon_caracteristics.weapon_crushingPower = w_crushP;
   this->weapon_caracteristics.weapon_cuttinPower = w_cutP;
   this->weapon_caracteristics.weapon_penetratingPower = w_penP;
   this->weapon_caracteristics.weapon_solidity = w_sol;
   this->weapon_caracteristics.weapon_mass = w_mass;
}


Sword::Sword(std::string weapon_sprite_path,cocos2d::Sprite* weapon_owner_sprite, void* owner_obj):
   Weapon(weapon_sprite_path,weapon_owner_sprite){
   this->weapon_owner_sprite = weapon_owner_sprite;
   this->weapon_owner_dirmove = &static_cast<Creature*>(owner_obj)->getCreatureInfo()->dmove;
   weapon_caracteristics.weapon_cuttinPower = 10;
   weapon_caracteristics.weapon_penetratingPower = 5;
   weapon_caracteristics.weapon_crushingPower =  1;
   weapon_caracteristics.weapon_solidity = 30;
   weapon_caracteristics.weapon_mass = 20;
   weapon_caracteristics.weapon_range = 110;
   weapon_physic_body->setMass(weapon_caracteristics.weapon_mass);
}

Axe::Axe(std::string weapon_sprite_path,cocos2d::Sprite* weapon_owner_sprite, void* owner_obj):
   Weapon(weapon_sprite_path,weapon_owner_sprite){
   this->weapon_owner_sprite = weapon_owner_sprite;
   this->weapon_owner_dirmove = &static_cast<Creature*>(owner_obj)->getCreatureInfo()->dmove;
   weapon_caracteristics.weapon_cuttinPower = 10;
   weapon_caracteristics.weapon_penetratingPower = 10;
   weapon_caracteristics.weapon_crushingPower =  15;
   weapon_caracteristics.weapon_solidity = 10;
   weapon_caracteristics.weapon_mass = 20;
   weapon_caracteristics.weapon_range = 90;
   weapon_physic_body->setMass(weapon_caracteristics.weapon_mass);
}

Spear::Spear(std::string weapon_sprite_path,cocos2d::Sprite* weapon_owner_sprite, void* owner_obj) :
   Weapon(weapon_sprite_path,weapon_owner_sprite){
   this->weapon_owner_sprite = weapon_owner_sprite;
   this->weapon_owner_dirmove = &static_cast<Creature*>(owner_obj)->getCreatureInfo()->dmove;
   weapon_caracteristics.weapon_cuttinPower = 5;
   weapon_caracteristics.weapon_penetratingPower = 20;
   weapon_caracteristics.weapon_crushingPower =  10;
   weapon_caracteristics.weapon_solidity = 12;
   weapon_caracteristics.weapon_mass = 20;
   weapon_caracteristics.weapon_range = 110;
   weapon_physic_body->setMass(weapon_caracteristics.weapon_mass);
}
