#pragma once

#include <types.hpp>

struct HealthSystem_t
{
    constexpr explicit HealthSystem_t() = default;

    template<class ECSMan_t, class GameCtx_t>
    constexpr static void update(const ECSMan_t& ecs_man, GameCtx_t& gctx)
    {
        ecs_man.template ForEachEntity<Alive_t>([&](auto& hel, auto&& ent){
                    if (hel.health < 1) {
                        gctx.Destroy(ent);
                    }
                });
    }
};
