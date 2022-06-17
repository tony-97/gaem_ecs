#pragma once

#include <types.hpp>

struct BulletSystem_t
{
    constexpr explicit BulletSystem_t() = default;

    template<class ECSMan_t>
    constexpr static void update(ECSMan_t& ecs_man, float elapsed_time)
    {
        ecs_man.template ForEachEntity<Bulletable_t>([&](auto& bull, auto& hel, auto&&) {
                    if (bull.time_alive < 0) {
                        hel.health = 0;
                    }
                    bull.time_alive -= elapsed_time;
                });
    }
};
