#pragma once


struct CreatureInfo{
    struct Animation{
        Animation();
        Animation(std::vector<uint> framesIdleNum,std::string animationForWho);
        std::vector<uint> framesIdleNum;
        std::string animationForWho;
    };
    struct Characteristic{
        uint                      blood;//How many blood liquid in creature
        float                     mass;
        float                     velocity_limit;//How fast player can run
        float                     jump_power;//How hard(heighst) creature can jump
        uint                      jump_ability;//How many jumps creature can make
        uint                      current_jump_ability_num;//How many jumps creature did already
        float                     acceleration_power;//How fast creature will be accelerating to velocity limit
        int                       stamina;//How long creature can fight efficient
        int                       stamina_limit;
        float                     stamina_regeneration_counter;//Timer for regenaration stamina
    
    };
    enum Type : uint{
        KITTYMITTY,
        KOOL_HASH,
        ERENU_DOO,
        GOO_ZOO,
        AVR,
    };
    enum State{
        ATTACK,
        GET_DAMMAGE,
        IDLE,
        START_RUN,
        RUNNING,
        BRACKING,
        STAND_UP,
        SOARING,
        IN_JUMP,
        JUMP_FROM_WALL,
        TAKE_ROOF,
        IN_FALL,
        INTERACTING,
        ON_STEPS,
        MOVE_BY_STEPS,
        ON_WALL,
        LETGO,
        LAND_ON,
    };
    enum DMove{
        OUT   =  3,
        DOWN  = -2,
        LEFT  = -1,
        RIGHT =  1,
        TOP   =  2,
        IN    =  3,
    };
    CreatureInfo();
    CreatureInfo(Type type,Animation animation);
    Type type;
    Animation animation;
    State state;
    Characteristic characteristic;
    DMove dmove;
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
    static std::string bg;
};
struct LevelTransition{
    LevelTransition();
    LevelTransition(cocos2d::Rect,std::string,std::string,cocos2d::Vec2);
    cocos2d::Rect reqt;
    cocos2d::Vec2 offset;     
    std::string   path;
    std::string   backgroundPath;
};
struct WorldProperties{
    static cocos2d::Size screenSize;
    static cocos2d::Size mapSize;
    static cocos2d::Vec2 playerSpawnPoint;
    static std::vector<cocos2d::Vec2> enemySpawnPoint;
    static std::vector<LevelTransition> levelTransitions;
    static std::vector<cocos2d::Rect> levelDeathZone;
    static std::vector<std::pair<std::string,cocos2d::Rect>> levelItems;
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

enum DebugStatistics{
    GAME_STATS,
    PHYSICS,
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