#pragma once

#include <raylib.h>

#include <functional>

struct SpawnComponent_t
{
    std::function<void(Vector2 pos, float rot)> fn_spawn {  };
    float spawn_interval {  };
    float elapsed {  };
    unsigned spawned {  };
};
