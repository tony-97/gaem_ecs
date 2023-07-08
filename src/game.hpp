#pragma once

#include "game_factory.hpp"
#include "resource_manager.hpp"

#include "systems/render.hpp"
#include "systems/uhd_render.hpp"
#include "systems/debug_render.hpp"
#include "systems/input.hpp"
#include "systems/physics.hpp"
#include "systems/animation.hpp"
#include "systems/collider.hpp"
#include "systems/health.hpp"
#include "systems/timer.hpp"
#include "systems/spawn.hpp"
#include "systems/charge.hpp"
#include "systems/engine.hpp"

struct Game
{
    explicit Game()
    {
        ren_sys.set_background(res_man.tBackground);
        game_fact.CreatePlayer(screen_width, screen_height);
        game_fact.CreateAsteroidSpawners(screen_width, screen_height);
    }

    template<class Ent_t>
    constexpr void Destroy(Ent_t ent)
    {
        ecs_man.Match(ent,
        [&](ECS::Handle_t<Asteroids_t> eid) {
            const auto& ren { ecs_man.GetComponent<RenderComponent_t>(eid)  };
            const auto& phy { ecs_man.GetComponent<PhysicsComponent_t>(eid) };
            game_fact.CreateAsteroidExplosion(phy.position, phy.rotation, ren.scale);
            game_fact.CreateFragmentAsteroids(phy.position, phy.rotation, ren.scale);
            player_score += 10 * ren.scale;        
        },
        [&](ECS::Handle_t<Bullet_t> eid) {
            const auto& phy { ecs_man.GetComponent<PhysicsComponent_t>(eid) };
            game_fact.CreateLaserExplosion(phy.position, phy.rotation);
        },
        [&](ECS::Handle_t<Player_t> eid) {
            const auto& phy { ecs_man.GetComponent<PhysicsComponent_t>(eid) };
            const auto& ren { ecs_man.GetComponent<RenderComponent_t>(eid) };
            game_fact.CreateShipExplosion(phy.position, phy.rotation, ren.scale);
            if (--player_lives != 0) {
                game_fact.CreatePlayer(screen_width, screen_height);
            }
        });
        ecs_man.Destroy(ent);
    }

    template<class Fn_t, class... Args_t> constexpr static auto
    DrawingRoutine(Fn_t&& fn, Args_t&&... args) -> void
    {
        BeginDrawing();
        ClearBackground(BLACK);
        fn(std::forward<Args_t>(args)...);
        EndDrawing();
    }

    template<class Ent_t>
    constexpr void Enable(Ent_t ent, InputEnablerComponent_t::Object_t type)
    {
        ecs_man.Match(ent,
        [&](ECS::Handle_t<RocketOff_t> eid) {
            switch (type) {
            case InputEnablerComponent_t::ROCKET_BOTTOM: {
              ecs_man.TransformTo<RocketOn_t>(eid);
            } break;
            default: break;
            }
        },
        [&](ECS::Handle_t<WeaponOff_t> eid) {
            auto& chrg { ecs_man.GetComponent<ChargeComponent_t>(eid) };
            chrg.charged = 0;
            auto e = ecs_man.TransformTo<WeaponOn_t>(eid);
            auto& spwn { ecs_man.GetComponent<SpawnComponent_t>(e) };
            spwn.to_spawn = chrg.charged;
        });
    }

    template<class Ent_t>
    constexpr void Disable(Ent_t ent, InputEnablerComponent_t::Object_t)
    {
        ecs_man.Match(ent,
        [&](ECS::Handle_t<RocketOn_t> eid) {
            ecs_man.TransformTo<RocketOff_t>(eid);
        },
        [&](ECS::Handle_t<WeaponOn_t> eid) {
            auto& chrg { ecs_man.GetComponent<ChargeComponent_t>(eid) };
            const auto& spwn { ecs_man.GetComponent<SpawnComponent_t>(eid) };
            chrg.charged = spwn.to_spawn;
            auto& ren { ecs_man.GetComponent<RenderComponent_t>(eid) };
            ren.crop_rec.x = 0;
            ecs_man.TransformTo<WeaponOff_t>(eid);
        });
    }

    void run()
    {
        while (!WindowShouldClose() && player_lives > 0) {
            spawn_sys.update(ecs_man, GetFrameTime());
            tim_sys.update(ecs_man, GetFrameTime());
            chrg_sys.update(ecs_man, GetFrameTime());
            anim_sys.update(ecs_man, GetFrameTime());
            //col_sys.update(ecs_man, screen_width, screen_height);
            DrawingRoutine([&](){
                ren_sys.update(ecs_man);
                dbg_ren.update(ecs_man);
                uhd_ren.update(ecs_man, player_score, player_max_lives, player_lives);
            });
            inp_sys.update(ecs_man, *this);
            phy_sys.update(ecs_man, GetFrameTime());
            eng_sys.update(ecs_man);
            hel_sys.update(ecs_man, *this);
        }
        while (!WindowShouldClose()) {
            tim_sys.update(ecs_man, GetFrameTime());
            anim_sys.update(ecs_man, GetFrameTime());
            col_sys.update(ecs_man, screen_width, screen_height);
            DrawingRoutine([&](){
                ren_sys.update(ecs_man);
                dbg_ren.update(ecs_man);
                uhd_ren.update(ecs_man, player_score, player_max_lives, player_lives);
            });
            phy_sys.update(ecs_man, GetFrameTime());
            hel_sys.update(ecs_man, *this);
        }
    }
private:
    static constexpr inline int screen_width  { 1000 };
    static constexpr inline int screen_height { 600 };

    unsigned player_lives { 3 };
    unsigned player_max_lives { 3 };
    unsigned player_score {   };

    RenderSystem_t    ren_sys   { screen_width, screen_height, "GAEM ECS!" };
    UHDRenderSystem_t uhd_ren   { screen_width, screen_height, res_man };
    DebugRenderSystem_t dbg_ren {  };
    InputSystem_t     inp_sys   {  };
    AnimationSystem_t anim_sys  {  };
    PhysicsSystem_t   phy_sys   {  };
    ColliderSystem_t  col_sys   {  };
    HealthSystem_t    hel_sys   {  };
    TimerSystem_t     tim_sys   {  };
    SpawnSystem_t     spawn_sys {  };
    ChargeSystem_t    chrg_sys  {  };
    EngineEffectSystem_t eng_sys   {  };

    const ResourceManager_t res_man {  };

    ECSMan_t ecs_man {  };
    EVMan_t ev_man {  };

    GameFactory_t<ECSMan_t, ResourceManager_t> game_fact { ecs_man, res_man };
};
