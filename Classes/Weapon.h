#pragma once 

#include <cocos2d.h>
#include "gameUI.h"
#include "engEnums.hpp"


class Weapon{
public:   
   /**
    * @param type set up weapon characteristics and sprite and sprite_hitbox
   */
   Weapon();
   virtual ~Weapon();
   /**
    * Define behavior of weapon
    * @param weapon_type which of weapon will be use
    * @param direction_attack which of attack weapon just made
    * @param creature_sprite who make this attacke
   */
   /**
    * What kind of attacke run
    * Depends on direction ...
   */
   virtual void attacke() = 0;
   virtual void interact(void* target_creature) = 0;
   /*Getters*/
   inline cocos2d::Sprite* getSprite(){ return weapon_sprite; };
   inline cocos2d::Sprite* getDammageSprite(){ return weapon_damage_hitbox; };
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
protected:
   cocos2d::Sprite* weapon_sprite;
   cocos2d::Sprite* weapon_damage_hitbox;
   cocos2d::Sprite* weapon_owner;
   WeaponCaracteristics weapon_caracteristics;
};

class Sword : public Weapon{
public:
   Sword(std::string weapon_sprite,cocos2d::Sprite* weapon_owner);
   virtual void attacke() override;
   virtual void interact(void* target_creature) override;
private:
private:
};
class Axe : public Weapon{
public:
   Axe(std::string weapon_sprite,cocos2d::Sprite* weapon_owner);
   virtual void attacke() override;
   virtual void interact(void* target_creature) override;
private:
private:
};
class Spear : public Weapon{
public:
   Spear(std::string weapon_sprite,cocos2d::Sprite* weapon_owner);
   virtual void attacke() override;
   virtual void interact(void* target_creature) override;
private:
private:
};