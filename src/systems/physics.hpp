#pragma once

#include <types.hpp>

struct PhysicsSystem_t
{
    constexpr explicit PhysicsSystem_t() = default;

    template<class ECSMan_t>
    constexpr static void update(ECSMan_t& ecs_man, float elapsed_time)
    {
        ecs_man.template ASYNCForEach<Movable_t>(
                [&](auto& phy, auto ent) {
                    phy.velocity += phy.acceleration * elapsed_time;
                    phy.position += phy.velocity * elapsed_time;
                    phy.velocity -= phy.velocity * phy.friction * elapsed_time;
                    ecs_man.template Match<ParentPhysics_t>(ent, [&](auto& parent_phy, auto) {
                                for (auto leaf : parent_phy.leafs) {
                                    auto& phy_leaf { ecs_man.template GetComponent<PhysicsComponent_t>(leaf) };
                                    phy_leaf.position = phy.position;
                                    phy_leaf.velocity = phy.velocity;
                                    phy_leaf.rotation = phy.rotation;
                                }
                            });
                });
    }
};
