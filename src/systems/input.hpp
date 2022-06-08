#pragma once

#include <raylib.h>

#include <types.hpp>

#include <cmath>

#include <components/physics.hpp>
#include <components/render.hpp>
#include <components/player_input.hpp>
#include <components/rocket_input.hpp>
#include <interface.hpp>
#include <utility>

struct InputSystem_t
{
    constexpr explicit InputSystem_t() = default;

    template<class ECSMan_t, class GFact_t>
    constexpr void update(ECSMan_t& ecs_man, const GFact_t& gfact)
    {
        ecs_man.template ForEachEntity<PlayerInput_t>([&](const auto& inp, auto& phy, auto&&) {
                    phy.acel.x = 0.0f;
                    phy.acel.y = 0.0f;
                    if (IsKeyDown(inp.left))  phy.ang  += -150.0f * GetFrameTime();
                    if (IsKeyDown(inp.right)) phy.ang  += +150.0f * GetFrameTime();
                    if (IsKeyDown(inp.up)) {
                        phy.acel.y += -100.0f * cos(phy.ang * DEG2RAD);
                        phy.acel.x += -100.0f * -sin(phy.ang * DEG2RAD);
                    }
                    if (IsKeyDown(inp.down)) {
                        phy.acel.y += +100.0f * cos(phy.ang * DEG2RAD);
                        phy.acel.x += +100.0f * -sin(phy.ang * DEG2RAD);
                    }
                    //phy.acel.y = -phy.vel.y * 0.8;
                    //phy.acel.x = -phy.vel.x * 0.8;
                });
        ecs_man.template ForEachEntity<RocketInput_t>([&](const auto& rock_inp, auto&& ent) {
                    if (IsKeyDown(rock_inp.enable)) {
                        if constexpr (ECS::IsInstanceOf_v<RocketDisable_t, decltype(ent)>) {
                            switch (rock_inp.type) {
                            case RocketInputComponent_t::ROCKET_FRONT:
                                ecs_man.template TransformTo<Rocket_t>(ent,
                                                                       gfact.GetRocketFrontRenArgs(),
                                                                       gfact.GetRocketAnimArgs());
                                break;
                            case RocketInputComponent_t::ROCKET_BOTTOM:
                                ecs_man.template TransformTo<Rocket_t>(ent,
                                                                       gfact.GetRocketBottomRenArgs(),
                                                                       gfact.GetRocketAnimArgs());
                                break;
                            }
                            
                        }
                    } else {
                        if constexpr (ECS::IsInstanceOf_v<Rocket_t, decltype(ent)>) {
                            ecs_man.template TransformTo<RocketDisable_t>(ent);
                        }
                    }
                });
    }
};
