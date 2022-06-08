#pragma once

#include <raylib.h>

struct PhysicsComponent_t
{
    Vector2 pos  {  };
    Vector2 vel  {  };
    Vector2 acel {  };
    Vector2 orig {  };
    float ang    {  };
};
