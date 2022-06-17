#pragma once

#include <types.hpp>
#include <utility.hpp>

struct SpawnSystem_t
{
    constexpr explicit SpawnSystem_t() = default;

    template<class ECSMan_t>
    constexpr static void update(ECSMan_t& ecs_man, float elapsed_time)
    {
        ecs_man.template ForEachEntity<Spawner_t>([&](auto& spawn, const auto& phy, auto&&){
                    spawn.elapsed += elapsed_time;
                    if (spawn.spawned > 0 && spawn.elapsed > spawn.spawn_interval) {
                        spawn.fn_spawn(phy.pos, phy.ang);
                        spawn.elapsed = 0.0f;
                        --spawn.spawned;
                    }
                });
    }
};
