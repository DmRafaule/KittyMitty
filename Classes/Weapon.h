#pragma once 

#include <cocos2d.h>

enum WeaponType : uint{
   SWORD,
   SPEER,
   AXE,
};
enum WeaponAttacks : uint{
   TOP_DOWN,
   DOWN_TOP,
   LEFT_RIGHT,
   RIGHT_LEFT,
};

struct WeaponCaracteristics{
   uint weapon_cuttinPower;
   uint weapon_penetratingPower;
   uint weapon_crushingPower;
   uint weapon_solidity;
};

class Weapon{
public:   
   Weapon(WeaponType type);
   virtual ~Weapon();
   void attacke(WeaponType weapon_type, WeaponAttacks weapon_attack, cocos2d::Sprite* creature_sprite);
   /*Getters*/
   inline cocos2d::Sprite* getSprite(){ return weapon_sprite; };
   inline const WeaponCaracteristics getCaracteristics(){ return weapon_caracteristics; };
   /*Setters*/
   void setCaracteristics(uint w_cutP,uint w_penP,uint w_crushP,uint w_sol);
protected:
protected:
   cocos2d::Sprite* weapon_sprite;
   WeaponCaracteristics weapon_caracteristics;
};