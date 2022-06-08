#pragma once

#include <raylib.h>

struct RocketInputComponent_t
{
    enum RocketType_t {
        ROCKET_FRONT,
        ROCKET_BOTTOM
    };
    KeyboardKey enable {  };
    RocketType_t type {  };
};
