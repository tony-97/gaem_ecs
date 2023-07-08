#pragma once

#include <raylib.h>

#include <nlohmann/json.hpp>

#include "math_vector.hpp"
#include "types.hpp"

#include "components/render.hpp"
#include "components/physics.hpp"
#include "components/animation.hpp"
#include "components/input_enabler.hpp"
#include "components/player_input.hpp"
#include "components/health.hpp"
#include "components/timer.hpp"
#include "components/spawn.hpp"
#include "components/charge.hpp"
#include "components/collider.hpp"
#include "components/parent.hpp"
#include "components/engine_effect.hpp"

#include <cmath>

#include <initializer_list>
#include <string_view>
#include <fstream>
#include <limits>

using json = nlohmann::json;

template<class ECSMan_t, class ResMan_t>
struct GameFactory_t
{
    constexpr explicit GameFactory_t(ECSMan_t& ecs_man, const ResMan_t& res_man)
        : mECSMan { ecs_man }, mResMan { res_man }
    {
        mData = json::parse(std::ifstream{ "./game_config.json" });
    }

    constexpr auto GetAnimationComponent(std::string_view name)
    {
        return AnimationComponent_t {
            .frames = mData[name]["animation"]["tiles"].get<int>(),
            .frame_time = mData[name]["animation"]["interval"].get<float>(),
        };
    }

    constexpr auto GetRenderComponent(std::string_view name, Texture2D sprite, float scale = 1.0f)
    {
        auto frames { mData[name]["animation"]["tiles"].get<int>() };
        auto height { mData[name]["animation"]["height"].get<float>() };
        return RenderComponent_t {
            .sprite = sprite,
            .crop_rec = { .width = sprite.width / (frames * 1.0f),
                          .height = sprite.height * height },
            .scale = scale

        };
    }

    constexpr auto GetRenderComponentFrom(AnimationComponent_t anim, Texture2D sprite, float scale = 1.0f)
    {
        return RenderComponent_t {
            .sprite = sprite,
            .crop_rec = { .width = sprite.width / (anim.frames * 1.0f),
                          .height = sprite.height * 1.0f },
            .scale = scale
        };
    }

    constexpr auto GetRenderComponentFrom(Texture2D sprite, float scale = 1.0f)
    {
        return RenderComponent_t {
            .sprite = sprite,
            .crop_rec = { .width = sprite.width * 1.0f, .height = sprite.height * 1.0f },
            .scale = scale
        };
    }

    constexpr auto GetPhysicComponent(RenderComponent_t ren, Vec2f_t pos, float vel = 0.0f, float rot = 0.0f, float fric = 0.0f, Vec2f_t orig = { 0.5f, 0.5f }, Vec2f_t acc = { 0.0f, 0.0f })
    {
        return PhysicsComponent_t {
            .position = pos,
            .velocity = { vel * std::sin(rot * DEG2RAD), -vel * std::cos(rot * DEG2RAD)  },
            .acceleration = acc,
            .origin = { .x = ren.crop_rec.width  * orig.x * ren.scale,
                        .y = ren.crop_rec.height * orig.y * ren.scale },
            .rotation = rot,
            .friction = fric
        };
    }

    auto CreateAsteroidSpawners(int screen_width, int screen_height)
    {
        const float sw { static_cast<float>(screen_width) };
        const float sh { static_cast<float>(screen_height) };
        const PhysicsComponent_t left_asteroid_spawner_phy_args {
            Vec2f_t{ 0.0f, 0.0f },
            Vec2f_t{ 0.0f, 150.0f },
            Vec2f_t{  },
            Vec2f_t{  },
            90.0f
        };
        const PhysicsComponent_t right_asteroid_spawner_phy_args {
            Vec2f_t{ sw, sh },
            Vec2f_t{ 0.0f, -150.0f },
            Vec2f_t{  },
            Vec2f_t{  },
            -90.0f
        };
        const PhysicsComponent_t top_asteroid_spawner_phy_args {
            Vec2f_t{ sw, 0.0f },
            Vec2f_t{ -150.0f, 0.0f },
            Vec2f_t{  },
            Vec2f_t{  },
            0.0f,
            180.0f,
        };
        const PhysicsComponent_t bottom_asteroid_spawner_phy_args {
            Vec2f_t{ 0.0f, sh },
            Vec2f_t{ 150.0f, 0.0f },
            Vec2f_t{  },
            Vec2f_t{  },
            0.0f
        };
        const SpawnComponent_t asteroid_spawner_spawn_args {
            [&](Vec2f_t pos, float rot) {
                CreateAsteroid(pos, rot);
            },
            1.0f,
            std::numeric_limits<unsigned>::max()
        };
        mECSMan.template CreateEntity<AsteroidsSpawner_t>(left_asteroid_spawner_phy_args,
                                                          asteroid_spawner_spawn_args);
        mECSMan.template CreateEntity<AsteroidsSpawner_t>(right_asteroid_spawner_phy_args,
                                                          asteroid_spawner_spawn_args);
        mECSMan.template CreateEntity<AsteroidsSpawner_t>(top_asteroid_spawner_phy_args,
                                                          asteroid_spawner_spawn_args);
        mECSMan.template CreateEntity<AsteroidsSpawner_t>(bottom_asteroid_spawner_phy_args,
                                                          asteroid_spawner_spawn_args);
    }

    constexpr auto CreateAsteroid(Vec2f_t pos, float rot)
    {
        rot += GetRandomValue(-30, 30);
        float size = GetRandomValue(1, 3);
        float vel  = GetRandomValue(100, 300);
        const RenderComponent_t ren_args {
            GetRenderComponentFrom(mResMan.tAsteroid, size)
        };
        const PhysicsComponent_t phy_args { GetPhysicComponent(ren_args, pos, vel, rot) };
        const ColliderComponent_t coll_args {
            static_cast<int>(19 * size)
        };
        const HealthComponent_t hel_args {
            static_cast<int>(size) * 40
        };
        mECSMan.template CreateEntity<Asteroids_t>(ren_args, phy_args, coll_args, hel_args);
    }

    constexpr auto CreateFragmentAsteroids(Vec2f_t pos, float rot, float size) 
    {
        
    }

    constexpr auto CreateExplosion(Texture2D explosion, int frames, float frame_speed, Vec2f_t pos, float rot, float scale)
    {
        const AnimationComponent_t anim_args {
            frames,
            frame_speed,
        };
        const RenderComponent_t ren_args {
            GetRenderComponentFrom(anim_args, explosion, scale)
        };
        const TimerComponent_t tim_args {
            frames * frame_speed,
        };
        const PhysicsComponent_t phy_args {
            GetPhysicComponent(ren_args, pos, 0.0f, rot)
        };
        mECSMan.template CreateEntity<Explosion_t>(ren_args, anim_args, tim_args, phy_args);
    }

    constexpr auto CreateAsteroidExplosion(Vec2f_t pos, float rot, float scale)
    {
        CreateExplosion(mResMan.tAsteroidExplosion, 8, 0.1f, pos, rot, scale * 0.5f);
    }

    constexpr auto CreateLaserExplosion(Vec2f_t pos, float rot)
    {
        CreateExplosion(mResMan.tLaserExplosion, 20, 0.02, pos, rot, 0.3f);
    }

    constexpr auto CreateShipExplosion(Vec2f_t pos, float rot, float scale)
    {
        CreateExplosion(mResMan.tShipExplosion, 48, 0.05f, pos, rot, scale);
    }

    constexpr auto CreateBullet(Vec2f_t pos, float rot)
    {
        PlaySound(mResMan.sLaser);
        const auto texture { mResMan.tProjectileRocket };
        float size { 0.5f };
        const AnimationComponent_t anim_args {
            GetAnimationComponent("projectile_rocket")
        };
        const RenderComponent_t ren_args {
            GetRenderComponentFrom(anim_args, texture, size)
        };
        rot += GetRandomValue(-2, 2);
        const PhysicsComponent_t phy_args {
            GetPhysicComponent(ren_args, pos, mData["projectile_rocket"]["velocity"].get<float>(), rot)
        };
        const ColliderComponent_t coll_args {
            static_cast<int>(texture.width * size / 32),
        };
        mECSMan.template CreateEntity<Bullet_t>(ren_args, anim_args, phy_args, coll_args);
    }

    constexpr auto CreatePlayer(int screen_width, int screen_height) -> void
    {
        const auto player_sprite { mResMan.tShip };
        const Vec2f_t player_pos { screen_width / 2.0f, screen_height / 2.0f };
        const RenderComponent_t player_ren_args {
            GetRenderComponentFrom(player_sprite)
        };
        const PhysicsComponent_t player_phy_args { GetPhysicComponent(player_ren_args, player_pos, 0.0f, 0.0f, PLAYER_FRICTION) };
        const ColliderComponent_t player_coll_args {
            static_cast<int>(player_sprite.width)
        };
        const InputEnablerComponent_t rocket_bottom_inp_args {
            KEY_W,
            InputEnablerComponent_t::ROCKET_BOTTOM
        };
        const AnimationComponent_t rocket_bottom_anim_args {
            GetAnimationComponent("engine_effect")
        };
        const RenderComponent_t bullet_spawner_ren_args {
            GetRenderComponent("weapon", mResMan.tWeapon)
        };
        const InputEnablerComponent_t bullet_spawner_inp_args {
            KEY_SPACE,
            InputEnablerComponent_t::BULLET_SPAWNER
        };
        const ChargeComponent_t bullet_spawner_chrg_args {
            CHARGE_INTERVAL,
            MAX_BULLETS,
            MAX_BULLETS
        };
        const SpawnComponent_t bullet_spawner_spwn_args {
                [&, next_position=0u](Vec2f_t pos, float rot) mutable {
                    //Vec2f_t positions[] = { Vec2f_t{}, Vec2f_t{} };
                    CreateBullet(pos, rot);
                    next_position = (next_position + 1) % 2;
                },
                0.1f,
        };
        const auto eid0 = mECSMan.template CreateEntity<Engine_t>(player_phy_args,
                                                                 GetRenderComponentFrom(mResMan.tEngine));
        const auto eid1 = mECSMan.template CreateEntity<WeaponOff_t>(bullet_spawner_spwn_args,
                                                                     bullet_spawner_inp_args,
                                                                     player_phy_args,
                                                                     bullet_spawner_ren_args,
                                                                     bullet_spawner_chrg_args);
        const auto eid2 = mECSMan.template CreateEntity<RocketOff_t>(rocket_bottom_inp_args, player_phy_args, GetRenderComponent("engine_effect", mResMan.tEngineEffect), rocket_bottom_anim_args);
        const auto phys = { mECSMan.template GetBase<Movable_t>(eid0), mECSMan.template GetBase<Movable_t>(eid1), mECSMan.template GetBase<Movable_t>(eid2) };
        const auto hels = { mECSMan.template GetBase<Alive_t>(eid0), mECSMan.template GetBase<Alive_t>(eid1), mECSMan.template GetBase<Alive_t>(eid2) };
        const ParentComponent_t<Movable_t> parent_phy_args {
            phys
        };
        const ParentComponent_t<Alive_t> parent_hel_args {
            hels
        };
        mECSMan.template CreateEntity<Player_t>(player_ren_args,
                                                player_phy_args,
                                                player_coll_args,
                                                parent_phy_args,
                                                parent_hel_args);
    }
private:
    ECSMan_t& mECSMan {  };
    json mData {  };
    const ResMan_t& mResMan {  };
    constexpr static inline float PLAYER_FRICTION { 0.8f };

    constexpr static inline unsigned MAX_BULLETS  { 50u };
    constexpr static inline float CHARGE_INTERVAL { 0.25f };
};
