#pragma once

#include <raylib.h>

struct PlayerInputComponent_t
{
    KeyboardKey left  { KEY_LEFT  };
    KeyboardKey up    { KEY_W    };
    KeyboardKey right { KEY_RIGHT };
    KeyboardKey down  { KEY_S  };
};
