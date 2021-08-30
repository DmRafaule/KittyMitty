#include "Weapon.h"
#include "Creature.h"
#include "engMacros.hpp"
#include "gameUI.h"

Weapon::Weapon(std::string weapon_sprite_path,cocos2d::Sprite* weapon_owner_sprite){
   weapon_mass = 10;
   weapon_sprite = cocos2d::Sprite::createWithSpriteFrameName(weapon_sprite_path);
   //weapon_physic_body = cocos2d::PhysicsBody::createEdgeBox(weapon_sprite->getBoundingBox().size);
   //weapon_physic_body->setMass(weapon_mass);
   //weapon_physic_body->setDynamic(false);
   //weapon_physic_body->setGravityEnable(true);
   //weapon_sprite->setPhysicsBody(weapon_physic_body);
   weapon_sprite->setAnchorPoint(weapon_sprite->getPosition().ANCHOR_MIDDLE_BOTTOM);
   weapon_sprite->setScale(5);
   cocos2d::Texture2D::TexParams tpar = {
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE
    };
   weapon_sprite->getTexture()->setTexParameters(tpar);
   /*Set up "hit box" for this weapon(for detect a collision with creatures and others thin*/
   weapon_damage_hitbox = cocos2d::Sprite::create();
   weapon_damage_hitbox->setTexture("textures/player.png");
   weapon_damage_hitbox->setVisible(false);//After all debug shit set false
   weapon_damage_hitbox->setScale(MAX(weapon_sprite->getBoundingBox().size.width/weapon_damage_hitbox->getBoundingBox().size.width,
                                      weapon_sprite->getBoundingBox().size.height/weapon_damage_hitbox->getBoundingBox().size.height));
}
Weapon::~Weapon(){}
void Weapon::attacke(){
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
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x + 40,
                                              weapon_sprite->getPosition().y);
            angleOfAttacke = 180;
         }
         else if (dirctionMove == CreatureInfo::DMove::LEFT){
            weapon_sprite->setFlippedX(true);
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x - 40,
                                              weapon_sprite->getPosition().y);
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
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x + 40,
                                              weapon_sprite->getPosition().y);
            angleOfAttacke = 0;
         }
         else if (dirctionMove == CreatureInfo::DMove::LEFT){
            weapon_sprite->setFlippedX(true);
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x - 40,
                                              weapon_sprite->getPosition().y);
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
         weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x + weapon_sprite->getBoundingBox().size.width/2 + 40,
                                           weapon_sprite->getPosition().y);
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
         weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x - weapon_sprite->getBoundingBox().size.width/2 - 40,
                                           weapon_sprite->getPosition().y);
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
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x + 40,
                                              weapon_sprite->getPosition().y);
            angleOfAttacke = 90;
            sckewF = 40;
         }
         else if (dirctionMove == CreatureInfo::DMove::LEFT){
            weapon_sprite->setFlippedX(true);
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x - 40,
                                              weapon_sprite->getPosition().y);
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
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x + 40,
                                              weapon_sprite->getPosition().y);
            angleOfAttacke = 90;
            sckewF = 40;
         }
         else if (dirctionMove == CreatureInfo::DMove::LEFT){
            weapon_sprite->setFlippedX(true);
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x - 40,
                                              weapon_sprite->getPosition().y);
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
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x + 40,
                                              weapon_sprite->getPosition().y);
            angleOfAttacke = 90;
            sckewF = 40;
         }
         else if (dirctionMove == CreatureInfo::DMove::LEFT){
            weapon_sprite->setFlippedX(true);
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x - 40,
                                              weapon_sprite->getPosition().y);
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
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x + 40,
                                              weapon_sprite->getPosition().y);
            angleOfAttacke = 90;
            sckewF = 40;
         }
         else if (dirctionMove == CreatureInfo::DMove::LEFT){
            weapon_sprite->setFlippedX(true);
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x - 40,
                                              weapon_sprite->getPosition().y);
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
void Weapon::defend(){

}
void Weapon::parry(){

}
void Weapon::update(){
   weapon_sprite->runAction(cocos2d::MoveTo::create(0.1,weapon_owner_sprite->getPosition()));
}

void Weapon::giveEffect(void* target_creature){
   Creature* target = static_cast<Creature*>(target_creature);
   /*Instead of this you have to put here attack commbo, not just direction of attack*/
   switch (ControlAttc::getDirectionAttacke()){
      case DirectionAttacke::TOP_TO_DOWN:{
         switch (ControlTargeting::getTarget()){
         case PartCreatureType::HEAD:{
               //Hit head
            if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::HEAD,PartCreatureField::PENETRATION)){
               int newDensity     = target->getPart(PartCreatureType::HEAD,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
               if (newDensity < 0) newDensity = 0;
               target->setPart(PartCreatureType::HEAD,PartCreatureStatus::WONDED,newDensity);
               target->setOrgan(PartCreatureType::HEAD,PartOrganType::BRAIN,PartCreatureStatus::WONDED);
            }
            else if (weapon_caracteristics.weapon_cuttinPower > target->getPart(PartCreatureType::HEAD,PartCreatureField::CRUSHING)){
               int newCrushing   =  target->getPart(PartCreatureType::HEAD,PartCreatureField::CRUSHING)     - weapon_caracteristics.weapon_crushingPower;
               if (newCrushing < 0) newCrushing = 0;
               target->setPart(PartCreatureType::HEAD,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::HEAD,PartCreatureField::CRUSHING));
               target->setOrgan(PartCreatureType::HEAD,PartOrganType::BRAIN,PartCreatureStatus::WONDED);
            }
            else{
               //Some miss debuffs
            }
            break;
         }
         case PartCreatureType::UPPER_TORSE:{
            //Hit upper torse
            if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::PENETRATION)){
               int newDensity     = target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
               if (newDensity < 0) newDensity = 0;
               target->setPart(PartCreatureType::UPPER_TORSE,PartCreatureStatus::WONDED,newDensity);
            }
            else if (weapon_caracteristics.weapon_cuttinPower > target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::CRUSHING)){
               int newCrushing   =  target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::CRUSHING)     - weapon_caracteristics.weapon_crushingPower;
               if (newCrushing < 0) newCrushing = 0;
               target->setPart(PartCreatureType::UPPER_TORSE,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::CRUSHING));
               target->setOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::LUNGS,PartCreatureStatus::WONDED);
               target->setOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::HEART,PartCreatureStatus::WONDED);
            }
            else{
               //Some miss debuffs
            }
            break;
         }
         }
         break;
      }
      case DirectionAttacke::DOWN_TO_TOP:{
         switch(ControlTargeting::getTarget()){
         case PartCreatureType::LEG_LEFT:{
            //Hit leg left
            if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::LEG_LEFT,PartCreatureField::PENETRATION)){
               int newDensity     = target->getPart(PartCreatureType::LEG_LEFT,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
               if (newDensity < 0) newDensity = 0;
               target->setPart(PartCreatureType::LEG_LEFT,PartCreatureStatus::WONDED,newDensity);
            }
            else if (weapon_caracteristics.weapon_crushingPower > target->getPart(PartCreatureType::LEG_LEFT,PartCreatureField::CRUSHING)){
               target->setPart(PartCreatureType::LEG_LEFT,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::LEG_LEFT,PartCreatureField::CRUSHING));
            }
            break;
         }
         case PartCreatureType::LEG_RIGHT:{
            //Hit leg right
            if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::LEG_RIGHT,PartCreatureField::PENETRATION)){
               int newDensity     = target->getPart(PartCreatureType::LEG_RIGHT,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
               if (newDensity < 0) newDensity = 0;
               target->setPart(PartCreatureType::LEG_RIGHT,PartCreatureStatus::WONDED,newDensity);
            }
            else if (weapon_caracteristics.weapon_crushingPower > target->getPart(PartCreatureType::LEG_RIGHT,PartCreatureField::CRUSHING)){
               target->setPart(PartCreatureType::LEG_RIGHT,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::LEG_RIGHT,PartCreatureField::CRUSHING));
            }
            break;
         }
         case PartCreatureType::BUTTOM_TORSE:{
               //Hit bottom torse
               if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::PENETRATION)){
                  int newDensity     = target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
                  if (newDensity < 0) newDensity = 0;
                  target->setPart(PartCreatureType::BUTTOM_TORSE,PartCreatureStatus::WONDED,newDensity);
               }
               else if (weapon_caracteristics.weapon_crushingPower > target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::CRUSHING)){
                  int newCrushing   =  target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::CRUSHING)     - weapon_caracteristics.weapon_crushingPower;
                  if (newCrushing < 0) newCrushing = 0;
                  target->setPart(PartCreatureType::BUTTOM_TORSE,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::CRUSHING));
                  target->setOrgan(PartCreatureType::BUTTOM_TORSE,PartOrganType::GUT,PartCreatureStatus::WONDED);
               }
               else{
                  //Some miss debuffs
               }
               break;
            }
         }
         break;
      }
      case DirectionAttacke::LEFT_TO_RIGHT:{
         switch(ControlTargeting::getTarget()){
            case PartCreatureType::HEAD:{
                 //Hit head
               if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::HEAD,PartCreatureField::PENETRATION)){
                  int newDensity     = target->getPart(PartCreatureType::HEAD,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
                  if (newDensity < 0) newDensity = 0;
                  target->setPart(PartCreatureType::HEAD,PartCreatureStatus::WONDED,newDensity);
                  target->setOrgan(PartCreatureType::HEAD,PartOrganType::BRAIN,PartCreatureStatus::WONDED);
               }
               else if (weapon_caracteristics.weapon_cuttinPower > target->getPart(PartCreatureType::HEAD,PartCreatureField::CRUSHING)){
                  int newCrushing   =  target->getPart(PartCreatureType::HEAD,PartCreatureField::CRUSHING)     - weapon_caracteristics.weapon_crushingPower;
                  if (newCrushing < 0) newCrushing = 0;
                  target->setPart(PartCreatureType::HEAD,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::HEAD,PartCreatureField::CRUSHING));
                  target->setOrgan(PartCreatureType::HEAD,PartOrganType::BRAIN,PartCreatureStatus::WONDED);
               }
               else{
                  //Some miss debuffs
               }
               break;
            }
            case PartCreatureType::UPPER_TORSE:{
               //Hit upper torse
               if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::PENETRATION)){
                  int newDensity     = target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
                  if (newDensity < 0) newDensity = 0;
                  target->setPart(PartCreatureType::UPPER_TORSE,PartCreatureStatus::WONDED,newDensity);
               }
               else if (weapon_caracteristics.weapon_crushingPower > target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::CRUSHING)){
                  int newCrushing   =  target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::CRUSHING)     - weapon_caracteristics.weapon_crushingPower;
                  if (newCrushing < 0) newCrushing = 0;
                  target->setPart(PartCreatureType::UPPER_TORSE,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::CRUSHING));
                  target->setOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::LUNGS,PartCreatureStatus::WONDED);
                  target->setOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::HEART,PartCreatureStatus::WONDED);
               }
               else{
                  //Some miss debuffs
               }
               break;
            }
            case PartCreatureType::BUTTOM_TORSE:{
               //Hit bottom torse
               if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::PENETRATION)){
                  int newDensity     = target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
                  if (newDensity < 0) newDensity = 0;
                  target->setPart(PartCreatureType::BUTTOM_TORSE,PartCreatureStatus::WONDED,newDensity);
               }
               else if (weapon_caracteristics.weapon_crushingPower > target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::CRUSHING)){
                  int newCrushing   =  target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::CRUSHING)     - weapon_caracteristics.weapon_crushingPower;
                  if (newCrushing < 0) newCrushing = 0;
                  target->setPart(PartCreatureType::BUTTOM_TORSE,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::CRUSHING));
                  target->setOrgan(PartCreatureType::BUTTOM_TORSE,PartOrganType::GUT,PartCreatureStatus::WONDED);
               }
               else{
                  //Some miss debuffs
               }
               break;
            }
         }
         break;
      }
      case DirectionAttacke::RIGHT_TO_LEFT:{
         switch(ControlTargeting::getTarget()){
            case PartCreatureType::UPPER_TORSE:{
               //Hit upper torse
               if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::PENETRATION)){
                  int newDensity     = target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
                  if (newDensity < 0) newDensity = 0;
                  target->setPart(PartCreatureType::UPPER_TORSE,PartCreatureStatus::WONDED,newDensity);
               }
               else if (weapon_caracteristics.weapon_crushingPower > target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::CRUSHING)){
                  int newCrushing   =  target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::CRUSHING)     - weapon_caracteristics.weapon_crushingPower;
                  if (newCrushing < 0) newCrushing = 0;
                  target->setPart(PartCreatureType::UPPER_TORSE,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::CRUSHING));
                  target->setOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::LUNGS,PartCreatureStatus::WONDED);
                  target->setOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::HEART,PartCreatureStatus::WONDED);
               }
               else{
                  //Some miss debuffs
               }
               break;
            }
            case PartCreatureType::BUTTOM_TORSE:{
               //Hit bottom torse
               if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::PENETRATION)){
                  int newDensity     = target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
                  if (newDensity < 0) newDensity = 0;
                  target->setPart(PartCreatureType::BUTTOM_TORSE,PartCreatureStatus::WONDED,newDensity);
               }
               else if (weapon_caracteristics.weapon_crushingPower > target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::CRUSHING)){
                  int newCrushing   =  target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::CRUSHING)     - weapon_caracteristics.weapon_crushingPower;
                  if (newCrushing < 0) newCrushing = 0;
                  target->setPart(PartCreatureType::BUTTOM_TORSE,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::CRUSHING));
                  target->setOrgan(PartCreatureType::BUTTOM_TORSE,PartOrganType::GUT,PartCreatureStatus::WONDED);
               }
               else{
                  //Some miss debuffs
               }
               break;
            }
         }
         break;
      }
      case DirectionAttacke::BOTTOMLEFT_TO_TOPRIGHT:{
         switch(ControlTargeting::getTarget()){
            case PartCreatureType::LEG_LEFT:{
               //Hit leg right
               if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::LEG_LEFT,PartCreatureField::PENETRATION)){
                  int newDensity     = target->getPart(PartCreatureType::LEG_LEFT,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
                  if (newDensity < 0) newDensity = 0;
                  target->setPart(PartCreatureType::LEG_LEFT,PartCreatureStatus::WONDED,newDensity);
               }
               else if (weapon_caracteristics.weapon_crushingPower > target->getPart(PartCreatureType::LEG_LEFT,PartCreatureField::CRUSHING)){
                  target->setPart(PartCreatureType::LEG_LEFT,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::LEG_LEFT,PartCreatureField::CRUSHING));
               }
               else{

               }
               break;
            }
            case PartCreatureType::BUTTOM_TORSE:{
               //Hit bottom torse
               if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::PENETRATION)){
                  int newDensity     = target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
                  if (newDensity < 0) newDensity = 0;
                  target->setPart(PartCreatureType::BUTTOM_TORSE,PartCreatureStatus::WONDED,newDensity);
               }
               else if (weapon_caracteristics.weapon_crushingPower > target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::CRUSHING)){
                  target->setPart(PartCreatureType::BUTTOM_TORSE,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::CRUSHING));
                  target->setOrgan(PartCreatureType::BUTTOM_TORSE,PartOrganType::GUT,PartCreatureStatus::WONDED);
               }
               else{
                  //Some miss debuffs
               }
               break;
            }
         }
         break;
      }
      case DirectionAttacke::BOTTOMRIGHT_TO_TOPLEFT:{
         switch(ControlTargeting::getTarget()){
            case PartCreatureType::LEG_RIGHT:{
               //Hit leg right
               if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::LEG_RIGHT,PartCreatureField::PENETRATION)){
                  int newDensity     = target->getPart(PartCreatureType::LEG_RIGHT,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
                  if (newDensity < 0) newDensity = 0;
                  target->setPart(PartCreatureType::LEG_RIGHT,PartCreatureStatus::WONDED,newDensity);
               }
               else if (weapon_caracteristics.weapon_crushingPower > target->getPart(PartCreatureType::LEG_RIGHT,PartCreatureField::CRUSHING)){
                  target->setPart(PartCreatureType::LEG_RIGHT,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::LEG_RIGHT,PartCreatureField::CRUSHING));
               }
               else{

               }
               break;
            }
            case PartCreatureType::BUTTOM_TORSE:{
               //Hit bottom torse
               if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::PENETRATION)){
                  int newDensity     = target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
                  if (newDensity < 0) newDensity = 0;
                  target->setPart(PartCreatureType::BUTTOM_TORSE,PartCreatureStatus::WONDED,newDensity);
               }
               else if (weapon_caracteristics.weapon_crushingPower > target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::CRUSHING)){
                  target->setPart(PartCreatureType::BUTTOM_TORSE,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::BUTTOM_TORSE,PartCreatureField::CRUSHING));
                  target->setOrgan(PartCreatureType::BUTTOM_TORSE,PartOrganType::GUT,PartCreatureStatus::WONDED);
               }
               else{
                  //Some miss debuffs
               }
               break;
            }
         }
         break;
      }
      case DirectionAttacke::TOPLEFT_TO_BOTTOMRIGHT:{
         switch(ControlTargeting::getTarget()){
            case PartCreatureType::HEAD:{
            if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::HEAD,PartCreatureField::PENETRATION)){
               int newDensity     = target->getPart(PartCreatureType::HEAD,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
               if (newDensity < 0) newDensity = 0;
               target->setPart(PartCreatureType::HEAD,PartCreatureStatus::WONDED,newDensity);
               target->setOrgan(PartCreatureType::HEAD,PartOrganType::BRAIN,PartCreatureStatus::WONDED);
            }
            else if (weapon_caracteristics.weapon_cuttinPower > target->getPart(PartCreatureType::HEAD,PartCreatureField::CRUSHING)){
               int newCrushing   =  target->getPart(PartCreatureType::HEAD,PartCreatureField::CRUSHING)     - weapon_caracteristics.weapon_crushingPower;
               if (newCrushing < 0) newCrushing = 0;
               target->setPart(PartCreatureType::HEAD,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::HEAD,PartCreatureField::CRUSHING));
               target->setOrgan(PartCreatureType::HEAD,PartOrganType::BRAIN,PartCreatureStatus::WONDED);
            }
            else{
               //Some miss debuffs
            }
               break;
            }
            case PartCreatureType::UPPER_TORSE:{
               if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::PENETRATION)){
                  int newDensity     = target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
                  if (newDensity < 0) newDensity = 0;
                  target->setPart(PartCreatureType::UPPER_TORSE,PartCreatureStatus::WONDED,newDensity);
               }
               else if (weapon_caracteristics.weapon_cuttinPower > target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::CRUSHING)){
                  int newCrushing   =  target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::CRUSHING)     - weapon_caracteristics.weapon_crushingPower;
                  if (newCrushing < 0) newCrushing = 0;
                  target->setPart(PartCreatureType::UPPER_TORSE,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::CRUSHING));
                  target->setOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::LUNGS,PartCreatureStatus::WONDED);
                  target->setOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::HEART,PartCreatureStatus::WONDED);
               }
               else{
                  //Some miss debuffs
               }
               break;
            }
            case PartCreatureType::HAND_LEFT:{
               if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::HAND_LEFT,PartCreatureField::PENETRATION)){
                  int newDensity     = target->getPart(PartCreatureType::HAND_LEFT,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
                  if (newDensity < 0) newDensity = 0;
                  target->setPart(PartCreatureType::HAND_LEFT,PartCreatureStatus::WONDED,newDensity);
               }
               else if (weapon_caracteristics.weapon_crushingPower > target->getPart(PartCreatureType::HAND_LEFT,PartCreatureField::CRUSHING)){
                  target->setPart(PartCreatureType::HAND_LEFT,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::HAND_LEFT,PartCreatureField::CRUSHING));
               }
               else{

               }
               break;
            }
         }
         break;
      }
      case DirectionAttacke::TOPRIGHT_TO_BOTTOMLEFT:{
         switch(ControlTargeting::getTarget()){
            case PartCreatureType::HEAD:{
               if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::HEAD,PartCreatureField::PENETRATION)){
                  int newDensity     = target->getPart(PartCreatureType::HEAD,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
                  if (newDensity < 0) newDensity = 0;
                  target->setPart(PartCreatureType::HEAD,PartCreatureStatus::WONDED,newDensity);
                  target->setOrgan(PartCreatureType::HEAD,PartOrganType::BRAIN,PartCreatureStatus::WONDED);
               }
               else if (weapon_caracteristics.weapon_cuttinPower > target->getPart(PartCreatureType::HEAD,PartCreatureField::CRUSHING)){
                  int newCrushing   =  target->getPart(PartCreatureType::HEAD,PartCreatureField::CRUSHING)     - weapon_caracteristics.weapon_crushingPower;
                  if (newCrushing < 0) newCrushing = 0;
                  target->setPart(PartCreatureType::HEAD,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::HEAD,PartCreatureField::CRUSHING));
                  target->setOrgan(PartCreatureType::HEAD,PartOrganType::BRAIN,PartCreatureStatus::WONDED);
               }
               else{
                  //Some miss debuffs
               }
               break;
            }
            case PartCreatureType::UPPER_TORSE:{
               if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::PENETRATION)){
                  int newDensity     = target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
                  if (newDensity < 0) newDensity = 0;
                  target->setPart(PartCreatureType::UPPER_TORSE,PartCreatureStatus::WONDED,newDensity);
               }
               else if (weapon_caracteristics.weapon_cuttinPower > target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::CRUSHING)){
                  int newCrushing   =  target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::CRUSHING)     - weapon_caracteristics.weapon_crushingPower;
                  if (newCrushing < 0) newCrushing = 0;
                  target->setPart(PartCreatureType::UPPER_TORSE,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::UPPER_TORSE,PartCreatureField::CRUSHING));
                  target->setOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::LUNGS,PartCreatureStatus::WONDED);
                  target->setOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::HEART,PartCreatureStatus::WONDED);
               }
               else{
                  //Some miss debuffs
               }
               break;
            }
            case PartCreatureType::HAND_RIGHT:{
               if (weapon_caracteristics.weapon_penetratingPower > target->getPart(PartCreatureType::HAND_RIGHT,PartCreatureField::PENETRATION)){
                  int newDensity     = target->getPart(PartCreatureType::HAND_RIGHT,PartCreatureField::DENSITY)     - weapon_caracteristics.weapon_cuttinPower;
                  if (newDensity < 0) newDensity = 0;
                  target->setPart(PartCreatureType::HAND_RIGHT,PartCreatureStatus::WONDED,newDensity);
               }
               else if (weapon_caracteristics.weapon_crushingPower > target->getPart(PartCreatureType::HAND_RIGHT,PartCreatureField::CRUSHING)){
                  target->setPart(PartCreatureType::HAND_RIGHT,PartCreatureStatus::WONDED,target->getPart(PartCreatureType::HAND_RIGHT,PartCreatureField::CRUSHING));
               }
               else{

               }
               break;
            }
         }
         break;
      }
   }
}
void Weapon::takeEffect(void* owner){
   static_cast<Creature*>(owner)->setCreatureInfo()->characteristic.stamina = static_cast<Creature*>(owner)->getCreatureInfo()->characteristic.stamina;
   //static_cast<Creature*>(owner)->setCreatureStamina(static_cast<Creature*>(owner)->getCreatureInfo()->characteristic.stamina-10);//Here make some effects
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
   this->weapon_owner_dirmove = &static_cast<Creature*>(owner_obj)->setCreatureInfo()->dmove;
   weapon_mass = 10;
   weapon_caracteristics.weapon_cuttinPower = 10;
   weapon_caracteristics.weapon_penetratingPower = 5;
   weapon_caracteristics.weapon_crushingPower =  1;
   weapon_caracteristics.weapon_solidity = 30;
   weapon_caracteristics.weapon_mass = 20;
   weapon_caracteristics.weapon_range = 110;
   //weapon_physic_body->setMass(weapon_caracteristics.weapon_mass);
}

Axe::Axe(std::string weapon_sprite_path,cocos2d::Sprite* weapon_owner_sprite, void* owner_obj):
   Weapon(weapon_sprite_path,weapon_owner_sprite){
   this->weapon_owner_sprite = weapon_owner_sprite;
   this->weapon_owner_dirmove = &static_cast<Creature*>(owner_obj)->setCreatureInfo()->dmove;
   weapon_caracteristics.weapon_cuttinPower = 10;
   weapon_caracteristics.weapon_penetratingPower = 10;
   weapon_caracteristics.weapon_crushingPower =  15;
   weapon_caracteristics.weapon_solidity = 10;
   weapon_caracteristics.weapon_mass = 20;
   weapon_caracteristics.weapon_range = 90;
   //weapon_physic_body->setMass(weapon_caracteristics.weapon_mass);
}

Spear::Spear(std::string weapon_sprite_path,cocos2d::Sprite* weapon_owner_sprite, void* owner_obj) :
   Weapon(weapon_sprite_path,weapon_owner_sprite){
   this->weapon_owner_sprite = weapon_owner_sprite;
   this->weapon_owner_dirmove = &static_cast<Creature*>(owner_obj)->setCreatureInfo()->dmove;
   weapon_mass = 10;
   weapon_caracteristics.weapon_cuttinPower = 5;
   weapon_caracteristics.weapon_penetratingPower = 20;
   weapon_caracteristics.weapon_crushingPower =  10;
   weapon_caracteristics.weapon_solidity = 12;
   weapon_caracteristics.weapon_mass = 20;
   weapon_caracteristics.weapon_range = 110;
   //weapon_physic_body->setMass(weapon_caracteristics.weapon_mass);
}
