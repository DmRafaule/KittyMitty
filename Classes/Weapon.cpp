#include "Weapon.h"

Weapon::Weapon(WeaponType type){
   std::string weapon_texture_path;
   switch (type){
   case WeaponType::SWORD:{
      weapon_texture_path = "textures/player.png";
      weapon_caracteristics.weapon_cuttinPower = 10;
      weapon_caracteristics.weapon_penetratingPower = 5;
      weapon_caracteristics.weapon_crushingPower =  1;
      weapon_caracteristics.weapon_solidity = 30;
      break;
   }
   case WeaponType::SPEER:{
      weapon_texture_path = "textures/player.png";
      weapon_caracteristics.weapon_cuttinPower = 1;
      weapon_caracteristics.weapon_penetratingPower = 15;
      weapon_caracteristics.weapon_crushingPower =  5;
      weapon_caracteristics.weapon_solidity = 20;
      break;
   }
   case WeaponType::AXE:{
      weapon_texture_path = "textures/player.png";
      weapon_caracteristics.weapon_cuttinPower = 8;
      weapon_caracteristics.weapon_penetratingPower = 10;
      weapon_caracteristics.weapon_crushingPower =  10;
      weapon_caracteristics.weapon_solidity = 10;
      break;
   }
   }
   weapon_sprite = cocos2d::Sprite::create(weapon_texture_path);
   weapon_sprite->setColor(cocos2d::Color3B::GREEN);
   weapon_sprite->setVisible(false);
   weapon_sprite->setAnchorPoint(weapon_sprite->getPosition().ANCHOR_MIDDLE_BOTTOM);
   weapon_sprite->setScale(0.5,2);
}
Weapon::~Weapon(){
   
}
void Weapon::attacke(WeaponType weapon_type, WeaponAttacks weapon_attack, cocos2d::Sprite* creature_sprite){
   weapon_sprite->setVisible(true);
   weapon_sprite->setPosition(creature_sprite->getPosition());//HERE upgrade actions
   weapon_sprite->runAction(cocos2d::Sequence::create(cocos2d::RotateTo::create(0.1,-180),cocos2d::RotateTo::create(0,0),nullptr));
}
void Weapon::setCaracteristics(uint w_cutP,uint w_penP,uint w_crushP,uint w_sol){
   this->weapon_caracteristics.weapon_crushingPower = w_crushP;
   this->weapon_caracteristics.weapon_cuttinPower = w_cutP;
   this->weapon_caracteristics.weapon_penetratingPower = w_penP;
   this->weapon_caracteristics.weapon_solidity = w_sol;
}