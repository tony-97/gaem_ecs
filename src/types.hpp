#pragma once

#include <ecs_manager.hpp>

#include "class.hpp"
#include "components/render.hpp"
#include "components/physics.hpp"
#include "components/animation.hpp"
#include "components/input_enabler.hpp"
#include "components/player_input.hpp"
#include "components/health.hpp"
#include "components/timer.hpp"
#include "components/spawn.hpp"
#include "components/charge.hpp"
#include "components/collider.hpp"
#include "components/parent.hpp"
#include "components/engine_effect.hpp"

#include "events/collision.hpp"

#include "event_manager.hpp"
#include "tmpl/type_list.hpp"

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
struct EngineEffectComponent_t;

template<class T> struct ParentComponent_t;

struct Movable_t          : ECS::Class_t<PhysicsComponent_t>{  };
struct Renderable_t       : ECS::Class_t<RenderComponent_t, Movable_t> {  };
struct Animable_t         : ECS::Class_t<AnimationComponent_t, RenderComponent_t>{  };
struct PlayerInput_t      : ECS::Class_t<PlayerInputComponent_t, Movable_t>{  };
struct InputEnabler_t     : ECS::Class_t<InputEnablerComponent_t>{  };
struct Alive_t            : ECS::Class_t<HealthComponent_t>{  };
struct Spawner_t          : ECS::Class_t<SpawnComponent_t, Movable_t>{  };
struct Timeable_t         : ECS::Class_t<TimerComponent_t, Alive_t>{  };
struct Chargeable_t       : ECS::Class_t<ChargeComponent_t>{  };
struct Collidable_t       : ECS::Class_t<ColliderComponent_t, Movable_t>{  };
struct ParentPhysics_t    : ECS::Class_t<ParentComponent_t<Movable_t>>{  };
struct ParentHealth_t     : ECS::Class_t<HealthComponent_t, ParentComponent_t<Alive_t>>{  };
struct Engine_t           : ECS::Class_t<Renderable_t, Alive_t>{  };
struct Explosion_t        : ECS::Class_t<Animable_t, Renderable_t, Timeable_t>{  };
struct Bullet_t           : ECS::Class_t<Animable_t, Renderable_t, Timeable_t, Collidable_t>{  };
struct WeaponOn_t         : ECS::Class_t<InputEnabler_t, Chargeable_t, Spawner_t, Renderable_t, Animable_t, Alive_t>{  };
struct WeaponOff_t        : ECS::Class_t<InputEnabler_t, Chargeable_t, Renderable_t, Alive_t, SpawnComponent_t, AnimationComponent_t>{  };
struct Asteroids_t        : ECS::Class_t<Animable_t, Renderable_t, Alive_t, Collidable_t>{  };
struct AsteroidsSpawner_t : ECS::Class_t<Spawner_t, Movable_t>{  };
struct RocketOn_t         : ECS::Class_t<Animable_t, Renderable_t, Movable_t, InputEnabler_t, Alive_t, EngineEffectComponent_t>{  };
struct RocketOff_t        : ECS::Class_t<Movable_t, InputEnabler_t, Alive_t, RenderComponent_t, AnimationComponent_t>{  };
struct Player_t           : ECS::Class_t<Renderable_t, PlayerInput_t, Alive_t, Collidable_t, ParentPhysics_t, ParentHealth_t>{  };

struct ECSConfig_t
{
using Signatures_t = TMPL::TypeList_t<Movable_t          ,
                                      Renderable_t       ,
                                      Animable_t         ,
                                      PlayerInput_t      ,
                                      InputEnabler_t     ,
                                      Alive_t            ,
                                      Spawner_t          ,
                                      Timeable_t         ,
                                      Chargeable_t       ,
                                      Collidable_t       ,
                                      ParentPhysics_t    ,
                                      ParentHealth_t     ,
                                      Engine_t           ,
                                      Explosion_t        ,
                                      Bullet_t           ,
                                      WeaponOn_t         ,
                                      WeaponOff_t        ,
                                      Asteroids_t        ,
                                      AsteroidsSpawner_t ,
                                      RocketOn_t         ,
                                      RocketOff_t        ,
                                      Player_t>;
};

using ECSMan_t = ECS::ECSManager_t<ECSConfig_t>;

struct EVConfig_t
{
    using events = TMPL::TypeList_t<CollisionEvent_t>;
};

using EVMan_t = EventManager_t<EVConfig_t>;
