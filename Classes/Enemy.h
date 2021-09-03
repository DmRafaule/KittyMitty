#pragma once

#include "Creature.h"

class Enemy : public Creature{
public:
   Enemy();
   Enemy(CreatureInfo::Type type, cocos2d::Vec2 pos,cocos2d::Node* gameLayer,int id);
   virtual void update(float dt) override;
   virtual void remove() override;
   /* Define init vison pattern and set upd looking obj for Image recognition*/
   virtual void updateVision();
   /* Pack states into queue and then extract them from it*/
   void updateBehavior(float dt);
   /**
    * @return pointer to data of creature_parts of enemy object
   */
   inline std::vector<PartCreature> getPartsOfCreature() {return creature_parts;};
   /**
    * @brief init fields which related to player and cant be assigned in constructor
   */
   void initPlayerDependenceFields();
   void setAI(int typeAI, std::string typeBehaviorPattern);
   inline const cocos2d::Node* getCreatureVisions() { return creature_vision; };
   inline const bool isVisionEnable() { return isVision; };
   inline uint64_t& getMemory() { return creature_memorySensors; };
   inline CreatureInfo::InteractedSurface& getSurface() {return creature_info.surface; };
   inline const Sensor::TypeSensor getActiveSensor() { return creature_currentSensor; };
private:
   /* Set up queue for looking some objects in vision*/
   void setVisionPattern(std::queue<Sensor> pattern);
   /* Create vision obj 'creature vision' for future detecting in updateTouchBegan*/
   void setLookAt(const Sensor& look);
   /* First pack States into Behavior pattern then unpack them in to creature_state*/
   void packBehaviorStates(float dt);
   /* Define which direction creature will be use*/
   void defineDirection();
   /* Define state for making new decision*/
   BehaviorPattern defineBehavior();
   /* Set up new state for creature_state from queue*/
   void unpackBehaviorState(float dt);
   /* Turn on or turn off Battle AI, depends on range of weapon*/
   void defineBattleAI();
protected:
   cocos2d::Node* player;          //Date about player node
   cocos2d::Node* creature_vision; //Represent vision object for interacting with other world obj 
   std::queue<BehaviorState> creature_behaviorStates;  //Pack of expanded states for update them in updateCurrentState one by one
   std::queue<Sensor> creature_visionPattern;          //Pack of where vision object will be and which size it can be
   BehaviorPattern creature_behaviorPattern;           //Pattern represent in witch queue and what kind of state will be in creature_behaviorStates
   Sensor::TypeSensor creature_currentSensor;          //Represent current poping sensors from queue
   uint64_t creature_memorySensors;//Bit field for remember wich sensors are active
   float deltaTime;
   bool isVision;// For optimaing collision updates
   bool sawPlayer;//If it saw creature it will never stop(Untill he die)
};
