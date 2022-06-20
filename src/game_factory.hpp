#pragma once

#include <arguments.hpp>

#include "raylib.h"
#include "types.hpp"

#include <components/render.hpp>
#include <components/physics.hpp>
#include <components/animation.hpp>
#include <components/player_input.hpp>
#include <components/input_enabler.hpp>
#include <components/health.hpp>
#include <components/timer.hpp>
#include <components/spawn.hpp>
#include <components/charge.hpp>
#include <components/collider.hpp>

#include <cmath>

#include <limits>

template<class ECSMan_t, class ResMan_t>
struct GameFactory_t
{
    constexpr explicit GameFactory_t(ECSMan_t& ecs_man, ResMan_t& res_man)
        : mECSMan { ecs_man }, mResMan { res_man }
    {
        
    }

    constexpr auto GetPlayerCropRect() const
    {
        const auto player_sprite { mResMan.GetTexturePlayer() };
        return Rectangle {
            0.0f, 0.0f,
            static_cast<float>(player_sprite.width),
            static_cast<float>(player_sprite.height)
        };
    }

    constexpr auto GetRocketBottomRenArgs() const
    {
        return Args::Arguments_t {
            Args::For_v<RenderComponent_t>,
            mResMan.GetTextureRocketBottom(),
            GetPlayerCropRect(),
        };
    }

    constexpr auto GetRocketFrontRenArgs() const
    {
        return Args::Arguments_t {
            Args::For_v<RenderComponent_t>,
            mResMan.GetTextureRocketFront(),
            GetPlayerCropRect(),
        };
    }

    constexpr auto GetRocketAnimArgs() const
    {
        return Args::Arguments_t {
            Args::For_v<AnimationComponent_t>,
            7,
            0.1f,
        };
    }

    constexpr auto GetBulletSpawnerSpawnArgs() const
    {
        return Args::Arguments_t {
            Args::For_v<SpawnComponent_t>,
            [&](Vector2& pos, float rot) {
                CreateBullet(pos, rot);
            },
            0.1f,
            0.15f,
            30u,
        };       
    }

    constexpr auto CreateAsteroidSpawners(int screen_width, int screen_height)
    {
        float sw { static_cast<float>(screen_width) };
        float sh { static_cast<float>(screen_height) };
        const Args::Arguments_t left_asteroid_spawner_phy_args {
            Args::For_v<PhysicsComponent_t>,
            Vector2{ 0.0f, 0.0f },
            Vector2{ 0.0f, 150.0f },
            Vector2{  },
            Vector2{  },
            0.0f,
            90.0f
        };
        const Args::Arguments_t right_asteroid_spawner_phy_args {
            Args::For_v<PhysicsComponent_t>,
            Vector2{ sw, sh },
            Vector2{ 0.0f, -150.0f },
            Vector2{  },
            Vector2{  },
            0.0f,
            -90.0f
        };
        const Args::Arguments_t top_asteroid_spawner_phy_args {
            Args::For_v<PhysicsComponent_t>,
            Vector2{ sw, 0.0f },
            Vector2{ -150.0f, 0.0f },
            Vector2{  },
            Vector2{  },
            0.0f,
            180.0f,
        };
        const Args::Arguments_t bottom_asteroid_spawner_phy_args {
            Args::For_v<PhysicsComponent_t>,
            Vector2{ 0.0f, sh },
            Vector2{ 150.0f, 0.0f },
            Vector2{  },
            Vector2{  },
            0.0f,
            0.0f
        };
        const Args::Arguments_t asteroid_spawner_spawn_args {
            Args::For_v<SpawnComponent_t>,
            [&](Vector2 pos, float rot) {
                CreateAsteroid(pos, rot);
            },
            7.0f,
            7.1f,
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

    constexpr auto CreateAsteroid(Vector2 pos, float rot)
    {
        auto texture { GetRandomValue(0, 1) ? mResMan.GetTextureAsteroid() : mResMan.GetTextureAsteroidSmall() };
        rot += GetRandomValue(-30, 30);
        auto size { (float)GetRandomValue(1, 3) };
        auto vel { (float)GetRandomValue(60, 150) };
        const Args::Arguments_t ren_args {
            Args::For_v<RenderComponent_t>,
            texture,
            Rectangle {
                0.0f, 0.0f,
                texture.width / 16.0f,
                texture.height * 1.0f
            }
        };
        const Args::Arguments_t anim_args {
            Args::For_v<AnimationComponent_t>,
            16,
            0.1f
        };
        const Args::Arguments_t phy_args {
            Args::For_v<PhysicsComponent_t>,
            pos,
            Vector2 { -vel * -std::sin(rot * DEG2RAD), -vel * std::cos(rot * DEG2RAD) },
            Vector2 {  },
            Vector2 { texture.width * size / 32.0f, texture.height * size / 2.0f },
            0.0f,
            rot,
            0.0f,
            0.0f,
            size
        };
        const Args::Arguments_t coll_args {
            Args::For_v<ColliderComponent_t>,
            texture.width / 16,
        };
        mECSMan.template CreateEntity<Asteroids_t>(ren_args, anim_args, phy_args, coll_args);
    }

    constexpr auto CreateAsteroidExplosion(Vector2 pos, float rot, float scale) {
        const Texture2D sprite { mResMan.GetTextureAstroidExplosion() };
        const Args::Arguments_t ren_args {
            Args::For_v<RenderComponent_t>,
            sprite,
            Rectangle {
                0.0f, 0.0f, 128.0f, 128.0f
            }
        };
        const Args::Arguments_t anim_args {
            Args::For_v<AnimationComponent_t>,
            48,
            0.05f,
        };
        const Args::Arguments_t tim_args {
            Args::For_v<TimerComponent_t>,
            48 * 0.05f,
        };
        const Args::Arguments_t phy_args {
            Args::For_v<PhysicsComponent_t>,
            pos,
            Vector2 {  },
            Vector2 {  },
            Vector2 { (sprite.width / (48 * 2.0f)) * scale * 0.5f, (sprite.height / 2.0f) * scale * 0.5f },
            1.0f,
            rot,
            0.0f,
            0.0f,
            scale * 0.5f
        };
        mECSMan.template CreateEntity<Explosion_t>(ren_args, anim_args, tim_args, phy_args);
    }

    constexpr auto CreateLaserExplosion(Vector2 pos, float rot)
    {
        const Texture2D sprite { mResMan.GetTextureLaserExplosion() };
        const Args::Arguments_t ren_args {
            Args::For_v<RenderComponent_t>,
            sprite,
            Rectangle {
                0.0f, 0.0f, 50.0f, 50.0f
            }
        };
        const Args::Arguments_t anim_args {
            Args::For_v<AnimationComponent_t>,
            20,
            0.01f,
        };
        const Args::Arguments_t tim_args {
            Args::For_v<TimerComponent_t>,
            20 * 0.01f,
        };
        const Args::Arguments_t phy_args {
            Args::For_v<PhysicsComponent_t>,
            pos,
            Vector2 {  },
            Vector2 {  },
            Vector2 { sprite.width * 0.3f / (20 * 2.0f), sprite.height * 0.3f / 2.0f },
            1.0f,
            rot,
            0.0f,
            0.0f,
            0.3f
        };
        mECSMan.template CreateEntity<Explosion_t>(ren_args, anim_args, tim_args, phy_args);
    }

    constexpr auto CreateBullet(Vector2 pos, float rot)
    {
        PlaySoundMulti(mResMan.GetLaserSound());
        const auto texture { mResMan.GetTextureBulletFire() };
        const Args::Arguments_t ren_args {
            Args::For_v<RenderComponent_t>,
            texture,
            Rectangle{
                0.0f, 0.0f, 
                texture.width / 16.0f,
                static_cast<float>(texture.height)
            }
        };
        const Args::Arguments_t anim_args {
            Args::For_v<AnimationComponent_t>,
            16,
            0.1f,
        };
        rot += GetRandomValue(-2, 2);
        const Args::Arguments_t phy_args {
            Args::For_v<PhysicsComponent_t>,
            pos,
            Vector2 { -400.0f * -std::sin(rot * DEG2RAD), -400.0f * std::cos(rot * DEG2RAD) },
            Vector2 {  },
            Vector2 { texture.width / 32.0f, texture.height * 1.0f },
            0.0f,
            rot
        };
        const Args::Arguments_t coll_args {
            Args::For_v<ColliderComponent_t>,
            texture.width / 16,
        };
        mECSMan.template CreateEntity<Bullet_t>(ren_args, anim_args, phy_args, coll_args);
    }

    constexpr auto CreatePlayer(int screen_width, int screen_height) -> void
    {
        const auto player_sprite { mResMan.GetTexturePlayer() };
        const Rectangle crop_rec { GetPlayerCropRect() };
        const Vector2 player_pos { screen_width / 2.0f, screen_height / 2.0f };
        const Args::Arguments_t player_ren_args {
            Args::For_v<RenderComponent_t>,
            player_sprite,
            crop_rec,
        };
        const Args::Arguments_t player_phy_args {
            Args::For_v<PhysicsComponent_t>,
            player_pos,
            Vector2 {  },
            Vector2 {  },
            Vector2 { 
                player_sprite.width  * PLAYER_SCALE / 2.0f,
                player_sprite.height * PLAYER_SCALE / 2.0f },
            PLAYER_FRICTION,
            0.0f,
            0.0f,
            0.0f,
            PLAYER_SCALE
        };
        const Args::Arguments_t player_coll_args {
            Args::For_v<ColliderComponent_t>,
            player_sprite.width
        };
        const Args::Arguments_t rocket_bottom_inp_args {
            Args::For_v<InputEnablerComponent_t>,
            KEY_W,
            InputEnablerComponent_t::ROCKET_BOTTOM
        };
        const Args::Arguments_t rocket_front_inp_args {
            Args::For_v<InputEnablerComponent_t>,
            KEY_S,
            InputEnablerComponent_t::ROCKET_FRONT
        };
        const Args::Arguments_t bullet_spawner_inp_args {
            Args::For_v<InputEnablerComponent_t>,
            KEY_SPACE,
            InputEnablerComponent_t::BULLET_SPAWNER
        };
        const Args::Arguments_t bullet_spawner_chrg_args {
            Args::For_v<ChargeComponent_t>,
            CHARGE_INTERVAL,
            0.0f,
            MAX_BULLETS,
            MAX_BULLETS
        };
        mECSMan.template CreateEntity<Player_t>(player_ren_args,
                                                player_phy_args,
                                                player_coll_args);
        mECSMan.template CreateEntity<BulletSpawnerOff_t>(bullet_spawner_inp_args,
                                                          player_phy_args,
                                                          bullet_spawner_chrg_args);
        mECSMan.template CreateEntity<RocketOff_t>(rocket_bottom_inp_args,
                                                   player_phy_args);
        mECSMan.template CreateEntity<RocketOff_t>(rocket_front_inp_args,
                                                   player_phy_args);
    }
private:
    ECSMan_t& mECSMan {  };
    const ResMan_t& mResMan {  };

    constexpr static inline float PLAYER_SCALE { 0.58f };
    constexpr static inline float PLAYER_FRICTION { 0.8f };

    constexpr static inline unsigned MAX_BULLETS { 30u };
    constexpr static inline float CHARGE_INTERVAL { 0.5f };
};
