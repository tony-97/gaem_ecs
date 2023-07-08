#pragma once

#include <raylib.h>

#include <math_vector.hpp>

struct PhysicsComponent_t
{
    Vec2f_t position     {  };
    Vec2f_t velocity     {  };
    Vec2f_t acceleration {  };
    Vec2f_t origin       {  };
    float rotation       {  };
    float friction       {  };
    float mass           { 1.0f };
};
