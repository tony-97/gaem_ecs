#pragma once

#include <types.hpp>

#include <iostream>

struct EngineEffectSystem_t
{
    constexpr explicit EngineEffectSystem_t() = default;

    template<class ECSMan_t>
    constexpr static void update(ECSMan_t& ecs_man)
    {
        ecs_man.template ASYNCForEach<RocketOn_t>([&](const auto e) {
                    auto& phy { ecs_man.template GetComponent<PhysicsComponent_t>(e) };
                    auto& ren { ecs_man.template GetComponent<RenderComponent_t>(e) };
                    auto& eng { ecs_man.template GetComponent<EngineEffectComponent_t>(e) };
                    if (eng.idle_velocity < phy.velocity.Modulus()) {
                        ren.crop_rec.y = ren.sprite.height * 0.5f;
                    } else {
                        ren.crop_rec.y = 0.0f;
                    }
                });
    }
};
