#pragma once

#include "game_factory.hpp"
#include "interface.hpp"
#include "resource_manager.hpp"

#include "systems/render.hpp"
#include "systems/input.hpp"
#include "systems/physics.hpp"
#include "systems/animation.hpp"
#include "systems/collider.hpp"
#include "systems/health.hpp"
#include "systems/timer.hpp"
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

    template<class Ent_t>
    constexpr void Destroy(const Ent_t& ent)
    {
        if constexpr (ECS::IsInstanceOf_v<Asteroids_t, decltype(ent)>) {
            auto& phy { ecs_man.template GetComponent<PhysicsComponent_t>(ent) };
            game_fact.CreateAsteroidExplosion(phy.pos, phy.ang, phy.size);
            game_fact.CreateFragmentAsteroids(phy.pos, phy.ang, phy.size);
            player_score += 10 * phy.size;
        } else if constexpr (ECS::IsInstanceOf_v<Bullet_t, decltype(ent)>) {
            auto& phy { ecs_man.template GetComponent<PhysicsComponent_t>(ent) };
            game_fact.CreateLaserExplosion(phy.pos, phy.ang);
        } else if constexpr (ECS::IsInstanceOf_v<Player_t, decltype(ent)>) {
            auto& phy { ecs_man.template GetComponent<PhysicsComponent_t>(ent) };
            game_fact.CreateShipExplosion(phy.pos, phy.ang, phy.size);
            --player_lives;
        }
        ecs_man.Destroy(ent);
    }

    template<class Ent_t>
    constexpr void Enable(Ent_t&& ent, InputEnablerComponent_t::Object_t type)
    {
        if constexpr (ECS::IsInstanceOf_v<RocketOff_t, decltype(ent)>) {
            switch (type) {
            case InputEnablerComponent_t::ROCKET_FRONT:
              ecs_man.template TransformTo<RocketOn_t>(ent,
                                                     game_fact.GetRocketFrontRenArgs(),
                                                     game_fact.GetRocketAnimArgs());
                break;
            case InputEnablerComponent_t::ROCKET_BOTTOM:
              ecs_man.template TransformTo<RocketOn_t>(ent,
                                                     game_fact.GetRocketBottomRenArgs(),
                                                     game_fact.GetRocketAnimArgs());
                break;
            default:
                break;
            }
        } else if constexpr (ECS::IsInstanceOf_v<BulletSpawnerOff_t, decltype(ent)>) {
            auto& chrg { ecs_man.template GetComponent<ChargeComponent_t>(ent) };
            Args::Arguments_t spwn_args {
                Args::For_v<SpawnComponent_t>,
                [&](Vector2 pos, float rot) {
                    game_fact.CreateBullet(pos, rot);
                },
                0.1f,
                0.15f,
                chrg.charged,
            };
            chrg.charged = 0;
            ecs_man.template TransformTo<BulletSpawnerOn_t>(ent, spwn_args);
        }
    }

    template<class Ent_t>
    constexpr void Disable(Ent_t&& ent, InputEnablerComponent_t::Object_t)
    {
        if constexpr (ECS::IsInstanceOf_v<RocketOn_t, decltype(ent)>) {
            ecs_man.template TransformTo<RocketOff_t>(ent);
        } else if constexpr (ECS::IsInstanceOf_v<BulletSpawnerOn_t, decltype(ent)>) {
            auto& chrg { ecs_man.template GetComponent<ChargeComponent_t>(ent) };
            auto& spwn { ecs_man.template GetComponent<SpawnComponent_t>(ent) };
            chrg.charged = spwn.spawned;
            ecs_man.template TransformTo<BulletSpawnerOff_t>(ent);
        }
    }

    void run()
    {
        while (!WindowShouldClose() && player_lives > 0) {
            ren_sys.update(ecs_man, player_score, player_max_lives, player_lives);
            inp_sys.update(ecs_man, *this);
            anim_sys.update(ecs_man, GetFrameTime());
            phy_sys.update(ecs_man, GetFrameTime());
            col_sys.update(ecs_man, screen_width, screen_height);
            hel_sys.update(ecs_man, *this);
            tim_sys .update(ecs_man, GetFrameTime());
            spawn_sys.update(ecs_man, GetFrameTime());
            chrg_sys.update(ecs_man, GetFrameTime());
        }
    }
private:
    static constexpr inline int screen_width  { 800 };
    static constexpr inline int screen_height { 600 };

    unsigned player_lives { 3 };
    unsigned player_max_lives { 3 };
    unsigned player_score {   };

    RenderSystem_t    ren_sys   { screen_width, screen_height, "GAEM ECS!", res_man };
    InputSystem_t     inp_sys   {  };
    AnimationSystem_t anim_sys  {  };
    PhysicsSystem_t   phy_sys   {  };
    ColliderSystem_t  col_sys   {  };
    HealthSystem_t    hel_sys   {  };
    TimerSystem_t     tim_sys   {  };
    SpawnSystem_t     spawn_sys {  };
    ChargeSystem_t    chrg_sys  {  };

    ResourceManager_t res_man {  };

    ECSMan_t ecs_man {  };

    GameFactory_t<ECSMan_t, ResourceManager_t> game_fact { ecs_man, res_man };
};
