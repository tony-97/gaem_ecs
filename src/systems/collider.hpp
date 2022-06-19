#pragma once

#include <types.hpp>
#include <utility.hpp>

struct ColliderSystem_t
{
    template<class PhyCmpt_t, class CollCmp_t>
    constexpr static bool collide(CollCmp_t coll1, const PhyCmpt_t& phy1, CollCmp_t coll2, const PhyCmpt_t& phy2)
    {
        auto sqdistp1p2 { (phy1.pos.x - phy2.pos.x) * (phy1.pos.x - phy2.pos.x) + (phy1.pos.y - phy2.pos.y) * (phy1.pos.y - phy2.pos.y) };
        auto sqdistr1r2 { phy1.size * phy2.size * coll1.size * coll2.size };
        return sqdistr1r2 > sqdistp1p2;
    }

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
                if (v1.y > screen_height && v2.y > screen_height && v3.y > screen_height && v4.y > screen_height) {
                    phy.pos.y -= screen_height;
                } else if (v1.y < 0 || v2.y < 0 || v3.y < 0 || v4.y < 0) {
                    phy.pos.y += screen_height;
                }
                if (v1.x > screen_width && v2.x > screen_width && v3.x > screen_width && v4.x > screen_width) {
                    phy.pos.x -= screen_width;
                } else if (v1.x < 0 || v2.x < 0 || v3.x < 0 || v4.x < 0) {
                    phy.pos.x += screen_width;
                }
        });
        ecs_man.template ForEachEntity<AsteroidsSpawner_t>([&](auto&,auto& phy, auto&& ent) {
                    if constexpr (not ECS::IsInstanceOf_v<AsteroidsSpawner_t, decltype(ent)>) {
                        return;
                    }
                    if (phy.pos.x < 0) {
                        phy.pos.x += screen_width;
                    } else if (phy.pos.x > screen_width) {
                        phy.pos.x -= screen_width;
                    }
                    if (phy.pos.y < 0) {
                        phy.pos.y += screen_height;
                    } else if (phy.pos.y > screen_height) {
                        phy.pos.y -= screen_height;
                    }
                });
        ecs_man.template ForEachEntity<Collidable_t>([&](auto& col_ast, auto& phy_ast, auto&& ast) {
            if constexpr (ECS::IsInstanceOf_v<Asteroids_t, decltype(ast)>) {
                ecs_man.template ForEachEntity<Collidable_t>([&](auto& col_ast, auto& phy_pla, auto&& pla) {
                    if constexpr (ECS::IsInstanceOf_v<Player_t, decltype(pla)>) {
                        
                    }
                });
                ecs_man.template ForEachEntity<Collidable_t>([&](auto& col_bull, auto& phy_bull, auto&& bull) {
                    if constexpr (ECS::IsInstanceOf_v<Bullet_t, decltype(bull)>) {
                        if (collide(col_ast, phy_ast, col_bull, phy_bull)) {
                            auto& hel1 { ecs_man.template GetComponent<HealthComponent_t>(bull) };
                            auto& hel2 { ecs_man.template GetComponent<HealthComponent_t>(ast) };
                            hel2.health = hel1.health = 0;
                        }
                    }
                });
            }
        });
    }
};
