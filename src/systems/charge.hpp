#pragma once

#include <types.hpp>

struct ChargeSystem_t
{
    constexpr explicit ChargeSystem_t() = default;

    template<class ECSMan_t>
    constexpr static void update(ECSMan_t& ecs_man, float elapsed_time)
    {
        ecs_man.template ForEachEntity<Chargeable_t>([&](auto& chrg, auto&& ent){
                chrg.elapsed += elapsed_time;
                if (chrg.elapsed > chrg.charge_interval && chrg.charged < chrg.max_charge) {
                    chrg.elapsed = 0;
                    ++chrg.charged;
                }
                if constexpr (ECS::IsInstanceOf_v<Spawner_t, decltype(ent)>) {
                    auto& spwn { ecs_man.template GetComponent<SpawnComponent_t>(ent) };
                    if ((chrg.elapsed == 0 || chrg.elapsed > chrg.charge_interval) && spwn.spawned + 1 < chrg.max_charge) {
                        ++spwn.spawned;
                        --chrg.charged;
                    }
                }
        });
    }
};
