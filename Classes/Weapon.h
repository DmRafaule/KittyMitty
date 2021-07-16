#pragma once 

#include <cocos2d.h>
#include "gameUI.h"
#include "engEnums.hpp"

/**
 * @brief Parrent class for all weapons
*/
class Weapon{
public:   
   /**
    * @param type set up weapon characteristics and sprite and sprite_hitbox
   */
   Weapon();
   virtual ~Weapon();
   /**
    * @brief What kind of attacke run(just for animation and properly movements for each attack)
    * Depends on direction attack ..
   */
   virtual void attacke() = 0;
   /**
    * @brief How some kind of attack will make some change on target
   */
   virtual void interact(void* target_creature) = 0;
   /*Getters*/
   /**
    * @return weapon sprite
   */
   inline cocos2d::Sprite* getSprite(){ return weapon_sprite; };
   /**
    * @return damage sprite(invisible)
   */
   inline cocos2d::Sprite* getDammageSprite(){ return weapon_damage_hitbox; };
   /**
    * @return struct of characteristics
   */
   inline const WeaponCaracteristics getCaracteristics(){ return weapon_caracteristics; };
   /*Setters*/
   /**
    * Set characteristics for weapon
    * @param w_cutP cutted power
    * @param w_penP penetrating power
    * @param w_crushP crushing power
    * @param w_sol   solidity of weapon
   */
   void setCaracteristics(uint w_cutP,uint w_penP,uint w_crushP,uint w_sol);
protected:
   cocos2d::Sprite* weapon_sprite;              //What player can see
   cocos2d::Sprite* weapon_damage_hitbox;       //What detect collision between weapon and enemies
   cocos2d::Sprite* weapon_owner;               //Who have this weapon(used methos setWeapon)
   WeaponCaracteristics weapon_caracteristics;  //Weapon characteristics, look at structure
};

/**
 * @brief Implement stock sword
*/
class Sword : public Weapon{
public:
   Sword(std::string weapon_sprite,cocos2d::Sprite* weapon_owner);
   virtual void attacke() override;
   virtual void interact(void* target_creature) override;
};
/**
 * @brief Implement stock axe
*/
class Axe : public Weapon{
public:
   Axe(std::string weapon_sprite,cocos2d::Sprite* weapon_owner);
   virtual void attacke() override;
   virtual void interact(void* target_creature) override;
};
/**
 * @brief Implement stock spear
*/
class Spear : public Weapon{
public:
   Spear(std::string weapon_sprite,cocos2d::Sprite* weapon_owner);
   virtual void attacke() override;
   virtual void interact(void* target_creature) override;
};