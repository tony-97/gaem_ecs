#pragma once

#include <types.hpp>

struct HealthSystem_T
{
    explicit HealthSystem_T() = default;

    template<class ECSMan_t> constexpr auto
    update(ECSMan_t& ecs_man) const -> void
    {
        ecs_man.template ForEachEntity<Alive_t>([&](auto& hel, auto&& ent){
                    if (hel.health < 1) {
                        ecs_man.Destroy(ent);
                    }
                });
    }
};
