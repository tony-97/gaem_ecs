#pragma once

#include <math_vector.hpp>

#include <functional>

struct SpawnComponent_t
{
    std::function<void(Vec2f_t pos, float rot)> fn_spawn {  };
    float spawn_interval {  };
    unsigned to_spawn {  };
    float elapsed { spawn_interval + 0.1f };
};
