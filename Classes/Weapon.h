#pragma once 

#include <cocos2d.h>

#include "engEnums.hpp"

/**
 * @brief Parrent class for all weapons
*/
class Creature;
class Weapon{
public:   
   /**
    * @param type set up weapon characteristics and sprite
   */
   Weapon(std::string weapon_sprite_path,cocos2d::Sprite* weapon_owner_sprite);
   virtual ~Weapon();
   /**
    * @brief What kind of attacke run(just for animation and properly movements for each attack)
    * Depends on direction attack ..
   */
   virtual void attacke();
   virtual void update();
   /**
    * @brief How some kind of attack will make some change on target
   */
   virtual void giveEffect(Creature* target_creature);
   /**
    * @brief How some kind of attack will make some change on owner
   */
   virtual void takeEffect(Creature* owner);
   /*Getters*/
   /**
    * @return weapon sprite
   */
   inline cocos2d::Sprite* getSprite(){ return weapon_sprite; };
   /**
    * @return struct of characteristics
   */
   inline const WeaponCaracteristics getCaracteristics(){ return weapon_caracteristics; };
   inline bool& getAttackStatus(){ return isAttack; };
   /*Setters*/
   /**
    * Set characteristics for weapon
    * @param w_cutP cutted power
    * @param w_penP penetrating power
    * @param w_crushP crushing power
    * @param w_sol   solidity of weapon
   */
   void setCaracteristics(uint w_cutP,uint w_penP,uint w_crushP,uint w_sol,uint w_mass);
protected:
   cocos2d::Sprite* weapon_sprite;              //What player can see
   cocos2d::Sprite* weapon_owner_sprite;        //Who have this weapon(used methos setWeapon)
   CreatureInfo::DMove*    weapon_owner_dirmove;
   cocos2d::PhysicsBody* weapon_physic_body;
   WeaponCaracteristics weapon_caracteristics;  //Weapon characteristics, look at structure
   bool isAttack;
};

/**
 * @brief Implement stock sword
*/
class Sword : public Weapon{
public:
   Sword(std::string weapon_sprite,cocos2d::Sprite* weapon_owner_sprite, void* owner_obj);
};
/**
 * @brief Implement stock axe
*/
class Axe : public Weapon{
public:
   Axe(std::string weapon_sprite,cocos2d::Sprite* weapon_owner_sprite, void* owner_obj);
};
/**
 * @brief Implement stock spear
*/
class Spear : public Weapon{
public:
   Spear(std::string weapon_sprite,cocos2d::Sprite* weapon_owner_sprite, void* owner_obj);
};