#pragma once

#include <raylib.h>

struct InputEnablerComponent_t
{
    enum Object_t {
        ROCKET_FRONT,
        ROCKET_BOTTOM,
        ROCKET_LEFT,
        ROCKET_RIGHT,
        BULLET_SPAWNER,
    };
    KeyboardKey enable {  };
    Object_t type {  };
};
