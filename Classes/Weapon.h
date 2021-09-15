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
   Weapon(std::string weapon_sprite_path,Creature* weapon_owner);
   virtual ~Weapon();
   /**
    * @brief What kind of updateAttackAnimation run(just for animation and properly movements for each attack)
    * Depends on direction attack ..
   */
   virtual void updateAttackAnimation();
   virtual void update();
   /**
    * @brief How some kind of attack will make some change on target
   */
   virtual void giveEffect(Creature* target_creature);
   /**
    * @brief How some kind of attack will make some change on owner
   */
   virtual void takeEffect();
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
private:
   void setIntergralityTo(Creature* target, const PartCreatureType& type);
   PartCreatureType defineTargetingPart();
protected:
   cocos2d::Sprite*        weapon_sprite;              //What player can see
   cocos2d::PhysicsBody*   weapon_physic_body;
   CreatureInfo::DMove*    weapon_owner_dirmove;       //This var was implemented because using weapon_owner will overcomplicated code
   std::string             target_part;
   std::string             target_status;
   Creature*               weapon_owner;               //Who hold this weapon
   WeaponCaracteristics    weapon_caracteristics;  //Weapon characteristics, look at structure
   bool                    isAttack;      //This one was implemneted because I need attack once on detecting event by AI 
};

/**
 * @brief Implement stock sword
*/
class Sword : public Weapon{
public:
   Sword(std::string weapon_sprite, Creature* owner_obj);
};
/**
 * @brief Implement stock axe
*/
class Axe : public Weapon{
public:
   Axe(std::string weapon_sprite, Creature* owner_obj);
};
/**
 * @brief Implement stock spear
*/
class Spear : public Weapon{
public:
   Spear(std::string weapon_sprite, Creature* owner_obj);
};