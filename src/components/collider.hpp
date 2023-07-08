#pragma once

#include <type_aliases.hpp>
#include <optional>

struct Collidable_t;

struct ColliderComponent_t
{
    int radius {  };
    int damage {  };
    std::optional<ECS::Handle_t<Collidable_t>> collided {  };
};
