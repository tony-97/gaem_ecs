#pragma once

#include <ecs_manager.hpp>
#include <interface.hpp>

struct RenderComponent_t;
struct PhysicsComponent_t;
struct PlayerInputComponent_t;
struct InputEnablerComponent_t;
struct AnimationComponent_t;
struct HealthComponent_t;
struct TimerComponent_t;
struct SpawnComponent_t;
struct ChargeComponent_t;
struct ColliderComponent_t;

struct AsteroidsTagComponent_t {  };
struct PlayerTagComponent_t {  };
struct BulletTagComponent_t {  };

using Renderable_t       = ECS::Base_t<RenderComponent_t, PhysicsComponent_t>;
using Movable_t          = ECS::Base_t<PhysicsComponent_t>;
using Animable_t         = ECS::Base_t<AnimationComponent_t, RenderComponent_t>;
using PlayerInput_t      = ECS::Base_t<PlayerInputComponent_t, PhysicsComponent_t>;
using InputEnabler_t     = ECS::Base_t<InputEnablerComponent_t>;
using Alive_t            = ECS::Base_t<HealthComponent_t>;
using Spawner_t          = ECS::Base_t<SpawnComponent_t, PhysicsComponent_t>;
using Timeable_t         = ECS::Base_t<TimerComponent_t, HealthComponent_t>;
using Chargeable_t       = ECS::Base_t<ChargeComponent_t>;
using Collidable_t       = ECS::Base_t<ColliderComponent_t, PhysicsComponent_t>;

using AsteroidsTag_t     = ECS::Base_t<AsteroidsTagComponent_t>;
using PlayerTag_t        = ECS::Base_t<AsteroidsTagComponent_t>;
using BulletTag_t        = ECS::Base_t<AsteroidsTagComponent_t>;

using Explosion_t        = ECS::Derived_t<Animable_t, Renderable_t, Timeable_t>;

using Bullet_t           = ECS::Derived_t<Animable_t, Movable_t, Renderable_t, Timeable_t, BulletTag_t, Collidable_t>;

using BulletSpawnerOn_t  = ECS::Derived_t<Spawner_t, Movable_t, InputEnabler_t, PlayerInput_t, Chargeable_t>;
using BulletSpawnerOff_t = ECS::Derived_t<Movable_t, InputEnabler_t, PlayerInput_t, Chargeable_t>;

using Asteroids_t        = ECS::Derived_t<Animable_t, Movable_t, Renderable_t, AsteroidsTag_t, Alive_t, Collidable_t>;
using AsteroidsSpawner_t = ECS::Derived_t<Spawner_t, Movable_t>;
using RocketOn_t         = ECS::Derived_t<Animable_t, Movable_t, InputEnabler_t, PlayerInput_t, Alive_t>;
using RocketOff_t        = ECS::Derived_t<Movable_t, InputEnabler_t, PlayerInput_t, Alive_t>;
using Player_t           = ECS::Derived_t<Renderable_t, Movable_t, PlayerInput_t, Alive_t, PlayerTag_t, Collidable_t>;

using ECSMan_t = ECS::ECSManager_t<Renderable_t,
                                   Movable_t,
                                   Animable_t,
                                   PlayerInput_t,
                                   InputEnabler_t,
                                   Alive_t,
                                   Spawner_t,
                                   Timeable_t,
                                   Chargeable_t,
                                   Collidable_t,
                                   Explosion_t,
                                   Bullet_t,
                                   BulletSpawnerOn_t,
                                   BulletSpawnerOff_t,
                                   Asteroids_t,
                                   AsteroidsSpawner_t,
                                   RocketOn_t,
                                   RocketOff_t,
                                   Player_t>;
