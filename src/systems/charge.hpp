#pragma once

#include <types.hpp>

struct ChargeSystem_t
{
    constexpr explicit ChargeSystem_t() = default;

    template<class ECSMan_t>
    constexpr static void update(ECSMan_t& ecs_man, float elapsed_time)
    {
        ecs_man.template ASYNCForEach<Chargeable_t>([&](auto& chrg, auto ent){
                chrg.elapsed += elapsed_time;
                if (chrg.elapsed > chrg.charge_interval && chrg.charged < chrg.max_charge) {
                    chrg.elapsed = 0;
                    ++chrg.charged;
                }
                ecs_man.template Match<Spawner_t>(ent, [&](auto e) {
                        auto& spwn { ecs_man.template GetComponent<SpawnComponent_t>(e) };
                        if (chrg.elapsed == 0 && spwn.to_spawn + 1 < chrg.max_charge) {
                            ++spwn.to_spawn;
                            --chrg.charged;
                        }    
                });
        });
    }
};
