#pragma once

enum CreatureType : uint{
    HUMANOID,
    INSECT,
    ANIMAL,
    TREE,
    FISH,
};

enum PartCreatureType : uint{
    HEAD,
    UPPER_TORSE,
    BUTTOM_TORSE,
    HAND_LEFT,
    HAND_RIGHT,
    LEG_LEFT,
    LEG_RIGHT,
    SIZE,
};
enum PartCreatureStatus : uint{
    NORMAL,
    CUTTED,
    WONDED,
    KILLED,
};
enum PartCreatureField : uint{
    STATUS,
    DENSITY,
    PENETRATION,
    CRUSHING,
};

enum PartOrganType : uint{
    NONE,
    BRAIN,
    LUNGS,
    HEART,
    GUT,
};
struct PartOrgan {
    PartOrgan(PartOrganType type);
    PartOrganType type;
    PartCreatureStatus status;
};

enum ZLevel{
    BACKGROUND,
    MIDLEGROUND,
    FOREGROUND,
    USER_INTERFACE,
};
struct SceneEntities{
    static std::vector<std::string> enemy;
    static std::string player;
    static std::string ball;
    static std::string ball_attacke;
    static std::string text;
    static std::string player_weapon;
    static std::string ui;
    static std::string gamesession;
};

enum TypeUI{
    CONTROL_IN_GAMESESSION,
    CONTROL_KEYS,
};
enum DirectionAttacke{
    TOP_TO_DOWN,
    DOWN_TO_TOP,
    LEFT_TO_RIGHT,
    RIGHT_TO_LEFT,
    TOPLEFT_TO_BOTTOMRIGHT,
    TOPRIGHT_TO_BOTTOMLEFT,
    BOTTOMLEFT_TO_TOPRIGHT,
    BOTTOMRIGHT_TO_TOPLEFT,
};
enum DirectionMove{
    LEFT,
    RIGHT,
    TOP,
    DOWN,
    IN,
    OUT,
};


enum WeaponType : uint{
   SWORD,
   SPEAR,
   AXE,
};
struct WeaponCaracteristics{
   uint weapon_cuttinPower;
   uint weapon_penetratingPower;
   uint weapon_crushingPower;
   uint weapon_solidity;
   uint weapon_mass;
};