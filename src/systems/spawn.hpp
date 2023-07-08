#pragma once

#include <types.hpp>
#include <utility.hpp>

struct SpawnSystem_t
{
    constexpr explicit SpawnSystem_t() = default;

    template<class ECSMan_t>
    constexpr static void update(ECSMan_t& ecs_man, float elapsed_time)
    {
        ecs_man.template ForEach<Spawner_t>([&](auto& spawn, const auto& phy){
                    spawn.elapsed += elapsed_time;
                    if (spawn.to_spawn > 0 && spawn.elapsed > spawn.spawn_interval) {
                        spawn.fn_spawn(phy.position, phy.rotation);
                        spawn.elapsed = 0.0f;
                        --spawn.to_spawn;
                    }
                });
    }
};
