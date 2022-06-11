#pragma once

#include <arguments.hpp>

#include "types.hpp"

#include <components/render.hpp>
#include <components/physics.hpp>
#include <components/animation.hpp>
#include <components/player_input.hpp>
#include <components/rocket_input.hpp>
#include <components/health.hpp>
#include <components/bullet.hpp>
#include <components/collider.hpp>

#include <cmath>

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
            0,
            0.1f,
        };
    }

    constexpr auto CreateFireBullet(Vector2 pos, float rot) const
    {
        //Vector2 bullet_pos { -23 + pos.x, 5 + pos.y };
        auto texture { mResMan.GetTextureBulletFire() };
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
            0,
            0.1f,
        };
        const Args::Arguments_t phy_args {
            Args::For_v<PhysicsComponent_t>,
            pos,
            Vector2 { -400.0f * -std::sin(rot * DEG2RAD), -400.0f * std::cos(rot * DEG2RAD) },
            Vector2 {  },
            Vector2 { texture.width / 32.0f, texture.height / 2.0f },
            0.0f,
            rot
        };
        mECSMan.template CreateEntity<Bullet_t>(ren_args, anim_args, phy_args);
    }

    constexpr auto CreatePlayer(int screen_width, int screen_height) -> void
    {
        const auto player_sprite { mResMan.GetTexturePlayer() };
        const Rectangle crop_rec { GetPlayerCropRect() };
        const Args::Arguments_t player_ren_args {
            Args::For_v<RenderComponent_t>,
            player_sprite,
            crop_rec,
        };
        const Args::Arguments_t rocket_bottom_inp_args {
            Args::For_v<RocketInputComponent_t>,
            KEY_W,
            RocketInputComponent_t::ROCKET_BOTTOM
        };
        const Args::Arguments_t rocket_front_inp_args {
            Args::For_v<RocketInputComponent_t>,
            KEY_S,
            RocketInputComponent_t::ROCKET_FRONT
        };
        const Args::Arguments_t player_phy_args {
            Args::For_v<PhysicsComponent_t>,
            Vector2 { screen_width / 2.0f,
                      screen_height / 2.0f },
            Vector2 {  },
            Vector2 {  },
            Vector2 { player_sprite.width / 2.0f, player_sprite.height / 2.0f },
            0.8f
        };

        mECSMan.template CreateEntity<Player_t>(player_ren_args,
                                                player_phy_args);
        mECSMan.template CreateEntity<RocketDisable_t>(rocket_bottom_inp_args,
                                                       player_phy_args);
        mECSMan.template CreateEntity<RocketDisable_t>(rocket_front_inp_args,
                                                       player_phy_args);
    }
private:
    ECSMan_t& mECSMan {  };
    const ResMan_t& mResMan {  };
};
