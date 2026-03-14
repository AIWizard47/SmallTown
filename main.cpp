// Stander library
#include<iostream>
#include<vector>
#include<array>
#include<string>
using namespace std;

// Including SDL library
#include<SDL3/SDL.h>
#include<SDL3/SDL_main.h>
#include<SDL3_mixer/SDL_mixer.h>
#include<SDL3_image/SDL_image.h>
#include<glm/glm.hpp>

// custom packages
#include "pkg/gameObject.h"


//Struct
struct SDLState
{
    //define datatypes
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width,hight,logW,logH;
    const bool *keys;
    bool fullscreen;

    SDLState():keys(SDL_GetKeyboardState(nullptr))
    {
        fullscreen = false;   
    }
};
struct Resources
{
    const int ANIM_PLAYER_IDLE = 0;
    const int ANIM_PLAYER_RUN = 1;
    const int ANIM_PLAYER_SLIDE = 2;
    const int ANIM_PLAYER_SHOOT = 3;
    const int ANIM_PLAYER_SLIDE_SHOOT = 4;
    vector<Animation> playerAnims;
    const int ANIM_BULLET_MOVING = 0;
    const int ANIM_BULLET_HIT = 1;
    vector<Animation> bulletAnims;
    const int ANIM_ENEMY = 0;
    const int ANIM_ENEMY_HIT = 1;
    const int ANIM_ENEMY_DIE = 2;
    vector<Animation> enemyAnims;

    vector<SDL_Texture*> textures;
    SDL_Texture *texIdle, *texRun, *texBrick, *texGrass, *texGround, *texPanel, 
    *texSlide, *texBg1, *texBg2, *texBg3, *texBg4, *texBullet, *texBulletHit,
    *texShoot, *texRunShoot, *texSlideShoot, *texEnemy, *texEnemyHit, *texEnemyDie;

    vector<MIX_Audio*> sounds;
    MIX_Audio *bgMusic;
    MIX_Audio *shootSound;
    MIX_Audio *enemyHitSound;
    MIX_Audio *enemyDieSound;
    MIX_Audio *wallHitSound;

    MIX_Mixer* mixer;
    MIX_Track* bgTrack;
    MIX_Track* shootTrack;
    MIX_Track* hitTrack;
    MIX_Track* dieTrack;
    MIX_Track* wallTrack;
    
    SDL_Texture *loadTexture(SDL_Renderer* renderer, const string &filepath)
    {
        SDL_Texture* tex = IMG_LoadTexture(renderer,filepath.c_str());
        SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
        textures.push_back(tex);
        return tex;
    }
    
    void load(SDLState &state)
    {
        if (!MIX_Init())
        {
            cout << "MIX_Init failed: " << SDL_GetError() << endl;
        }
        mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
        if(!mixer)
        {
            cout << "Mixer creation failed: " << SDL_GetError() << endl;
        }

        playerAnims.resize(5); //change the size of vector {0,0,0,0,0}
        playerAnims[ANIM_PLAYER_IDLE] = Animation(8, 1.6f);  //{ANIM_PLAYER_IDLE,0,0,0,0}  
        playerAnims[ANIM_PLAYER_RUN] = Animation(4,0.5f); //{ANIM_PLAYER_IDLE,ANIM_PLAYER_RUN,0,0,0} 
        playerAnims[ANIM_PLAYER_SLIDE] = Animation(1,0.1f); //{ANIM_PLAYER_IDLE,ANIM_PLAYER_RUN,ANIM_PLAYER_SLIDE,0,0} 
        playerAnims[ANIM_PLAYER_SHOOT] = Animation(4,0.5f);
        playerAnims[ANIM_PLAYER_SLIDE_SHOOT] = Animation(4,0.5f);
        bulletAnims.resize(2);
        bulletAnims[ANIM_BULLET_MOVING] = Animation(4,0.05f);
        bulletAnims[ANIM_BULLET_HIT] = Animation(4,0.15f);
        enemyAnims.resize(3);
        enemyAnims[ANIM_ENEMY] = Animation(8,1.0f);
        enemyAnims[ANIM_ENEMY_HIT] = Animation(8,1.0f);
        enemyAnims[ANIM_ENEMY_DIE] = Animation(18,2.0f);
        
        texIdle = loadTexture(state.renderer,"data/idle.png");
        texRun = loadTexture(state.renderer,"data/run.png");
        texBrick = loadTexture(state.renderer,"data/tiles/brick.png");
        texSlide = loadTexture(state.renderer,"data/slide.png");
        texGrass = loadTexture(state.renderer,"data/tiles/grass.png");
        texGround = loadTexture(state.renderer,"data/tiles/ground.png");
        texPanel = loadTexture(state.renderer,"data/tiles/panel.png");
        texBg1 = loadTexture(state.renderer,"data/bg/bg_layer1.png");
        texBg2 = loadTexture(state.renderer,"data/bg/bg_layer2.png");
        texBg3 = loadTexture(state.renderer,"data/bg/bg_layer3.png");
        texBg4 = loadTexture(state.renderer,"data/bg/bg_layer4.png");
        texBullet = loadTexture(state.renderer,"data/bullet.png");
        texBulletHit = loadTexture(state.renderer,"data/bullet_hit.png");
        texShoot = loadTexture(state.renderer,"data/shoot.png");
        texRunShoot = loadTexture(state.renderer,"data/shoot_run.png");
        texSlideShoot = loadTexture(state.renderer,"data/slide_shoot.png");
        texEnemy = loadTexture(state.renderer,"data/enemy.png");
        texEnemyHit = loadTexture(state.renderer,"data/enemy_hit.png");
        texEnemyDie = loadTexture(state.renderer,"data/enemy_die.png");

        bgMusic = MIX_LoadAudio(mixer, "data/audio/bg.wav", true);
        bgTrack = MIX_CreateTrack(mixer);
        shootSound = MIX_LoadAudio(mixer, "data/audio/shoot.wav", true);
        shootTrack = MIX_CreateTrack(mixer);
        enemyHitSound = MIX_LoadAudio(mixer, "data/audio/enemy_hit.wav", true);
        hitTrack = MIX_CreateTrack(mixer);
        enemyDieSound = MIX_LoadAudio(mixer, "data/audio/monster_die.wav", true);
        dieTrack = MIX_CreateTrack(mixer);
        wallHitSound = MIX_LoadAudio(mixer, "data/audio/wall_hit.wav", true);
        wallTrack = MIX_CreateTrack(mixer);

        sounds.push_back(bgMusic);
        sounds.push_back(shootSound);
        sounds.push_back(enemyHitSound);
        sounds.push_back(enemyDieSound);
        sounds.push_back(wallHitSound);
    }

    void unload(){
        for(SDL_Texture *tex:textures)
        {
            SDL_DestroyTexture(tex);
        }
        for(auto sound : sounds)
        {
            MIX_DestroyAudio(sound);
        }
    }
};


const size_t LAYER_IDX_LEVEL = 0;
const size_t LAYER_IDX_CHARACTERS = 1;
const int MAP_ROWS = 5;
const int MAP_COLS = 50;
const int TILE_SIZE = 32;

struct GameState
{
    array<vector<GameObject>, 2> layers;
    vector<GameObject> backgroundTiles;
    vector<GameObject> foreGroundTiles;
    vector<GameObject> bullets;
    int playerIndex;
    SDL_FRect mapViewPort;
    float bg2Scroll, bg3Scroll, bg4Scroll;
    bool debugMode;

    GameState(const SDLState &state)
    {
        playerIndex = -1; // When we load other object then change it.
        mapViewPort = SDL_FRect
        {
            .x = 0,
            .y = 0,
            .w = static_cast<float>(state.logW),
            .h = static_cast<float>(state.logH)
        };
        bg2Scroll = bg3Scroll = bg4Scroll = 0;
        debugMode = false;
    }

    GameObject &player() {return layers[LAYER_IDX_CHARACTERS][playerIndex];}
};


// Declaring
void cleanUp(SDLState &state);
bool initialize(SDLState &state);
void drawObject(const SDLState &state, GameState &gs, GameObject &obj, float width, float height, float deltaTime);
void update(const SDLState &state, GameState &gs,Resources &res, GameObject &obj, float deltaTime);
void createTiles(const SDLState &state, GameState &gs, const Resources &res);
void checkCollision(const SDLState &state, GameState &gs, Resources &res, GameObject &a, GameObject &b, float deltaTime);
void handleKeyInput(const SDLState &state, GameState &gs, GameObject &obj, SDL_Scancode key, bool keyDown);
void drawParallaxBackground(SDL_Renderer *renderer, SDL_Texture *texture, float xVelocity, float &scrollPos, float scrollFactor, float deltaTime);


int main(int argc, char* argv[])
{   
    SDLState state;
    state.width = 1600;
    state.hight = 900;
    state.logW = 640;
    state.logH = 320;
    if(!initialize(state))
    {
        return 1;
    }
    //load game assets
    Resources res;
    res.load(state);

    //setup game data
    GameState gs(state);
    createTiles(state,gs,res);

    uint64_t prevTime = SDL_GetTicks();
    MIX_SetTrackAudio(res.bgTrack, res.bgMusic);
    cout<<"playing BG audio"<<endl;
    MIX_SetTrackGain(res.bgTrack, 0.3f);  
    MIX_PlayTrack(res.bgTrack, 1);
    // const float spriteSize = 32;
    // Start the game loop
    bool running = true;
    while(running)
    {
        uint64_t nowTime = SDL_GetTicks();
        float deltaTime = (nowTime - prevTime) / 1000.0f;
        SDL_Event event{0};  //event = 0
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_EVENT_QUIT:
                {
                    running = false;
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED:
                {
                    state.width = event.window.data1;
                    state.hight = event.window.data2;
                    SDL_SetRenderLogicalPresentation(
                        state.renderer,
                        state.logW,
                        state.logH,
                        SDL_LOGICAL_PRESENTATION_LETTERBOX
                    );
                    gs.mapViewPort.w = state.logW;
                    gs.mapViewPort.h = state.logH;
                    // cout<<width<<" "<<hight<<endl;
                    break;
                }
                // This is best for press type key like walking, running and firing.
                case SDL_EVENT_KEY_DOWN:
                {   
                    handleKeyInput(state, gs,gs.player(),event.key.scancode,true);
                    break;
                }
                // This is best for one time click.
                case SDL_EVENT_KEY_UP:
                {
                    handleKeyInput(state, gs,gs.player(),event.key.scancode,false);
                    if (event.key.scancode == SDL_SCANCODE_F12)
                    {
                        gs.debugMode = !gs.debugMode;
                    }
                    else if(event.key.scancode == SDL_SCANCODE_F11)
                    {
                        state.fullscreen = !state.fullscreen;
                        SDL_SetWindowFullscreen(state.window, state.fullscreen);
                    }
                    break;
                }
            }
        }

        //update all object
        for(auto &layer: gs.layers)
        {
            for(GameObject &obj:layer)
            {
                update(state,gs,res,obj,deltaTime);
            }
        }
        

        // update bullets
        for(GameObject &bullet: gs.bullets)
        {
            update(state,gs,res,bullet,deltaTime);
        }

        //calculate viewport postion
        gs.mapViewPort.x = (gs.player().position.x + TILE_SIZE / 2) - gs.mapViewPort.w / 2;

        // perform drawing command
        SDL_SetRenderDrawColor(state.renderer,20,10,30,255);
        SDL_RenderClear(state.renderer);

        
        //draw a background
        SDL_RenderTexture(state.renderer,res.texBg1,nullptr,nullptr);
        
        if(gs.debugMode)
        {
            SDL_SetRenderDrawColor(state.renderer,255,255,255,255);
            SDL_RenderDebugText(state.renderer, 5,5,to_string(static_cast<int>(gs.player().data.player.state)).c_str());
    
            SDL_SetRenderDrawColor(state.renderer,255,255,255,255);
            SDL_RenderDebugText(state.renderer, 50,5,"[A][D] [Space]");
            //display some debug info
        }

        drawParallaxBackground(state.renderer, res.texBg4, gs.player().velocity.x, gs.bg2Scroll, 0.075f, deltaTime);
        drawParallaxBackground(state.renderer, res.texBg3, gs.player().velocity.x, gs.bg3Scroll, 0.150f, deltaTime);
        drawParallaxBackground(state.renderer, res.texBg2, gs.player().velocity.x, gs.bg4Scroll, 0.3f, deltaTime);

        //draw background tiles.
        for(GameObject &obj : gs.backgroundTiles)
        {
            SDL_FRect dst
            {
                .x = obj.position.x - gs.mapViewPort.x,
                .y = obj.position.y,
                .w = static_cast<float>(obj.texture->w),
                .h = static_cast<float>(obj.texture->h)
            };
            SDL_RenderTexture(state.renderer,obj.texture,nullptr,&dst);
        }

        //draw all object
        for(auto &layer: gs.layers)
        {
            for(GameObject &obj:layer)
            {
                drawObject(state,gs,obj,static_cast<float>(TILE_SIZE),static_cast<float>(TILE_SIZE),deltaTime);
            }
        }
        // draw bullets
        for(GameObject &bullet: gs.bullets)
        {
            if (bullet.data.bullet.state != BulletState::inactive)
            {
                drawObject(state, gs, bullet, bullet.collider.w, bullet.collider.h,deltaTime);
            }
        }

        
        // draw a foreground tile.
        for(GameObject &obj : gs.foreGroundTiles)
        {
            SDL_FRect dst
            {
                .x = obj.position.x - gs.mapViewPort.x,
                .y = obj.position.y,
                .w = static_cast<float>(obj.texture->w),
                .h = static_cast<float>(obj.texture->h)
            };
            SDL_RenderTexture(state.renderer,obj.texture,nullptr,&dst);
        }
        //swap buffer and present
        SDL_RenderPresent(state.renderer);
        prevTime = nowTime;
    }
    
    res.unload();
    cleanUp(state);
    
    return 0;
}

//initialize
bool initialize(SDLState &state)
{   
    bool initSuccess = true;
    // Initialise SDL3
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","Error to initialise the SDL3",nullptr);
        initSuccess = false;
    }

    // Create a window
    state.window = SDL_CreateWindow("Macro",state.width,state.hight,SDL_WINDOW_RESIZABLE);

    if(!state.window)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","Error to creating a window",state.window);
        cleanUp(state);
        initSuccess = false;
    }
    // create renderer
    state.renderer = SDL_CreateRenderer(state.window,nullptr);
    if(!state.renderer)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","Error to creating a renderer",state.window);
        cleanUp(state);
        initSuccess = false;
    }
    SDL_SetRenderVSync(state.renderer,1);
    //configure presentation
    SDL_SetRenderLogicalPresentation(state.renderer,state.logW,state.logH,SDL_LOGICAL_PRESENTATION_LETTERBOX);
    
    return initSuccess;
}


// Cleanup
void cleanUp(SDLState &state)
{
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}

void drawObject(const SDLState &state, GameState &gs, GameObject &obj,float width, float height, float deltaTime)
{       
    float srcX = obj.currentAnimation != -1? obj.animations[obj.currentAnimation].currentFrame() * width : (obj.spriteFrame-1)*width;
    SDL_FRect src
    {
        .x = srcX,
        .y = 0,
        .w = width,
        .h = height
    };
    SDL_FRect dst
    {
        .x = obj.position.x - gs.mapViewPort.x,
        .y = obj.position.y,
        .w = width,
        .h = height
    };

    // if(!flipHorizontal)
    // {
    //     SDL_RenderTexture(state.renderer,idleTex,&src,&des);
    // }else
    // {
    //     SDL_RenderTextureRotated(state.renderer,idleTex,&src,&des,0,nullptr,SDL_FLIP_HORIZONTAL);
    // }
    SDL_FlipMode flipMode = obj.direction == -1 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    if(!obj.shouldFlash)
    {
        SDL_RenderTextureRotated(state.renderer, obj.texture, &src, &dst, 0, nullptr, flipMode);
    }
    else
    {   
        //flash object with a redis tint
        SDL_SetTextureColorModFloat(obj.texture, 2.5f, 1.0f, 1.0f);
        SDL_RenderTextureRotated(state.renderer, obj.texture, &src, &dst, 0, nullptr, flipMode);
        SDL_SetTextureColorModFloat(obj.texture, 1.0f, 1.0f, 1.0f);
        if(obj.flashTimer.step(deltaTime))
        {
            obj.shouldFlash = false;
        }
    }
}

void update(const SDLState &state, GameState &gs,Resources &res, GameObject &obj, float deltaTime)
{
    if(obj.currentAnimation!=-1)
    {
        obj.animations[obj.currentAnimation].step(deltaTime);
    }
    if(obj.dynamic && !obj.grounded)
    {
        //gravity
        obj.velocity += glm::vec2(0,500) * deltaTime;
    }
    float currentDirection = 0;
    if(obj.type == ObjectType::player)
    {
        if(state.keys[SDL_SCANCODE_A])
        {
            currentDirection += -1;
        }
        if(state.keys[SDL_SCANCODE_D])
        {
            currentDirection += 1;
        }
        
        Timer &weaponTimer = obj.data.player.weaponTimer;
        weaponTimer.step(deltaTime);
        //handle shooting
        const auto handleShooting = [&state, &gs, &res, &obj, &weaponTimer](SDL_Texture *tex, SDL_Texture *shootTex, int animIndex, int shootAnimIndex)
        {
            // Bullet animation
            if(state.keys[SDL_SCANCODE_J])
            {
                obj.texture = shootTex;
                obj.currentAnimation = shootAnimIndex;

                if (weaponTimer.isTimeout())
                {
                    // bullet Spawn
                    weaponTimer.reset();
                    GameObject bullet;
                    bullet.data.bullet = BulletData();
                    bullet.type = ObjectType::bullet;
                    bullet.direction = gs.player().direction;
                    bullet.texture = res.texBullet;
                    bullet.maxSpeedX = 1000.0f;
                    bullet.currentAnimation = res.ANIM_BULLET_MOVING;
                    bullet.collider = SDL_FRect
                    {
                        .x = 0,
                        .y = 0,
                        .w = static_cast<float>(res.texBullet->h),
                        .h = static_cast<float>(res.texBullet->h)
                    };
                    const int yVariation = 60;
                    const float yVelocity = SDL_rand(yVariation) - yVariation / 2.0f;
                    bullet.velocity = glm::vec2(
                        obj.velocity.x + 600.0f * obj.direction,
                        yVelocity
                    );
                    bullet.animations = res.bulletAnims;
                    //adjust bullet start position.
                    const float left = 4;
                    const float right = 20;
                    const float t = (obj.direction+1)/2.0f;
                    const float xOffset = left + right * t;
                    // cout<<obj.direction<<endl;
                    // cout<<xOffset<<endl;
                    bullet.position = glm::vec2(
                        obj.position.x + xOffset,
                        obj.position.y + TILE_SIZE/2 +1
                    );

                    bool foundInactive = false;
                    for(int i = 0; i<gs.bullets.size() && !foundInactive; i++)
                    {
                        if(gs.bullets[i].data.bullet.state == BulletState::inactive)
                        {
                            foundInactive = true;
                            gs.bullets[i] = bullet;
                        }
                    }
                    if (!foundInactive)
                    {
                        gs.bullets.push_back(bullet);
                    }
                    MIX_SetTrackAudio(res.shootTrack, res.shootSound);
                    cout<<"playing shooting audio"<<endl;
                    MIX_SetTrackGain(res.bgTrack, 0.5f);
                    MIX_PlayTrack(res.shootTrack, 0);
                }
            }
            else
            {
                obj.texture = tex;
                obj.currentAnimation = animIndex;
            }
        };

        switch(obj.data.player.state)
        {
            case PlayerState::idle:
            {
                if(currentDirection)
                {
                    obj.data.player.state = PlayerState::running;
                }else
                {
                    //decelerate
                    if(obj.velocity.x)
                    {
                        const int factor = obj.velocity.x>0? -1.5f: 1.5f;
                        float amount = factor * obj.acceleration.x * deltaTime;
                        if(abs(obj.velocity.x)<abs(amount))
                        {
                            obj.velocity.x = 0;
                        }else
                        {
                            obj.velocity.x += amount;
                        }
                    }
                }
                
                handleShooting(res.texIdle, res.texShoot, res.ANIM_PLAYER_IDLE, res.ANIM_PLAYER_SHOOT);
                break;
            }
            case PlayerState::running:
            {
                if(!currentDirection)
                {
                    obj.data.player.state = PlayerState::idle;
                }
                //creating running sliding
                if(obj.velocity.x*obj.direction<0 && obj.grounded)
                {
                    handleShooting(res.texSlide, res.texSlideShoot, res.ANIM_PLAYER_SLIDE, res.ANIM_PLAYER_SLIDE_SHOOT);
                }
                else
                {   
                    handleShooting(res.texRun, res.texRunShoot, res.ANIM_PLAYER_RUN, res.ANIM_PLAYER_RUN);
                }
                break;
            }
            case PlayerState::jumping:
            {   
                handleShooting(res.texRun, res.texRunShoot, res.ANIM_PLAYER_RUN, res.ANIM_PLAYER_RUN);
                // if(obj.grounded)
                // {
                //     if(abs(obj.velocity.x) > 0)
                //         obj.data.player.state = PlayerState::running;
                //     else
                //         obj.data.player.state = PlayerState::idle;
                // }
                break;
            }
        }
    }
    else if(obj.type == ObjectType::bullet)
    {
        switch(obj.data.bullet.state)
        {
            case BulletState::moving:
            {
                if (obj.position.x - gs.mapViewPort.x < 0 || obj.position.x - gs.mapViewPort.x > state.logW 
                    || obj.position.y - gs.mapViewPort.y < 0 || obj.position.y - gs.mapViewPort.y > state.logH )
                {
                    obj.data.bullet.state = BulletState::inactive;
                }
                break;
            }
            case BulletState::colliding:
            {
                if(obj.animations[obj.currentAnimation].isDone())
                {
                    obj.data.bullet.state = BulletState::inactive;
                }
                break;
            }
        }
    }
    else if(obj.type == ObjectType::enemy)
    {
        switch(obj.data.enemy.state)
        {
            case EnemyState::shambling:
            {
                glm::vec2 playerDir = gs.player().position - obj.position;
                if(glm::length(playerDir)<200)
                {
                    currentDirection = playerDir.x < 0? -1:1;
                    obj.acceleration = glm::vec2(30,0);
                }
                else
                {
                    obj.acceleration = glm::vec2(0);
                    obj.velocity.x = 0;
                }
                break;
            }
            case EnemyState::damage:
            {
                if (obj.data.enemy.damageTimer.step(deltaTime))
                {
                    obj.data.enemy.state = EnemyState::shambling;
                    obj.texture = res.texEnemy;
                    obj.currentAnimation = res.ANIM_ENEMY;
                }
                break;
            }
            case EnemyState::dead:
            {
                obj.velocity.x = 0;
                if(obj.currentAnimation != -1 && obj.animations[obj.currentAnimation].isDone())
                {
                    obj.currentAnimation = -1;
                    obj.spriteFrame = 18;
                }
                break;
            }
        }
    }

    if(currentDirection)
    {
        obj.direction = currentDirection;
    }
    //applying Physic here.
    // add acceleration to velocity
    // v = velocity
    // u = initial velocity
    // a = acceleration
    // t = time (delta time)
    obj.velocity += currentDirection * obj.acceleration* deltaTime;  //v = u + at
    if(abs(obj.velocity.x) > obj.maxSpeedX)
    {
        obj.velocity.x = (obj.velocity.x > 0 ? 1 : -1) * obj.maxSpeedX;
    }
    // cout<<obj.position[0]<<endl;
    // add velocity to position
    obj.position += obj.velocity * deltaTime;

    //handle collision detection
    bool foundGround = false;
    for(auto &layer: gs.layers)
    {
        for(GameObject &objB :layer)
        {
            if(&obj!=&objB)
            {
                checkCollision(state,gs,res,obj,objB,deltaTime);
                if (objB.type == ObjectType::level)
                //grounded
                {
                    SDL_FRect sensor
                    {
                        .x = obj.position.x + obj.collider.x,
                        .y = obj.position.y + obj.collider.y + obj.collider.h,
                        .w = obj.collider.w , .h = 1
                    };
                    SDL_FRect rectB
                    {
                        .x = objB.position.x + objB.collider.x,
                        .y = objB.position.y + objB.collider.y,
                        .w = objB.collider.w, 
                        .h = objB.collider.h
                    };
                    SDL_FRect rectC {0};
                    if(SDL_GetRectIntersectionFloat(&sensor,&rectB,&rectC))
                    {
                        foundGround = true;
                    }
                }
            }
        }
    }

    if(obj.grounded != foundGround)
    {
        //switching grounded state
        obj.grounded = foundGround;
        if(foundGround && obj.type == ObjectType::player)
        {
            if(abs(obj.velocity.x) > 0)
                obj.data.player.state = PlayerState::running;
            else
                obj.data.player.state = PlayerState::idle;
        }
    }
}

void collisionResponse(const SDLState &state,GameState &gs,Resources &res, const SDL_FRect &rectA, const SDL_FRect &rectB, const SDL_FRect &rectC, GameObject &objA, GameObject &objB, float deltaTime)
{
    const auto genericResponse = [&]
    {
        if(rectC.w < rectC.h)
        {
            //horizontal collision
            if(objA.velocity.x>0) //going right
            {
                objA.position.x -= rectC.w;
            }
            else if(objA.velocity.x<0) //going left
            {
                objA.position.x += rectC.w;
            }
            objA.velocity.x = 0;
        }
        else
        {
            //vertical collision
            if(objA.velocity.y>0) //going down
            {
                objA.position.y -= rectC.h;
            }
            else if(objA.velocity.y<0) //going up
            {
                objA.position.y += rectC.h;
            }
            objA.velocity.y = 0;
        }
    };
    //object we are checking
    if(objA.type==ObjectType::player)
    {
        //object is colliding with
        switch(objB.type)
        {
            case ObjectType::level:
            {
                genericResponse();
                break;
            }
            case ObjectType::enemy:
            {
                if(objB.data.enemy.state != EnemyState::dead)
                {
                    objA.velocity = glm::vec2(100,0) * -objA.direction;
                }
                break;
            }
        }
    }
    else if(objA.type == ObjectType::bullet)
    {
        bool passThrough = false;
        switch(objA.data.bullet.state)
        {
            case BulletState::moving:
            {
                switch(objB.type)
                {
                    case ObjectType::level:
                    {   
                        MIX_SetTrackAudio(res.wallTrack, res.wallHitSound);
                        cout<<"playing hitting wall audio"<<endl;
                        MIX_SetTrackGain(res.bgTrack, 0.5f);
                        MIX_PlayTrack(res.wallTrack, 0);
                        break;
                    }
                    case ObjectType::enemy:
                    {
                        EnemyData &d = objB.data.enemy;
                        if(d.state != EnemyState::dead)
                        {
                            objB.direction = -objA.direction;
                            objB.shouldFlash = true;
                            objB.flashTimer.reset();
                            // cout<<objB.shouldFlash<<endl;
                            objB.texture = res.texEnemyHit;
                            objB.currentAnimation = res.ANIM_BULLET_HIT;
                            d.state = EnemyState::damage;
                            // damage the enemy and flag dead if needed
                            d.healthPoint -= SDL_rand(5);
                            if(d.healthPoint<=0)
                            {
                                d.state = EnemyState::dead;
                                objB.texture = res.texEnemyDie;
                                objB.currentAnimation = res.ANIM_ENEMY_DIE;
                                MIX_SetTrackAudio(res.dieTrack, res.enemyDieSound);
                                cout<<"playing enemy die audio"<<endl;
                                MIX_SetTrackGain(res.bgTrack, 1.0f);
                                MIX_PlayTrack(res.dieTrack, 0);
                            }
                        }
                        else
                        {
                            passThrough = true;
                        }
                        cout<<d.healthPoint<<" Damage get : "<<SDL_rand(5)+5<<endl;
                        MIX_SetTrackAudio(res.hitTrack, res.enemyHitSound);
                        cout<<"playing enemy audio"<<endl;
                        MIX_SetTrackGain(res.bgTrack, 0.5f);
                        MIX_PlayTrack(res.hitTrack, 0);
                        break;
                    }
                }
                if(!passThrough)
                {
                    genericResponse();
                    objA.velocity *= 0;
                    objA.data.bullet.state = BulletState::colliding;
                    objA.texture = res.texBulletHit;
                    objA.currentAnimation = res.ANIM_BULLET_HIT;
                }
                break;
            }
        }
    }
    else if(objA.type == ObjectType::enemy)
    {
        genericResponse();
    }
}

void checkCollision(const SDLState &state, GameState &gs, Resources &res, GameObject &a, GameObject &b, float deltaTime)
{
    SDL_FRect rectA
    {
        .x = a.position.x + a.collider.x, 
        .y = a.position.y +a.collider.y,
        .w = a.collider.w, 
        .h = a.collider.h
    };
    SDL_FRect rectB
    {
        .x = b.position.x + b.collider.x, 
        .y = b.position.y + b.collider.y,
        .w = b.collider.w, 
        .h = b.collider.h
    };
    SDL_FRect rectC {0};
    
    if(SDL_GetRectIntersectionFloat(&rectA,&rectB,&rectC))
    {
        //Found intersection response accordingly.
        collisionResponse(state, gs,res,rectA,rectB,rectC,a,b,deltaTime);
    }

}

void createTiles(const SDLState &state, GameState &gs, const Resources &res)
{
    /*
        1 - Ground
        2 - Panel
        3 - Enemy
        4 - Player
        5 - Grass
        6 - Brick
    */
    short map[MAP_ROWS][MAP_COLS] =
    {
        4,0,0,0,0,0,0,0,3,0,0,0,2,2,2,0,0,0,0,3,0,0,2,0,0,0,0,3,0,0,2,2,2,0,0,0,0,3,0,0,2,2,0,0,0,0,3,0,0,0,

        0,0,0,2,2,0,0,0,0,0,0,3,0,0,0,0,2,2,2,0,0,0,0,0,3,0,0,0,2,2,0,0,0,0,3,0,0,0,2,0,0,0,0,3,0,0,2,0,0,0,

        0,0,2,0,0,0,3,0,0,2,0,0,0,0,3,0,0,0,0,0,2,0,0,0,0,3,0,0,0,2,0,0,0,3,0,0,0,2,0,0,0,0,3,0,0,2,0,0,0,0,

        0,2,0,0,0,2,0,0,0,0,2,2,0,0,0,3,0,0,2,0,0,0,3,0,0,0,0,2,0,0,0,3,0,0,2,0,0,0,3,0,0,2,0,0,0,0,2,0,0,0,

        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
    };
    short foreGround[MAP_ROWS][MAP_COLS] =
    {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

        5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,

        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };
    short backGround[MAP_ROWS][MAP_COLS] =
    {
        6,6,6,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,6,6,6,0,0,0,0,0,6,6,6,0,0,0,0,0,6,6,6,0,0,0,0,6,6,6,0,0,0,0,

        6,0,6,0,0,0,0,0,0,0,0,0,6,0,6,0,0,0,0,0,6,0,6,0,0,0,0,0,6,0,6,0,0,0,0,0,6,0,6,0,0,0,0,6,0,6,0,0,0,0,

        6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

        6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };

    const auto loadMap = [&state, &gs, &res](short layer[MAP_ROWS][MAP_COLS])
    {
        const auto createObject = [&state](int r, int c, SDL_Texture *tex, ObjectType type)
        {
            GameObject o;
            o.type = type;
            o.position = glm::vec2(
                            c*TILE_SIZE,
                            state.logH - (MAP_ROWS - r) * TILE_SIZE
                        );
            o.texture = tex;
            o.collider =
            {
                .x = 0,
                .y = 0,
                .w = TILE_SIZE,
                .h = TILE_SIZE
            };
            return o;
        };

        for(int r = 0;r<MAP_ROWS;r++)
        {
            for(int c = 0;c<MAP_COLS;c++)
            {
                switch(layer[r][c])
                {
                    case 1: //Ground
                    {
                        GameObject o = createObject(r,c,res.texGround,ObjectType::level);
                        gs.layers[LAYER_IDX_LEVEL].push_back(o);
                        break;
                    }
                    case 2: //Panel
                    {
                        GameObject o = createObject(r,c,res.texPanel,ObjectType::level);
                        gs.layers[LAYER_IDX_LEVEL].push_back(o);
                        break;
                    }
                    case 3:
                    {
                        GameObject enemy = createObject(r,c,res.texEnemy,ObjectType::enemy);
                        enemy.animations = res.enemyAnims;
                        enemy.data.enemy = EnemyData();
                        enemy.currentAnimation = res.ANIM_ENEMY;
                        enemy.collider =
                        SDL_FRect {
                            .x = 10,
                            .y = 6,
                            .w = 12,
                            .h = 26
                        };
                        enemy.maxSpeedX = 30;
                        enemy.dynamic = true;
                        gs.layers[LAYER_IDX_CHARACTERS].push_back(enemy);
                        break;
                    }
                    case 4: //Player
                    {
                        // create player.
                        GameObject player = createObject(r,c,res.texIdle,ObjectType::player);
                        player.position = glm::vec2(
                            c*TILE_SIZE,
                            state.logH - (MAP_ROWS - r) * TILE_SIZE
                        );
                        player.data.player = PlayerData();
                        player.acceleration = glm::vec2(300,0);
                        player.maxSpeedX = 100;
                        player.animations = res.playerAnims;
                        player.currentAnimation = res.ANIM_PLAYER_IDLE;
                        player.dynamic = true;
                        player.collider =
                        {
                            .x = 10,
                            .y = 6,
                            .w = 12,
                            .h = 26
                        };
                        gs.layers[LAYER_IDX_CHARACTERS].push_back(player);
                        gs.playerIndex = gs.layers[LAYER_IDX_CHARACTERS].size()-1;
                        break;
                    }
                    case 5: //grass
                    {
                        GameObject o = createObject(r,c,res.texGrass,ObjectType::level);
                        gs.foreGroundTiles.push_back(o);
                        break;
                    }
                    case 6: //brick
                    {
                        GameObject o = createObject(r,c,res.texBrick,ObjectType::level);
                        gs.backgroundTiles.push_back(o);
                        break;
                    }
                }
            }
        }
    };
    loadMap(map);
    loadMap(foreGround);
    loadMap(backGround);
    assert(gs.playerIndex != -1);
}

void handleKeyInput(const SDLState &state, GameState &gs, GameObject &obj, SDL_Scancode key, bool keyDown)
{
    const float JUMP_FORCE = -200.0f;
    if(obj.type==ObjectType::player)
    {
        switch(obj.data.player.state)
        {
            case PlayerState::idle:
            {
                if(key == SDL_SCANCODE_SPACE && keyDown && obj.grounded)
                {
                    obj.data.player.state = PlayerState::jumping;
                    obj.velocity.y = JUMP_FORCE;
                    obj.grounded = false;
                }
                break;
            }
            case PlayerState::running:
            {
                if(key == SDL_SCANCODE_SPACE && keyDown && obj.grounded)
                {
                    obj.data.player.state = PlayerState::jumping;
                    obj.velocity.y = JUMP_FORCE;  // IMPORTANT: use = not +=
                    obj.grounded = false;
                }
                break;
            }
        }
    }

}

void drawParallaxBackground(SDL_Renderer *renderer, SDL_Texture *texture, float xVelocity, float &scrollPos, float scrollFactor, float deltaTime)
{
    scrollPos -= xVelocity * scrollFactor * deltaTime;
    if(scrollPos <= -texture->w)
    {
        scrollPos = 0;
    }
    SDL_FRect dst
    {
        .x = scrollPos,
        .y = 10,
        .w = texture->w * 2.0f,
        .h = static_cast<float>(texture->h)
    };
    SDL_RenderTextureTiled(renderer, texture, nullptr,1 ,&dst);
}