#pragma once

#include "game_factory.hpp"
#include "resource_manager.hpp"

#include "systems/render.hpp"
#include "systems/input.hpp"
#include "systems/animation.hpp"
#include "systems/collider.hpp"
#include "systems/health.hpp"
#include "systems/bullet.hpp"
#include "systems/spawn.hpp"
#include "systems/charge.hpp"

struct Game
{
    explicit Game()
    {
        ren_sys.set_background(res_man.GetTextureBackground());
        game_fact.CreatePlayer(screen_width, screen_height);
        game_fact.CreateAsteroidSpawners(screen_width, screen_height);
    }

    void run()
    {
        while (!WindowShouldClose()) {
            ren_sys.update(ecs_man);
            inp_sys.update(ecs_man, game_fact);
            anim_sys.update(ecs_man, GetFrameTime());
            ecs_man.ForEachEntity<Movable_t>(
                    [&](auto& phy, auto&&) {
                        phy.w += phy.a * GetFrameTime();
                        phy.vel.x += phy.acc.x * GetFrameTime();
                        phy.vel.y += phy.acc.y * GetFrameTime();
            
                        phy.ang += phy.w * GetFrameTime();
                        phy.pos.x += phy.vel.x * GetFrameTime();
                        phy.pos.y += phy.vel.y * GetFrameTime();
            
                        phy.w -= phy.w * phy.friction * GetFrameTime();
                        phy.vel.x -= phy.vel.x * phy.friction * GetFrameTime();
                        phy.vel.y -= phy.vel.y * phy.friction * GetFrameTime();
                    });
            col_sys.update(ecs_man, screen_width, screen_height);
            hel_sys.update(ecs_man);
            bull_sys.update(ecs_man, GetFrameTime());
            spawn_sys.update(ecs_man, game_fact, GetFrameTime());
            chrg_sys.update(ecs_man, GetFrameTime());
        }    
    }

    static constexpr inline int screen_width  { 800 };
    static constexpr inline int screen_height { 600 };

    RenderSystem_t    ren_sys   { screen_width, screen_height, "GAEM ECS!" };
    InputSystem_t     inp_sys   {  };
    AnimationSystem_t anim_sys  {  };
    ColliderSystem_t  col_sys   {  };
    HealthSystem_T    hel_sys   {  };
    BulletSystem_t    bull_sys  {  };
    SpawnSystem_t     spawn_sys {  };
    ChargeSystem_t    chrg_sys  {  };

    ResourceManager_t res_man {  };

    ECSMan_t ecs_man {  };

    GameFactory_t<ECSMan_t, ResourceManager_t> game_fact { ecs_man, res_man };
};
