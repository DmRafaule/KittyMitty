#include "NPC.h"
#include "engMacros.hpp"

NPC::NPC(CreatureInfo::Type type,std::string for_who,cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id){
   this->currentLayer = gameLayer;
   this->isStatisticsShowing = false;
   this->isNewState = false;
   this->isWeaponSet = false;
   this->indentificator = id;
   this->creature_info.type = type;
   this->creature_info.state = CreatureInfo::State::IN_FALL;
   this->creature_memorySensors = 0;
   this->isVision  = false;
   this->sawPlayer = false;
   this->creature_info.animation.animationForWho = for_who;
   // Here add some uniqe creatures
   if (for_who == "kool-hash"){
      this->creature_info.animation.framesIdleNum   = std::vector<uint>({8,3,5,3,4,9,4,6,5,4,4,4,8,5,4,7});
      creature_part.emplace(PartCreatureType::TOP,(new CreatureInfo::Part(PartCreatureType::TOP,PartCreatureStatus::NORMAL,9999)));
      creature_part.emplace(PartCreatureType::MIDDLE,(new CreatureInfo::Part(PartCreatureType::MIDDLE,PartCreatureStatus::NORMAL,9999)));
      creature_part.emplace(PartCreatureType::BOTTOM,(new CreatureInfo::Part(PartCreatureType::BOTTOM,PartCreatureStatus::NORMAL,9999)));
      creature_info.characteristic.velocity_limit  = 180;
      creature_info.characteristic.jump_power = 120;
      creature_info.characteristic.acceleration_power = 55;
      creature_info.characteristic.stamina = 80;
      creature_info.characteristic.stamina_limit = 80;
      creature_info.characteristic.blood   = 20;
      creature_info.characteristic.jump_ability = 1;
      creature_info.characteristic.current_jump_ability_num = 0;
      creature_info.characteristic.mass = 15;
      creature_info.characteristic.vision_radius = 50;
   }

   initAnimations();
   initBody(pos);
}
void NPC::update(float dt){

   if (isNewState){
      updateCurrentState();
   }
   updatePermament();
}
void NPC::updateVision(){
   if (getDistanceTo(player->getPosition()) < creature_info.characteristic.vision_radius && Creature::isInInteraction){
      sawPlayer = true;
   }
   else{
      sawPlayer = false;
      Creature::isInInteraction = false;
   } 
}
void NPC::updateBehavior(float dt){
   if (sawPlayer){
      Creature::isInInteraction = false;
      //Some dialogs and trade logic when developing GUI for game
   }
}