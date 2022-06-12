#pragma once

#include <types.hpp>
#include <utility.hpp>

struct SpawnSystem_t
{
    template<class ECSMan_t, class GFact_t> constexpr auto
    update(ECSMan_t& ecs_man, GFact_t& gfact, float elapsed_time) -> void
    {
        ecs_man.template ForEachEntity<Spawner_t>([&](auto& spawn, const auto& phy, auto&&){
                    spawn.elapsed += elapsed_time;
                    if (spawn.spawned > 0 && spawn.elapsed >= spawn.spawn_interval) {
                        const Vector2 orig { phy.pos.x + phy.orig.x, phy.pos.y + phy.orig.y };
                        const auto polar_vec { to_polar_vector(phy.pos, orig, phy.ang) };
                        const auto dest_vec { to_rect_vector(polar_vec, orig) };
                        spawn.fn_spawn(dest_vec.x, dest_vec.y, phy.ang, reinterpret_cast<void*>(&gfact));
                        spawn.elapsed = 0.0f;
                        --spawn.spawned;
                    }
                });
    }
};
