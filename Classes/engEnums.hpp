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
        float                     vision_radius;//How far creature can see others creatures
    
    };
    enum Type : uint{
        KITTYMITTY,
        KOOL_HASH,
        ERENU_DOO,
        GOO_ZOO,
        AVR,
        BOSS,
        NPC,
    };
    enum InteractedSurface{
        FLOOR,
        ROOF,
        WALL,
        STEPS,
        OTHER_CREATURE,
        OTHER,
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
        CLIMBING,
        DEATH,
        STOP,
        IN_BATTLE,
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
    InteractedSurface surface;
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

enum BehaviorPattern{
    //HERE you can add some patterns for I level AI 
    CHAISING,
    CHANGE_VERTICAL,
    USING_ITEM,
    STOP_BEFORE_SOMETHING,
    WALL_JUMP_TO,
    WALL_JUMP_FROM,
    WAITING_NEW_BEHAVIORPATTERN,
    PATROL,
};
struct BehaviorState{
    BehaviorState(CreatureInfo::State state, CreatureInfo::DMove dmove,float time = 0);
    CreatureInfo::DMove dmove;
    CreatureInfo::State state;
    float time;
};
struct Sensor{
    enum TypeSensor{
        EMPTY                           = 0,
        CUSTOM                          = 666,
        NEAR_BY_SIDE                    = 1,
        NEAR_BY_SIDETOP2X               = 1024,
        NEAR_BY_TOP                     = 4,
        GROUND_UNDER_ME                 = 8,
        GROUND_LEFTWARD_IS              = 256,
        GROUND_RIGHTWARD_IS             = 512,
        SOMETHING_IN_VISION_R           = 16,
        SOMETHING_ON_THE_SAME_LEVEL     = 32,
        SOMETHING_ABOVE                 = 64,
        SOMETHING_BELOW                 = 128,
    };
    Sensor();
    Sensor(TypeSensor type, cocos2d::Vec2 offset);//Make offset
    cocos2d::Vec2 whereTo;
    cocos2d::Vec2 howTo;
    
    TypeSensor type;
};

enum SceneZOrder{
    BACKGROUND,
    MIDLEGROUND,
    FOREGROUND,
    USER_INTERFACE,
};
struct SceneLayer{
    static std::string ui;
    static std::string gamesession;
    static std::string bg;
};

struct LevelCreatures{
    LevelCreatures();
    LevelCreatures(uint,uint,cocos2d::Vec2);
    uint typeCr;
    uint typeWepon;
    std::string typeBehavior;
    std::string name;
    cocos2d::Vec2 point;
};
struct LevelNonePhysicalObj{
    LevelNonePhysicalObj();
    LevelNonePhysicalObj(cocos2d::Rect,std::string,std::string,cocos2d::Vec2,std::string);
    cocos2d::Rect reqt;
    cocos2d::Vec2 offset;     
    std::string   path;
    std::string   backgroundPath;
    std::string   name;
};
struct LevelPhysicalObj{
    LevelPhysicalObj();
    LevelPhysicalObj(std::string frameName,std::string typeAction,cocos2d::Rect rect,std::string nameTarget);
    std::string frameName;
    std::string typeAction;
    uint targetID;
    std::string targetAction;
    cocos2d::Rect rect;
    cocos2d::Sprite* spr;
    cocos2d::Node* target;
    uint id;
    bool isCollided;
};
struct WorldProperties{
    static cocos2d::Size screenSize;
    static cocos2d::Size mapSize;
    static std::multimap<CreatureInfo::Type,LevelCreatures> creatureObj;//Contetn data about players, nps, enemies exported from tiled map ed
    static std::multimap<std::string,LevelPhysicalObj> dynamicObj;//Contetn data about objects with physical body and they are have some animations/actions [door,stair,lever,platform]
    static std::vector<LevelNonePhysicalObj> staticObj;//Contetn data about objects without physical body [death zones, newLevel]
    static std::map<std::string,cocos2d::Action*> actionPool;
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
   uint weapon_range;
};

enum DebugStatistics{
    GAME_STATS,
    PHYSICS,
    LOGIC,
};
