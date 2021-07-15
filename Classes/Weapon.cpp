#include "Weapon.h"
#include "gameUI.h"
#include "engMacros.hpp"

Weapon::Weapon(){

}
Weapon::~Weapon(){

}
void Weapon::setCaracteristics(uint w_cutP,uint w_penP,uint w_crushP,uint w_sol){
   this->weapon_caracteristics.weapon_crushingPower = w_crushP;
   this->weapon_caracteristics.weapon_cuttinPower = w_cutP;
   this->weapon_caracteristics.weapon_penetratingPower = w_penP;
   this->weapon_caracteristics.weapon_solidity = w_sol;
}

Sword::Sword(std::string weapon_sprite_path){
   weapon_caracteristics.weapon_cuttinPower = 10;
   weapon_caracteristics.weapon_penetratingPower = 5;
   weapon_caracteristics.weapon_crushingPower =  1;
   weapon_caracteristics.weapon_solidity = 30;
   /*Set up sprite for weapon(how it will looks)*/
   
   weapon_sprite = cocos2d::Sprite::create(weapon_sprite_path);
   weapon_sprite->setColor(cocos2d::Color3B::GREEN);
   weapon_sprite->setVisible(true);//Rem
   weapon_sprite->setAnchorPoint(weapon_sprite->getPosition().ANCHOR_MIDDLE_BOTTOM);
   weapon_sprite->setScale(0.5,1.5);//Rem
   /*Set up "hit box" for this weapon(for detect a collision with creatures and others thin*/
   weapon_damage_hitbox = cocos2d::Sprite::create();
   weapon_damage_hitbox->setTexture("textures/player.png");
   weapon_damage_hitbox->setVisible(true);//After all debug shit set false
   weapon_damage_hitbox->setPosition(100,100);
   weapon_damage_hitbox->setScale(MAX(weapon_sprite->getBoundingBox().size.width/weapon_damage_hitbox->getBoundingBox().size.width,
                                      weapon_sprite->getBoundingBox().size.height/weapon_damage_hitbox->getBoundingBox().size.height));
}
void Sword::attacke(DirectionAttacke direction_attack, cocos2d::Sprite* creature_sprite){
   float angleOfMoveDirection = ControlBall::getAngle();
   float angleOfAttacke;

   /*//After all debug shit remove this lines*/
   weapon_sprite->setVisible(true);
   weapon_sprite->setPosition(creature_sprite->getPosition());
   
   if (direction_attack == DirectionAttacke::TOP_TO_DOWN){
         weapon_sprite->setRotation(0);
         if (angleOfMoveDirection >= 0 && angleOfMoveDirection < 180){
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x + 40,
                                              weapon_sprite->getPosition().y);
            angleOfAttacke = 180;
         }
         else if (angleOfMoveDirection >= 180 && angleOfMoveDirection <= 360){
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x - 40,
                                              weapon_sprite->getPosition().y);
            angleOfAttacke = -180;
         }
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::RotateTo::create(0.1,angleOfAttacke),cocos2d::FadeIn::create(0),nullptr),
                                                            cocos2d::Spawn::create(cocos2d::RotateTo::create(0,0),cocos2d::FadeOut::create(0.1),nullptr),
                                                            nullptr));
      }
   else if (direction_attack == DirectionAttacke::DOWN_TO_TOP){
      weapon_sprite->setRotation(180);
      if (angleOfMoveDirection >= 0 && angleOfMoveDirection < 180){
         weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x + 40,
                                           weapon_sprite->getPosition().y);
         angleOfAttacke = 0;
      }
      else if (angleOfMoveDirection >= 180 && angleOfMoveDirection <= 360){
         weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x - 40,
                                           weapon_sprite->getPosition().y);
         angleOfAttacke = 360;
      }
      weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::RotateTo::create(0.1,angleOfAttacke),cocos2d::FadeIn::create(0),nullptr),
                                                         cocos2d::Spawn::create(cocos2d::RotateTo::create(0,0),cocos2d::FadeOut::create(0.1),nullptr),
                                                         nullptr));
   }
   else if (direction_attack == DirectionAttacke::LEFT_TO_RIGHT){
      float forMoveBack;
      float forMoveForward;
      weapon_sprite->setRotation(90);
      weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x + weapon_sprite->getBoundingBox().size.width/2 + 40,
                                        weapon_sprite->getPosition().y);
      forMoveBack = -50;
      forMoveForward = 100;
      weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::MoveBy::create(0.3,cocos2d::Vec2(forMoveBack,0)),cocos2d::FadeIn::create(0),nullptr),cocos2d::MoveBy::create(0.1,cocos2d::Vec2(forMoveForward,0)),nullptr),
                                                         cocos2d::Spawn::create(cocos2d::MoveTo::create(0,creature_sprite->getPosition()),cocos2d::FadeOut::create(0.1),nullptr),
                                                         nullptr));
   }
   else if (direction_attack == DirectionAttacke::RIGHT_TO_LEFT){
      float forMoveBack;
      float forMoveForward;
      weapon_sprite->setRotation(270);
      weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x - weapon_sprite->getBoundingBox().size.width/2 - 40,
                                        weapon_sprite->getPosition().y);
      forMoveBack = 50;
      forMoveForward = -100;
      weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::MoveBy::create(0.3,cocos2d::Vec2(forMoveBack,0)),cocos2d::FadeIn::create(0),nullptr),cocos2d::MoveBy::create(0.1,cocos2d::Vec2(forMoveForward,0)),nullptr),
                                                         cocos2d::Spawn::create(cocos2d::MoveTo::create(0,creature_sprite->getPosition()),cocos2d::FadeOut::create(0.1),nullptr),
                                                         nullptr));
   }
}

Axe::Axe(std::string weapon_sprite_path){
   weapon_caracteristics.weapon_cuttinPower = 10;
   weapon_caracteristics.weapon_penetratingPower = 10;
   weapon_caracteristics.weapon_crushingPower =  15;
   weapon_caracteristics.weapon_solidity = 10;
   /*Set up sprite for weapon(how it will looks)*/
   
   weapon_sprite = cocos2d::Sprite::create(weapon_sprite_path);
   weapon_sprite->setColor(cocos2d::Color3B::GREEN);//Rem
   weapon_sprite->setVisible(true);//Rem
   weapon_sprite->setAnchorPoint(weapon_sprite->getPosition().ANCHOR_MIDDLE_BOTTOM);
   weapon_sprite->setScale(1,1);//Rem
   /*Set up "hit box" for this weapon(for detect a collision with creatures and others thin*/
   weapon_damage_hitbox = cocos2d::Sprite::create();
   weapon_damage_hitbox->setTexture("textures/player.png");//Chane on hit_box sprite
   weapon_damage_hitbox->setVisible(true);//After all debug shit set false
   weapon_damage_hitbox->setScale(MAX(weapon_sprite->getBoundingBox().size.width/weapon_damage_hitbox->getBoundingBox().size.width,
                                      weapon_sprite->getBoundingBox().size.height/weapon_damage_hitbox->getBoundingBox().size.height));
}
void Axe::attacke(DirectionAttacke direction_attack, cocos2d::Sprite* creature_sprite){
   float angleOfMoveDirection = ControlBall::getAngle();
   float angleOfAttacke;

   /*//After all debug shit remove this lines*/
   weapon_sprite->setVisible(true);
   weapon_sprite->setPosition(creature_sprite->getPosition());
   
   if (direction_attack == DirectionAttacke::TOP_TO_DOWN){
         weapon_sprite->setRotation(0);
         if (angleOfMoveDirection >= 0 && angleOfMoveDirection < 180){
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x + 40,
                                              weapon_sprite->getPosition().y);
            angleOfAttacke = 180;
         }
         else if (angleOfMoveDirection >= 180 && angleOfMoveDirection <= 360){
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x - 40,
                                              weapon_sprite->getPosition().y);
            angleOfAttacke = -180;
         }
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::RotateTo::create(0.1,angleOfAttacke),cocos2d::FadeIn::create(0),nullptr),
                                                            cocos2d::Spawn::create(cocos2d::RotateTo::create(0,0),cocos2d::FadeOut::create(0.1),nullptr),
                                                            nullptr));
      }
   else if (direction_attack == DirectionAttacke::DOWN_TO_TOP){
      weapon_sprite->setRotation(180);
      if (angleOfMoveDirection >= 0 && angleOfMoveDirection < 180){
         weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x + 40,
                                           weapon_sprite->getPosition().y);
         angleOfAttacke = 0;
      }
      else if (angleOfMoveDirection >= 180 && angleOfMoveDirection <= 360){
         weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x - 40,
                                           weapon_sprite->getPosition().y);
         angleOfAttacke = 360;
      }
      weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::RotateTo::create(0.1,angleOfAttacke),cocos2d::FadeIn::create(0),nullptr),
                                                         cocos2d::Spawn::create(cocos2d::RotateTo::create(0,0),cocos2d::FadeOut::create(0.1),nullptr),
                                                         nullptr));
   }
   else if (direction_attack == DirectionAttacke::LEFT_TO_RIGHT){
      float forMoveBack;
      float forMoveForward;
      weapon_sprite->setRotation(90);
      weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x + weapon_sprite->getBoundingBox().size.width/2 + 40,
                                        weapon_sprite->getPosition().y);
      forMoveBack = -50;
      forMoveForward = 100;
      weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::MoveBy::create(0.3,cocos2d::Vec2(forMoveBack,0)),cocos2d::FadeIn::create(0),nullptr),cocos2d::MoveBy::create(0.1,cocos2d::Vec2(forMoveForward,0)),nullptr),
                                                         cocos2d::Spawn::create(cocos2d::MoveTo::create(0,creature_sprite->getPosition()),cocos2d::FadeOut::create(0.1),nullptr),
                                                         nullptr));
   }
   else if (direction_attack == DirectionAttacke::RIGHT_TO_LEFT){
      float forMoveBack;
      float forMoveForward;
      weapon_sprite->setRotation(270);
      weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x - weapon_sprite->getBoundingBox().size.width/2 - 40,
                                        weapon_sprite->getPosition().y);
      forMoveBack = 50;
      forMoveForward = -100;
      weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::MoveBy::create(0.3,cocos2d::Vec2(forMoveBack,0)),cocos2d::FadeIn::create(0),nullptr),cocos2d::MoveBy::create(0.1,cocos2d::Vec2(forMoveForward,0)),nullptr),
                                                         cocos2d::Spawn::create(cocos2d::MoveTo::create(0,creature_sprite->getPosition()),cocos2d::FadeOut::create(0.1),nullptr),
                                                         nullptr));
   }
}

Spear::Spear(std::string weapon_sprite_path){
   weapon_caracteristics.weapon_cuttinPower = 5;
   weapon_caracteristics.weapon_penetratingPower = 20;
   weapon_caracteristics.weapon_crushingPower =  10;
   weapon_caracteristics.weapon_solidity = 12;
   /*Set up sprite for weapon(how it will looks)*/
   
   weapon_sprite = cocos2d::Sprite::create(weapon_sprite_path);
   weapon_sprite->setColor(cocos2d::Color3B::GREEN);//Rem
   weapon_sprite->setVisible(true);//Rem
   weapon_sprite->setAnchorPoint(weapon_sprite->getPosition().ANCHOR_MIDDLE_BOTTOM);
   weapon_sprite->setScale(0.5,2);//Rem
   /*Set up "hit box" for this weapon(for detect a collision with creatures and others thin*/
   weapon_damage_hitbox = cocos2d::Sprite::create();
   weapon_damage_hitbox->setTexture("textures/player.png");//Chane on hit_box sprite
   weapon_damage_hitbox->setVisible(true);//After all debug shit set false
   weapon_damage_hitbox->setScale(MAX(weapon_sprite->getBoundingBox().size.width/weapon_damage_hitbox->getBoundingBox().size.width,
                                      weapon_sprite->getBoundingBox().size.height/weapon_damage_hitbox->getBoundingBox().size.height));
}
void Spear::attacke(DirectionAttacke direction_attack, cocos2d::Sprite* creature_sprite){
   float angleOfMoveDirection = ControlBall::getAngle();
   float angleOfAttacke;

   /*//After all debug shit remove this lines*/
   weapon_sprite->setVisible(true);
   weapon_sprite->setPosition(creature_sprite->getPosition());
   
   if (direction_attack == DirectionAttacke::TOP_TO_DOWN){
         weapon_sprite->setRotation(0);
         if (angleOfMoveDirection >= 0 && angleOfMoveDirection < 180){
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x + 40,
                                              weapon_sprite->getPosition().y);
            angleOfAttacke = 180;
         }
         else if (angleOfMoveDirection >= 180 && angleOfMoveDirection <= 360){
            weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x - 40,
                                              weapon_sprite->getPosition().y);
            angleOfAttacke = -180;
         }
         weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::RotateTo::create(0.1,angleOfAttacke),cocos2d::FadeIn::create(0),nullptr),
                                                            cocos2d::Spawn::create(cocos2d::RotateTo::create(0,0),cocos2d::FadeOut::create(0.1),nullptr),
                                                            nullptr));
      }
   else if (direction_attack == DirectionAttacke::DOWN_TO_TOP){
      weapon_sprite->setRotation(180);
      if (angleOfMoveDirection >= 0 && angleOfMoveDirection < 180){
         weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x + 40,
                                           weapon_sprite->getPosition().y);
         angleOfAttacke = 0;
      }
      else if (angleOfMoveDirection >= 180 && angleOfMoveDirection <= 360){
         weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x - 40,
                                           weapon_sprite->getPosition().y);
         angleOfAttacke = 360;
      }
      weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::RotateTo::create(0.1,angleOfAttacke),cocos2d::FadeIn::create(0),nullptr),
                                                         cocos2d::Spawn::create(cocos2d::RotateTo::create(0,0),cocos2d::FadeOut::create(0.1),nullptr),
                                                         nullptr));
   }
   else if (direction_attack == DirectionAttacke::LEFT_TO_RIGHT){
      float forMoveBack;
      float forMoveForward;
      weapon_sprite->setRotation(90);
      weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x + weapon_sprite->getBoundingBox().size.width/2 + 40,
                                        weapon_sprite->getPosition().y);
      forMoveBack = -50;
      forMoveForward = 100;
      weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::MoveBy::create(0.3,cocos2d::Vec2(forMoveBack,0)),cocos2d::FadeIn::create(0),nullptr),cocos2d::MoveBy::create(0.1,cocos2d::Vec2(forMoveForward,0)),nullptr),
                                                         cocos2d::Spawn::create(cocos2d::MoveTo::create(0,creature_sprite->getPosition()),cocos2d::FadeOut::create(0.1),nullptr),
                                                         nullptr));
   }
   else if (direction_attack == DirectionAttacke::RIGHT_TO_LEFT){
      float forMoveBack;
      float forMoveForward;
      weapon_sprite->setRotation(270);
      weapon_damage_hitbox->setPosition(weapon_sprite->getPosition().x - weapon_sprite->getBoundingBox().size.width/2 - 40,
                                        weapon_sprite->getPosition().y);
      forMoveBack = 50;
      forMoveForward = -100;
      weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::MoveBy::create(0.3,cocos2d::Vec2(forMoveBack,0)),cocos2d::FadeIn::create(0),nullptr),cocos2d::MoveBy::create(0.1,cocos2d::Vec2(forMoveForward,0)),nullptr),
                                                         cocos2d::Spawn::create(cocos2d::MoveTo::create(0,creature_sprite->getPosition()),cocos2d::FadeOut::create(0.1),nullptr),
                                                         nullptr));
   }
}