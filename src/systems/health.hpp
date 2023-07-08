#pragma once

#include <types.hpp>

struct HealthSystem_t
{
    constexpr explicit HealthSystem_t() = default;

    template<class ECSMan_t, class GameCtx_t>
    constexpr static void update(ECSMan_t& ecs_man, GameCtx_t& gctx)
    {
        ecs_man.template ASYNCForEach<ParentHealth_t>([&](const auto& hel, const auto& hels) {
                    if (hel.health < 1) {
                        for (auto par_hel : hels.leafs) {
                            auto& leaf_hel { ecs_man.template GetComponent<HealthComponent_t>(par_hel) };
                            leaf_hel.health = 0;
                        }
                    }
                });
        ecs_man.template ForEach<Alive_t>([&](const auto& hel, auto ent){
                    if (hel.health < 1) {
                        gctx.Destroy(ent);
                    }
                });
    }
};
