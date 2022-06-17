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

    template<class ECSMan_t, class GameCtx_t>
    constexpr static void update(ECSMan_t& ecs_man, GameCtx_t& gctx)
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
        ecs_man.template ForEachEntity<InputEnabler_t>([&](const auto& inp, auto&& ent) {
                    if (IsKeyDown(inp.enable)) {
                        gctx.Enable(ent, inp.type);
                    } else {
                        gctx.Disable(ent, inp.type);
                    }
                });
    }
};
