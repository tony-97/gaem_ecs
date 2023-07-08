#pragma once

#include <type_aliases.hpp>

struct Collidable_t;

struct CollisionEvent_t
{
    ECS::Handle_t<Collidable_t> e1;
    ECS::Handle_t<Collidable_t> e2;
};
