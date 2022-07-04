#pragma once

#include <raylib.h>

struct PhysicsComponent_t
{
    Vector2 pos  {  };
    Vector2 vel  {  };
    Vector2 acc  {  };
    Vector2 orig {  };
    float friction {  };
    float ang    {  };
    float size   { 1.0f };
};
