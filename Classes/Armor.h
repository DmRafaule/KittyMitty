#pragma once

#include <cocos2d.h>

#include "engEnums.hpp"

/**
 * @brief Parrent class for all armors
*/
class Creature;

class Armor{
public:
      /**
    * @param type set up weapon characteristics and sprite
   */
   Armor(std::string armor_sprite_path,Creature* armor_owner);
   virtual ~Armor();
   /**
    * @brief What kind of updateTakeDammageAnimation run(just for animation and properly movements for each attack)
    * Depends on direction attack ..
   */
   virtual void updateTakeDammageAnimation();
   virtual void update();
   inline cocos2d::Sprite* getSprite(){ return armor_sprite; };
   inline const ArmorCaracteristics getCaracteristics(){ return armor_caracteristics; };
protected:
   cocos2d::Sprite*        armor_sprite;              //What player can see
   cocos2d::PhysicsBody*   armor_physic_body;
   CreatureInfo::DMove*    armor_owner_dirmove;       //This var was implemented because using weapon_owner will overcomplicated code
   Creature*               armor_owner;               //Who hold this weapon
   ArmorCaracteristics     armor_caracteristics;  //Weapon characteristics, look at structure
};