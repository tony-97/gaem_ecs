#pragma once

#include <raylib.h>

#include <types.hpp>

#include <cmath>

#include <components/physics.hpp>
#include <components/render.hpp>
#include <components/player_input.hpp>
#include <components/input_enabler.hpp>
#include <interface.hpp>

struct InputSystem_t
{
    constexpr explicit InputSystem_t() = default;

    template<class ECSMan_t, class GFact_t>
    constexpr void update(ECSMan_t& ecs_man, GFact_t& gfact)
    {
        ecs_man.template ForEachEntity<PlayerInput_t>([&](const auto& inp, auto& phy, auto&&) {
                    phy.acc.x = 0.0f;
                    phy.acc.y = 0.0f;
                    phy.a = 0.0f;
                    if (IsKeyDown(inp.left))  phy.a += -400.0f;
                    if (IsKeyDown(inp.right)) phy.a += +400.0f;
                    if (IsKeyDown(inp.up)) {
                        phy.acc.y += -100.0f * std::cos(phy.ang * DEG2RAD);
                        phy.acc.x += -100.0f * -std::sin(phy.ang * DEG2RAD);
                    }
                    if (IsKeyDown(inp.down)) {
                        phy.acc.y += +100.0f * std::cos(phy.ang * DEG2RAD);
                        phy.acc.x += +100.0f * -std::sin(phy.ang * DEG2RAD);
                    }
                });
        ecs_man.template ForEachEntity<InputEnabler_t>([&](const auto& rock_inp, auto&& ent) {
                    if (IsKeyDown(rock_inp.enable)) {
                        gfact.Enable(ent, rock_inp.type);
                    } else {
                        gfact.Disable(ent, rock_inp.type);
                    }
                });
    }
};
