#pragma once
#include<glm/glm.hpp>
using namespace std;
#include<vector>
#include "animation.h"
#include<SDL3/SDL.h>

enum class PlayerState
{
    idle, running, jumping
};

enum class BulletState
{
    moving, colliding, inactive
};

enum class EnemyState
{
    shambling, damage, dead
};

struct PlayerData
{
    PlayerState state;
    Timer weaponTimer;
    PlayerData() : weaponTimer(0.1f)
    {
        state = PlayerState::idle;
    }
};

struct LevelData {};
struct EnemyData 
{
    EnemyState state;
    Timer damageTimer;
    int healthPoint;

    EnemyData(): state(EnemyState::shambling), damageTimer(0.5f)
    {
        healthPoint = 100;
    }
};
struct BulletData 
{
    BulletState state;
    BulletData() : state(BulletState::moving)
    {
    }
};

union ObjectData
{
    PlayerData player;
    LevelData level;
    EnemyData enemy;
    BulletData bullet;
};

enum class ObjectType
{
    player, level, enemy, bullet
};

struct GameObject
{
    ObjectType type;
    ObjectData data;
    glm::vec2 position, velocity, acceleration;
    float direction;
    float maxSpeedX;
    vector<Animation> animations;
    int currentAnimation;
    SDL_Texture* texture;
    bool dynamic;
    SDL_FRect collider;
    bool grounded;
    Timer flashTimer;
    bool shouldFlash;
    int spriteFrame;

    GameObject(): data{.level = LevelData()}, collider{0}, flashTimer{0.05f}
    {
        type = ObjectType::level;
        position = velocity = acceleration = glm::vec2(0);
        maxSpeedX = 0;
        currentAnimation = -1;
        texture = nullptr;
        dynamic = false;
        grounded = false;
        shouldFlash = false;
        spriteFrame = 1;
    }
};
