#pragma once

#include <raylib.h>

struct RenderComponent_t
{
    Texture2D sprite {  };
    Rectangle crop_rec {  };
    float scale { 1.0f };
};
