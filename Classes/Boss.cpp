#include "Boss.h"
#include "engMacros.hpp"

Boss::Boss(CreatureInfo::Type type,std::string for_who,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id){
   this->currentLayer = gameLayer;
   this->isStatisticsShowing = false;
   this->isNewState = false;
   this->isWeaponSet = false;
   this->indentificator = id;
   this->creature_info.type = type;
   this->creature_info.state = CreatureInfo::State::IN_FALL;
   this->creature_memorySensors = 0;
   creature_currentAttackPattern = 0;
   this->isVision  = false;
   this->sawPlayer = false;
   this->creature_info.animation.animationForWho = for_who;

   if (for_who == "kool-hash"){
      this->creature_info.animation.framesIdleNum   = std::vector<uint>({8,3,5,3,4,9,4,6,5,4,4,4,8,5,4,7});
      creature_part.emplace(PartCreatureType::TOP,(new CreatureInfo::Part(PartCreatureType::TOP,PartCreatureStatus::NORMAL,4)));
      creature_part.emplace(PartCreatureType::MIDDLE,(new CreatureInfo::Part(PartCreatureType::MIDDLE,PartCreatureStatus::NORMAL,6)));
      creature_part.emplace(PartCreatureType::BOTTOM,(new CreatureInfo::Part(PartCreatureType::BOTTOM,PartCreatureStatus::NORMAL,4)));
      creature_attackPattern.push_back(TypeAttacke::TOP_TO_DOWN);
      creature_attackPattern.push_back(TypeAttacke::LEFT_TO_RIGHT);
      creature_attackPattern.push_back(TypeAttacke::DOWN_TO_TOP);
      creature_info.characteristic.velocity_limit  = 180;
      creature_info.characteristic.jump_power = 120;
      creature_info.characteristic.acceleration_power = 55;
      creature_info.characteristic.stamina = 80;
      creature_info.characteristic.stamina_limit = 80;
      creature_info.characteristic.blood   = 20;
      creature_info.characteristic.jump_ability = 1;
      creature_info.characteristic.current_jump_ability_num = 0;
      creature_info.characteristic.mass = 15;
      creature_info.characteristic.vision_radius = 200;
   }
   else if (for_who == "erenu-doo"){
      this->creature_info.animation.framesIdleNum   = std::vector<uint>({9,4,5,4,8,0,4,10,0,4,3,0,5,5,0,10});
      creature_part.emplace(PartCreatureType::TOP,(new CreatureInfo::Part(PartCreatureType::TOP,PartCreatureStatus::NORMAL,10)));
      creature_part.emplace(PartCreatureType::MIDDLE,(new CreatureInfo::Part(PartCreatureType::MIDDLE,PartCreatureStatus::NORMAL,10)));
      creature_part.emplace(PartCreatureType::BOTTOM,(new CreatureInfo::Part(PartCreatureType::BOTTOM,PartCreatureStatus::NORMAL,8)));
      creature_attackPattern.push_back(TypeAttacke::TOP_TO_DOWN);
      creature_attackPattern.push_back(TypeAttacke::LEFT_TO_RIGHT);
      creature_attackPattern.push_back(TypeAttacke::DOWN_TO_TOP);
      creature_info.characteristic.velocity_limit  = 50;
      creature_info.characteristic.jump_power = 0;
      creature_info.characteristic.acceleration_power = 15;
      creature_info.characteristic.stamina = 300;
      creature_info.characteristic.stamina_limit = 300;
      creature_info.characteristic.blood   = 10;
      creature_info.characteristic.jump_ability = 0;
      creature_info.characteristic.current_jump_ability_num = 0;
      creature_info.characteristic.mass = 25;
      creature_info.characteristic.vision_radius = 100;
   }
   else if (for_who == "goo-zoo"){
      this->creature_info.animation.framesIdleNum   = std::vector<uint>({7,5,4,5,4,7,3,7,0,2,4,0,6,5,0,11});
      creature_part.emplace(PartCreatureType::TOP,(new CreatureInfo::Part(PartCreatureType::TOP,PartCreatureStatus::NORMAL,8)));
      creature_part.emplace(PartCreatureType::MIDDLE,(new CreatureInfo::Part(PartCreatureType::MIDDLE,PartCreatureStatus::NORMAL,12)));
      creature_part.emplace(PartCreatureType::BOTTOM,(new CreatureInfo::Part(PartCreatureType::BOTTOM,PartCreatureStatus::NORMAL,6)));
      creature_attackPattern.push_back(TypeAttacke::TOP_TO_DOWN);
      creature_attackPattern.push_back(TypeAttacke::LEFT_TO_RIGHT);
      creature_attackPattern.push_back(TypeAttacke::DOWN_TO_TOP);
      creature_info.characteristic.velocity_limit  = 120;
      creature_info.characteristic.jump_power = 80;
      creature_info.characteristic.acceleration_power = 15;
      creature_info.characteristic.stamina = 200;
      creature_info.characteristic.stamina_limit = 200;
      creature_info.characteristic.blood   = 60;
      creature_info.characteristic.jump_ability = 0;
      creature_info.characteristic.current_jump_ability_num = 0;
      creature_info.characteristic.mass = 50;
      creature_info.characteristic.vision_radius = 300;
   }
   else if (for_who == "avr"){
      this->creature_info.animation.framesIdleNum   = std::vector<uint>({12,3,4,3,7,8,8,6,5,2,4,4,6,5,4,6});
      creature_part.emplace(PartCreatureType::TOP,(new CreatureInfo::Part(PartCreatureType::TOP,PartCreatureStatus::NORMAL,2)));
      creature_part.emplace(PartCreatureType::MIDDLE,(new CreatureInfo::Part(PartCreatureType::MIDDLE,PartCreatureStatus::NORMAL,3)));
      creature_part.emplace(PartCreatureType::BOTTOM,(new CreatureInfo::Part(PartCreatureType::BOTTOM,PartCreatureStatus::NORMAL,2)));
      creature_attackPattern.push_back(TypeAttacke::TOP_TO_DOWN);
      creature_attackPattern.push_back(TypeAttacke::LEFT_TO_RIGHT);
      creature_attackPattern.push_back(TypeAttacke::DOWN_TO_TOP);
      creature_info.characteristic.velocity_limit  = 300;
      creature_info.characteristic.jump_power = 180;
      creature_info.characteristic.acceleration_power = 45;
      creature_info.characteristic.stamina = 500;
      creature_info.characteristic.stamina_limit = 500;
      creature_info.characteristic.blood   = 30;
      creature_info.characteristic.jump_ability = 2;
      creature_info.characteristic.current_jump_ability_num = 0;
      creature_info.characteristic.mass = 10;
      creature_info.characteristic.vision_radius = 300;
   }

   initAnimations();
   initBody(pos);
   creature_sprite->setScale(7);//Remove
}
void Boss::update(float dt){
   showStatistics(DebugStatistics::PHYSICS);

   updateBossFight(dt);

   if (isNewState){
      updateCurrentState();
   }
   updatePermament();
}
void Boss::updateBossFight(float dt){
   
}