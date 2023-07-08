#pragma once

#include <math_vector.hpp>

#include <types.hpp>

#include <utility.hpp>
#include <optional>

struct ColliderSystem_t
{
    constexpr static bool collide(ColliderComponent_t coll1, PhysicsComponent_t phy1,
                                  ColliderComponent_t coll2, PhysicsComponent_t phy2)
    {
        const auto sqdistp1p2 { phy1.position.DistanceSQ(phy2.position) };
        const auto distr1r2 { coll1.radius + coll2.radius };
        const auto sqdistr1r2 { distr1r2 * distr1r2 };
        return sqdistr1r2 > sqdistp1p2;
    }

    constexpr static auto static_collision_response(PhysicsComponent_t phy1, ColliderComponent_t col1,
                                                    PhysicsComponent_t phy2, ColliderComponent_t col2)
    {
        const auto dist { phy1.position.Distance(phy2.position) };
        const auto overlap_dist { col1.radius * col2.radius - dist };
        const auto half_overalp { overlap_dist * 0.5f };

        const auto dir_vec { (phy1.position - phy2.position).Normalize() };
        const auto force { half_overalp * dir_vec };

        return force;
    }

    constexpr static bool check_collision(ECSMan_t& ecs_man, int screen_width, int screen_height, auto e1, auto e2)
    {
        const auto [col1, phy1] { ecs_man.GetComponents<ColliderComponent_t, PhysicsComponent_t>(e1) };
        const auto [col2, phy2] { ecs_man.GetComponents<ColliderComponent_t, PhysicsComponent_t>(e2) };
        auto phy_left    { phy2 };
        phy_left.position.x += screen_width;
        auto phy_up      { phy2 };
        phy_up.position.y += screen_height;
        auto phy_topleft { phy2 };
        phy_topleft.position.x += screen_width;
        phy_topleft.position.y += screen_height;
        auto collided1 { collide(col1, phy1, col2, phy2) };
        auto collided2 { collide(col1, phy1, col2, phy_left) };
        auto collided3 { collide(col1, phy1, col2, phy_up) };
        auto collided4 { collide(col1, phy1, col2, phy_topleft) };
        Vec2f_t force {  };
        if (collided1) {
            force = static_collision_response(phy1, col1, phy2, col2);
        } else
        if (collided2) {
            force = static_collision_response(phy1, col1, phy_left, col2);
        } else
        if (collided3) {
            force = static_collision_response(phy1, col1, phy_up, col2);
        } else
        if (collided4) {
            force = static_collision_response(phy1, col1, phy_topleft, col2);
        }
        if (   collided1
            || collided2
            || collided3
            || collided4) {
            if      (!col1.collided) col1.collided = ecs_man.GetBase<Collidable_t>(e2);
            else if (!col2.collided) col2.collided = ecs_man.GetBase<Collidable_t>(e1);
            phy1.position += force;
            phy2.position -= force;
            
            return true;
        } else {
            auto k_coll1 = ecs_man.GetComponentID<PhysicsComponent_t>(e1);
            auto k_coll2 = ecs_man.GetComponentID<PhysicsComponent_t>(e2);
            auto a = col1.collided ? col1.collided->GetIndex() == k_coll2.GetIndex() : false;
            auto b = col2.collided ? col2.collided->GetIndex() == k_coll1.GetIndex() : false;
            if (a || b) {
                col1.collided = std::nullopt;
                col2.collided = std::nullopt;
            }
            return false;
        }
    }

    void update(ECSMan_t& ecs_man, int screen_width, int screen_height)
    {
        ecs_man.ASYNCForEach<Renderable_t>([&](const auto& ren, auto& phy) {
                const auto polar_v1 { to_polar_vector({ 0.0f, 0.0f }, phy.origin, phy.rotation) };
                const auto polar_v2 { to_polar_vector({ ren.crop_rec.width * ren.scale, 0.0f }, phy.origin, phy.rotation) };
                const auto polar_v3 { to_polar_vector({ 0.0f, ren.crop_rec.height * ren.scale }, phy.origin, phy.rotation) };
                const auto polar_v4 { to_polar_vector({ ren.crop_rec.width * ren.scale, ren.crop_rec.height * ren.scale }, phy.origin, phy.rotation) };
                const auto v1 { to_rect_vector(polar_v1, phy.position) };
                const auto v2 { to_rect_vector(polar_v2, phy.position) };
                const auto v3 { to_rect_vector(polar_v3, phy.position) };
                const auto v4 { to_rect_vector(polar_v4, phy.position) };
                if (v1.y > screen_height && v2.y > screen_height && v3.y > screen_height && v4.y > screen_height) {
                    phy.position.y -= screen_height;
                } else if (v1.y < 0 || v2.y < 0 || v3.y < 0 || v4.y < 0) {
                    phy.position.y += screen_height;
                }
                if (v1.x > screen_width && v2.x > screen_width && v3.x > screen_width && v4.x > screen_width) {
                    phy.position.x -= screen_width;
                } else if (v1.x < 0 || v2.x < 0 || v3.x < 0 || v4.x < 0) {
                    phy.position.x += screen_width;
                }
        });
        ecs_man.template ASYNCForEach<AsteroidsSpawner_t>([&](auto&,auto& phy) {
                    if (phy.position.x < 0) {
                        phy.position.x += screen_width;
                    } else if (phy.position.x > screen_width) {
                        phy.position.x -= screen_width;
                    }
                    if (phy.position.y < 0) {
                        phy.position.y += screen_height;
                    } else if (phy.position.y > screen_height) {
                        phy.position.y -= screen_height;
                    }
                });
        ecs_man.ForEach<Asteroids_t>([&](auto ast) {
                ecs_man.ForEach<Player_t>([&](auto pla) {
                        if (check_collision(ecs_man, screen_width, screen_height, ast, pla)) {
                            auto& pla_hel { ecs_man.template GetComponent<HealthComponent_t>(pla) };
                            auto& ast_hel { ecs_man.template GetComponent<HealthComponent_t>(ast) };
                            auto& col_ast { ecs_man.template GetComponent<ColliderComponent_t>(ast) };
                            ast_hel.health  = 0;
                            pla_hel.health -= col_ast.radius * 34;
                        }
                });
                ecs_man.ForEach<Bullet_t>([&](auto bull) {
                        if (check_collision(ecs_man, screen_width, screen_height, ast, bull)) {
                            auto& bull_hel { ecs_man.template GetComponent<HealthComponent_t>(bull) };
                            auto& ast_hel  { ecs_man.template GetComponent<HealthComponent_t>(ast) };
                            bull_hel.health = 0;
                            ast_hel.health -= 200;
                        }
                });
        });
        ecs_man.ForEach<Collidable_t>([&](auto& resp1, auto& phy1){
                    if (resp1.collided) {
                        auto phy2 { ecs_man.GetComponent<PhysicsComponent_t>(*resp1.collided) };
                        const auto norm = (phy2.position - phy1.position).Normalize();
                        const auto tan = norm.Tangent();
        
                        const auto dpTan1 = phy1.velocity.DotProduct(tan);
                        const auto dpTan2 = phy2.velocity.DotProduct(tan);
        
                        const auto dpNorm1 = phy1.velocity.DotProduct(norm);
                        const auto dpNorm2 = phy2.velocity.DotProduct(norm);
        
                        const float m1 = (dpNorm1 * (phy1.mass - phy2.mass) + 2.0f * phy2.mass * dpNorm2) / (phy1.mass + phy2.mass);
                        const float m2 = (dpNorm2 * (phy2.mass - phy1.mass) + 2.0f * phy1.mass * dpNorm1) / (phy1.mass + phy2.mass);
        
                        phy1.velocity = tan * dpTan1 + norm * m1;
                        phy2.velocity = tan * dpTan2 + norm * m2;
                    }
                });
        ecs_man.ForEach<Collidable_t>([&](auto& col, auto&) {
                    col.collided = std::nullopt;
                });
    }
};
