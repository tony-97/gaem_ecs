#pragma once

#include <types.hpp>

struct ColliderSystem_t
{
    template<class ECSMan_t>
    void update(ECSMan_t& ecs_man, int screen_width, int screen_height)
    {
        ecs_man.template ForEachEntity<Renderable_t>([&](auto& ren, auto& phy, auto&&) {
                
        });
    }
};
