#pragma once

#include "types.hpp"

struct PhysicsSystem_t
{
    constexpr explicit PhysicsSystem_t() = default;

    template<class ECSMan_t>
    constexpr static void update(ECSMan_t& ecs_man, float elapsed_time)
    {
        ecs_man.template ForEachEntity<Movable_t>(
                [&](auto& phy, auto&&) {
                    phy.w += phy.a * elapsed_time;
                    phy.vel.x += phy.acc.x * elapsed_time;
                    phy.vel.y += phy.acc.y * elapsed_time;

                    phy.ang += phy.w * elapsed_time;
                    phy.pos.x += phy.vel.x * elapsed_time;
                    phy.pos.y += phy.vel.y * elapsed_time;

                    phy.w -= phy.w * phy.friction * elapsed_time;
                    phy.vel.x -= phy.vel.x * phy.friction * elapsed_time;
                    phy.vel.y -= phy.vel.y * phy.friction * elapsed_time;
                });
    }
};
