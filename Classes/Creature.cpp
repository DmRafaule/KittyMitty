#include "Creature.h"
#include "GameLayer.h"

///////////////////////////////////////////////////////*Creature class*///////////////////////////////////////////////////////
Creature::Creature(std::string texturePath,CreatureType creature_type,cocos2d::Vec2 pos,void* gameLayer,std::string id){
    isStatisticsShowing = false;
    this->creature_type = creature_type;
    currentlayer = gameLayer;
    switch(creature_type){
        case CreatureType::HUMANOID:{
            creature_parts.push_back(PartCreature(PartCreatureType::HEAD));
            creature_parts.push_back(PartCreature(PartCreatureType::UPPER_TORSE));
            creature_parts.push_back(PartCreature(PartCreatureType::HAND_LEFT));
            creature_parts.push_back(PartCreature(PartCreatureType::HAND_RIGHT));
            creature_parts.push_back(PartCreature(PartCreatureType::BUTTOM_TORSE));
            creature_parts.push_back(PartCreature(PartCreatureType::LEG_LEFT));
            creature_parts.push_back(PartCreature(PartCreatureType::LEG_RIGHT));
            creature_speed_current  = 0;
            creature_speed_max = 90;
            creature_stamina = 100;
            creature_blood   = 20;
            crearure_mass   = 10;
            break;
        }
        case CreatureType::ANIMAL:{
            break;
        }
    }
    creature_sprite = cocos2d::Sprite::createWithSpriteFrameName(texturePath);
    creature_physic_body = cocos2d::PhysicsBody::createEdgeBox(creature_sprite->getBoundingBox().size);
    creature_physic_body->setMass(crearure_mass);
    creature_physic_body->setDynamic(false);
    creature_physic_body->setGravityEnable(true);
    creature_sprite->setPhysicsBody(creature_physic_body);
    creature_sprite->setPosition(pos);
    creature_sprite->setScale(5);
    /*This is prevent of bluring my textures*/
    cocos2d::Texture2D::TexParams tpar = {
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerFilter::NEAREST,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE,
        cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE
    };
    creature_sprite->getTexture()->setTexParameters(tpar);
    static_cast<GameLayer*>(currentlayer)->getChildByName("gamesession")->addChild(creature_sprite,Layer::MIDLEGROUND,id);
}
Creature::~Creature(){
    creature_parts.clear();
}
void Creature::removeSprite(){
    static_cast<GameLayer*>(currentlayer)->getChildByName("gamesession")->removeChildByName(creature_sprite->getName());
}
void Creature::removeStatistics(){
    if (isStatisticsShowing)
        static_cast<GameLayer*>(currentlayer)->removeChild(creature_statistics);
    isStatisticsShowing = false;
}
void Creature::setPart(PartCreatureType part_type, PartCreatureStatus part_status, uint part_densityDef){
    for (auto& part: creature_parts){
        if (part.part_type == part_type){
            part.part_status  = part_status;
            part.part_densityDef = part_densityDef;
        }
    }
}
uint Creature::getPart(PartCreatureType part_type, PartCreatureField part_field){
    for (auto& part : creature_parts){
        if (part.part_type == part_type){
            if (PartCreatureField::STATUS == part_field) return part.part_status;
            else if (PartCreatureField::DENSITY == part_field) return part.part_densityDef;
            else if (PartCreatureField::PENETRATION == part_field) return part.part_penetrationDef;
            else if (PartCreatureField::CRUSHING == part_field) return part.part_crushingDef;

        }
    }
}
PartOrgan& Creature::getOrgan(PartCreatureType part_type,PartOrganType part_organ_type){
    for (auto& part : creature_parts){
        if (part.part_type == part_type){
            for (auto& organ : part.part_organs){
                if (organ.type == part_organ_type)
                    return organ;
            }
        }
    }
}
void Creature::getStatistics(){
    if (!isStatisticsShowing){
        isStatisticsShowing = true;
        creature_statistics = cocos2d::Label::createWithTTF("","fonts/arial.ttf",18,cocos2d::Size::ZERO);
        creature_statistics->setPosition(cocos2d::Vec2(creature_sprite->getPosition().x - creature_statistics->getBoundingBox().size.width/2,
                                                       creature_sprite->getPosition().y + creature_statistics->getBoundingBox().size.height/2));
        static_cast<GameLayer*>(currentlayer)->getChildByName("gamesession")->addChild(creature_statistics,Layer::USER_INTERFACE);
    }
    else{
        isStatisticsShowing = false;
        static_cast<GameLayer*>(currentlayer)->getChildByName("gamesession")->removeChild(creature_statistics);
    }
}

void Creature::setOrgan(PartCreatureType part_type, PartOrganType part_organ_type,PartCreatureStatus status){
    for (auto& part : creature_parts){
        if (part.part_type == part_type){
            for (auto& organ : part.part_organs){
                if (organ.type == part_organ_type)
                    organ.status = status;
            }
        }
    }
}
void Creature::setCreatureSpeed(uint creature_speed_current){
    this->creature_speed_current = creature_speed_current;
}
void Creature::setCreatureBlood(uint creature_blood){
    this->creature_blood = creature_blood;
}
void Creature::setCreatureStamina(uint creature_stamina){
    this->creature_stamina =creature_stamina;
}
void Creature::setStatistics(){
    std::string partStatus;

    /*Set strings about part of body*/
    partStatus.append("Status:\n");
    for (PartCreature &part : creature_parts){
        switch(part.part_type){
            case PartCreatureType::HEAD:{
                partStatus.append("\thead:");
                break;
            }
            case PartCreatureType::UPPER_TORSE:{
                partStatus.append("\tupTorse:");
                break;
            }
            case PartCreatureType::HAND_LEFT:{
                partStatus.append("\thand left:");
                break;
            }
            case PartCreatureType::HAND_RIGHT:{
                partStatus.append("\thand right:");
                break;
            }
            case PartCreatureType::BUTTOM_TORSE:{
                partStatus.append("\tbotTorse:");
                break;
            }
            case PartCreatureType::LEG_LEFT:{
                partStatus.append("\tleg left:");
                break;
            }
            case PartCreatureType::LEG_RIGHT:{
                partStatus.append("\tleg right:");
                break;
            }
        }
        partStatus.append(part.part_status == PartCreatureStatus::NORMAL ? "norm-" :
                          part.part_status == PartCreatureStatus::WONDED ? "wonded-" :
                          part.part_status == PartCreatureStatus::CUTTED ? "cutted-" :
                          "killed");
        partStatus.append(std::to_string(part.part_densityDef) + "-" + std::to_string(part.part_penetrationDef) + "-" + std::to_string(part.part_crushingDef) + "\n");
    
    }
    partStatus.append("brain:");
    partStatus.append(getOrgan(PartCreatureType::HEAD,PartOrganType::BRAIN).status == PartCreatureStatus::NORMAL ? "norm\n" :
                      getOrgan(PartCreatureType::HEAD,PartOrganType::BRAIN).status == PartCreatureStatus::WONDED ? "wonded\n" :
                      getOrgan(PartCreatureType::HEAD,PartOrganType::BRAIN).status == PartCreatureStatus::CUTTED ? "cutted\n" :
                      "killed\n");
    partStatus.append("lungs:");
    partStatus.append(getOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::LUNGS).status == PartCreatureStatus::NORMAL ? "norm\n" :
                      getOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::LUNGS).status == PartCreatureStatus::WONDED ? "wonded\n" :
                      getOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::LUNGS).status == PartCreatureStatus::CUTTED ? "cutted\n" :
                      "killed\n");
    partStatus.append("heart:");
    partStatus.append(getOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::HEART).status == PartCreatureStatus::NORMAL ? "norm\n" :
                      getOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::HEART).status == PartCreatureStatus::WONDED ? "wonded\n" :
                      getOrgan(PartCreatureType::UPPER_TORSE,PartOrganType::HEART).status == PartCreatureStatus::CUTTED ? "cutted\n" :
                      "killed\n");
    partStatus.append("gut:");
    partStatus.append(getOrgan(PartCreatureType::BUTTOM_TORSE,PartOrganType::GUT).status == PartCreatureStatus::NORMAL ? "norm\n" :
                      getOrgan(PartCreatureType::BUTTOM_TORSE,PartOrganType::GUT).status == PartCreatureStatus::WONDED ? "wonded\n" :
                      getOrgan(PartCreatureType::BUTTOM_TORSE,PartOrganType::GUT).status == PartCreatureStatus::CUTTED ? "cutted\n" :
                      "killed\n");
    /*Set strings about body*/
    partStatus.append("blood:" + std::to_string(creature_blood) + "l\n");    
    partStatus.append("stamina:" + std::to_string(creature_stamina) + "\n");
    creature_statistics->setString(partStatus);
}
void Creature::setWeapon(WeaponType wMap ){
    switch (wMap){
    case WeaponType::SWORD:{
        creature_weapon = new Sword("swordStock.png",creature_sprite);
        break;
    }
    case WeaponType::AXE:{
        creature_weapon = new Axe("axeStock.png",creature_sprite);
        break;
    }
    case WeaponType::SPEAR:{
        creature_weapon = new Spear("spearStock.png",creature_sprite);
        break;
    }
    }
    creature_weapon->getSprite()->setPosition(creature_sprite->getPosition());
    creature_weapon->getDammageSprite()->setPosition(creature_weapon->getSprite()->getPosition());
    static_cast<GameLayer*>(currentlayer)->getChildByName("gamesession")->addChild(creature_weapon->getSprite(),Layer::MIDLEGROUND);
    static_cast<GameLayer*>(currentlayer)->getChildByName("gamesession")->addChild(creature_weapon->getDammageSprite(),Layer::MIDLEGROUND);
}

///////////////////////////////////////////////////////*PartCreature class*///////////////////////////////////////////////////////
PartOrgan::PartOrgan(PartOrganType type){
    this->type = type;
    this->status = PartCreatureStatus::NORMAL;
}
Creature::PartCreature::PartCreature(PartCreatureType part_type){
    this->part_status      = PartCreatureStatus::NORMAL;
    this->part_type        = part_type;
    this->part_penetrationDef = 1;
    this->part_crushingDef    = 5;
    switch (part_type){
    case PartCreatureType::HEAD:{
        this->part_densityDef = 10;
        this->part_organs.push_back(PartOrgan(PartOrganType::BRAIN));
        break;
    }
    case PartCreatureType::UPPER_TORSE:{
        this->part_densityDef = 30;
        this->part_organs.push_back(PartOrgan(PartOrganType::LUNGS));
        this->part_organs.push_back(PartOrgan(PartOrganType::HEART));     
        break;
    }
    case PartCreatureType::HAND_LEFT:{
        this->part_densityDef = 10;
        this->part_organs.push_back(PartOrgan(PartOrganType::NONE));
        break;
    }
    case PartCreatureType::HAND_RIGHT:{
        this->part_densityDef = 10;
        this->part_organs.push_back(PartOrgan(PartOrganType::NONE));
        break;
    }
    case PartCreatureType::BUTTOM_TORSE:{
        this->part_densityDef = 20;
        this->part_organs.push_back(PartOrgan(PartOrganType::GUT));
        break;
    }
    case PartCreatureType::LEG_LEFT:{
        this->part_densityDef = 15;
        this->part_organs.push_back(PartOrgan(PartOrganType::NONE));
        break;
    }
    case PartCreatureType::LEG_RIGHT:{
        this->part_densityDef = 15;
        this->part_organs.push_back(PartOrgan(PartOrganType::NONE));
        break;
    }
    }
}

///////////////////////////////////////////////////////*Enemy class*///////////////////////////////////////////////////////
Enemy::Enemy(std::string texturePath,CreatureType bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id) :
    Creature(texturePath,bMap,pos,gameLayer,id){
}
void Enemy::update(float dt){
    /*For statistics*/
    if (isStatisticsShowing){
        setStatistics();
        creature_statistics->runAction(cocos2d::MoveTo::create(0.2,cocos2d::Vec2(creature_sprite->getPosition().x - creature_statistics->getBoundingBox().size.width/2,
                                                                                 creature_sprite->getPosition().y + creature_statistics->getBoundingBox().size.height/2)));
    }
    creature_weapon->getSprite()->runAction(cocos2d::MoveTo::create(0.1f,creature_sprite->getPosition()));
}

///////////////////////////////////////////////////////*Player class*///////////////////////////////////////////////////////
Player::Player(std::string texturePath,CreatureType bMap,cocos2d::Vec2 pos,void* gameLayer,std::string id) :
    Creature(texturePath,bMap,pos,gameLayer,id){
    enemyNode = static_cast<GameLayer*>(gameLayer)->getEnemy();
    currentInteractedEnemy = -1;
    creature_stamina_regeneration_counter = 0;
}
void Player::update(float dt){
    /*For statistics*/
    if (isStatisticsShowing){
        setStatistics();
        creature_statistics->runAction(cocos2d::MoveTo::create(0.2,cocos2d::Vec2(creature_sprite->getPosition().x - creature_statistics->getBoundingBox().size.width/2,
                                                                                 creature_sprite->getPosition().y + creature_statistics->getBoundingBox().size.height/2)));
    }
    //For moves of all body
    if (ControlKeys::getMoving()){
        //Controle the speed of player
        //Speed limit for creature
        if (creature_speed_current < creature_speed_max)
            creature_speed_current+=3;
        //Breathtaken limit
        if (creature_speed_current >= 70)
            creature_stamina--;
        creature_sprite->runAction(cocos2d::EaseQuadraticActionInOut::create(cocos2d::MoveBy::create(1.f,ControlKeys::getDirection())));
        //Weapon follow by body
        creature_weapon->getSprite()->runAction(cocos2d::EaseQuadraticActionInOut::create(cocos2d::MoveBy::create(1.f,ControlKeys::getDirection())));
    }
    else 
        creature_speed_current = 30;
    //For attacke
    if (ControlAttc::getAttacke() && creature_stamina >= 20){
        //Set to default state
        ControlAttc::setAttacke(false);
        creature_weapon->attacke();
        creature_weapon->takeEffect(this);
        for (int i=0; i < enemyNode->size(); ++i)
            if (enemyNode->at(i)->getCreatureSprite()->getBoundingBox().intersectsRect(creature_weapon->getDammageSprite()->getBoundingBox()))
                currentInteractedEnemy = i;
        /*If we hit the enemy*/
        if (currentInteractedEnemy >= 0){
            creature_weapon->interact(enemyNode->at(currentInteractedEnemy));
        }
            
        currentInteractedEnemy = -1;
    }
    //For regeneration of stamina
    creature_stamina_regeneration_counter += dt;
    if (creature_stamina < 100 && creature_stamina_regeneration_counter >= 1){
        creature_stamina++;
        creature_stamina_regeneration_counter = 0;
    }
    
}

/**Some code for future
 **First clean engine's calls
 *enemyNode->at(currentInteractedEnemy)->removeSprite();
 *enemyNode->at(currentInteractedEnemy)->removeStatistics();
 **Second clean game's  call
 *enemyNode->erase(enemyNode->begin()+currentInteractedEnemy);
*/