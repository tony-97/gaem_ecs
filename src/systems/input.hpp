#pragma once

#include <raylib.h>

#include <cmath>

#include <types.hpp>

struct InputSystem_t
{
    constexpr explicit InputSystem_t() = default;

    template<class ECSMan_t, class GameCtx_t>
    constexpr static void update(ECSMan_t& ecs_man, GameCtx_t& gctx)
    {
        ecs_man.template ASYNCForEach<PlayerInput_t>([&](const auto& inp, auto& phy) {
                    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                        const auto mouse { GetMousePosition() };
                        phy.rotation = 90 + std::atan2(phy.position.y - mouse.y, phy.position.x - mouse.x) * RAD2DEG;
                    }
                    phy.acceleration = 0.0f;
                    if (IsKeyDown(inp.up)) {
                        phy.acceleration.y = -400.0f * std::cos(phy.rotation * DEG2RAD);
                        phy.acceleration.x = -400.0f * -std::sin(phy.rotation * DEG2RAD);
                    }
                    if (IsKeyDown(inp.down)) {
                        phy.acceleration.y = +400.0f * std::cos(phy.rotation * DEG2RAD);
                        phy.acceleration.x = +400.0f * -std::sin(phy.rotation * DEG2RAD);
                    }
                });
        ecs_man.template ForEach<InputEnabler_t>([&](const auto& inp, auto ent) {
                    if (IsKeyDown(inp.enable)) {
                        gctx.Enable(ent, inp.type);
                    } else {
                        gctx.Disable(ent, inp.type);
                    }
                });
    }
};
