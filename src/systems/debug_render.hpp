#pragma once

#include <cmath>

#include <raylib.h>

#include <types.hpp>

struct DebugRenderSystem_t
{
    constexpr explicit DebugRenderSystem_t() = default;

    template<class ECSMan_t> constexpr auto
    update(const ECSMan_t& ecs_man) -> void
    {
        ecs_man.template ForEach<Renderable_t>(
                [&](auto& ren, auto& phy, auto ent)
                {
                    const auto polar_v1 { to_polar_vector({ 0.0f, 0.0f }, phy.origin, phy.rotation) };
                    const auto polar_v2 { to_polar_vector({ ren.crop_rec.width * ren.scale, 0.0f }, phy.origin, phy.rotation) };
                    const auto polar_v3 { to_polar_vector({ 0.0f, ren.crop_rec.height * ren.scale }, phy.origin, phy.rotation) };
                    const auto polar_v4 { to_polar_vector({ ren.crop_rec.width * ren.scale, ren.crop_rec.height * ren.scale }, phy.origin, phy.rotation) };
                    const auto v1 { to_rect_vector(polar_v1, phy.position) };
                    const auto v2 { to_rect_vector(polar_v2, phy.position) };
                    const auto v3 { to_rect_vector(polar_v3, phy.position) };
                    const auto v4 { to_rect_vector(polar_v4, phy.position) };
                    DrawLine(v1.x, v1.y, v2.x, v2.y, GREEN);
                    DrawLine(v2.x, v2.y, v4.x, v4.y, GREEN);
                    DrawLine(v3.x, v3.y, v4.x, v4.y, GREEN);
                    DrawLine(v3.x, v3.y, v1.x, v1.y, GREEN);
                    DrawCircle(phy.position.x, phy.position.y, 2, RED);
                    ecs_man.template Match<Player_t>(ent, [&](auto) {
                                DrawText(TextFormat("Velocity: %f", std::hypot(phy.velocity.x, phy.velocity.y)), phy.position.x, phy.position.y + 15, 10, BLUE);
                            });
                    ecs_man.template Match<Collidable_t>(ent, [&](auto e) {
                                auto& col { ecs_man.template GetComponent<ColliderComponent_t>(e) };
                                DrawCircleLines(phy.position.x, phy.position.y, col.radius, RED);
                            });
                    ecs_man.template Match<Alive_t>(ent, [&](auto& hel, auto) {
                                DrawText(TextFormat("Lives: %d", hel.health), phy.position.x, phy.position.y, 10, BLUE);
                            });
                });
    }
};
