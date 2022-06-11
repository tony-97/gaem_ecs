#pragma once

#include <ecs_manager.hpp>
#include <interface.hpp>

struct RenderComponent_t;
struct PhysicsComponent_t;
struct PlayerInputComponent_t;
struct RocketInputComponent_t;
struct AnimationComponent_t;
struct ColliderComponent_t;
struct HealthComponent_t;

using Renderable_t     = ECS::Base_t<RenderComponent_t, PhysicsComponent_t>;
using Movable_t        = ECS::Base_t<PhysicsComponent_t>;
using Animable_t       = ECS::Base_t<AnimationComponent_t, RenderComponent_t>;
using PlayerInput_t    = ECS::Base_t<PlayerInputComponent_t, PhysicsComponent_t>;
using RocketInput_t    = ECS::Base_t<RocketInputComponent_t>;
using Collidable_t     = ECS::Base_t<ColliderComponent_t, PhysicsComponent_t>;
using Alive_t          = ECS::Base_t<HealthComponent_t>;
using BasicCharacter_t = ECS::Derived_t<Animable_t, Movable_t, Renderable_t, Collidable_t, HealthComponent_t>;
using Rocket_t         = ECS::Derived_t<Animable_t, Movable_t, RocketInput_t, PlayerInput_t, Collidable_t, HealthComponent_t>;
using RocketDisable_t  = ECS::Derived_t<Movable_t, RocketInput_t, PlayerInput_t, Collidable_t, HealthComponent_t>;
using Player_t         = ECS::Derived_t<Renderable_t, Movable_t, PlayerInput_t, Collidable_t, HealthComponent_t>;

using ECSMan_t = ECS::ECSManager_t<Renderable_t,
                                   Animable_t,
                                   Movable_t,
                                   Alive_t,
                                   PlayerInput_t,
                                   RocketInput_t,
                                   BasicCharacter_t,
                                   RocketDisable_t,
                                   Rocket_t,
                                   Player_t>;
