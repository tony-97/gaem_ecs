#pragma once

#include <types.hpp>
#include <raylib.h>
#include <cmath>
#include <utility.hpp>
#include <iostream>
struct ColliderSystem_t
{
    template<class ECSMan_t>
    void update(ECSMan_t& ecs_man, int screen_width, int screen_height)
    {
        ecs_man.template ForEachEntity<Renderable_t>([&](const auto& ren, auto& phy, auto&&) {
                const auto polar_v1 { to_polar_vector({ 0.0f, 0.0f }, phy.orig, phy.ang) };
                const auto polar_v2 { to_polar_vector({ ren.crop_rec.width, 0.0f }, phy.orig, phy.ang) };
                const auto polar_v3 { to_polar_vector({ 0.0f, ren.crop_rec.height }, phy.orig, phy.ang) };
                const auto polar_v4 { to_polar_vector({ ren.crop_rec.width, ren.crop_rec.height }, phy.orig, phy.ang) };
                const auto v1 { to_rect_vector(polar_v1, phy.pos) };
                const auto v2 { to_rect_vector(polar_v2, phy.pos) };
                const auto v3 { to_rect_vector(polar_v3, phy.pos) };
                const auto v4 { to_rect_vector(polar_v4, phy.pos) };
                if (v1.y > screen_height || v2.y > screen_height || v3.y > screen_height || v4.y > screen_height) {
                    phy.pos.y -= screen_height;
                } else if (v1.y < 0 || v2.y < 0 || v3.y < 0 || v4.y < 0) {
                    phy.pos.y += screen_height;
                }
                if (v1.x > screen_width || v2.x > screen_width || v3.x > screen_width || v4.x > screen_width) {
                    phy.pos.x -= screen_width;
                } else if (v1.x < 0 || v2.x < 0 || v3.x < 0 || v4.x < 0) {
                    phy.pos.x += screen_width;
                }
        });
    }
};
