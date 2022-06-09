#pragma once

#include <raylib.h>

#include <helpers.hpp>
#include <type_traits>

#include "types.hpp"

struct ResourceManager_t : ECS::Uncopyable_t
{
    explicit ResourceManager_t()
        : mPlayer { LoadTexture("./resources/images/ship.png") },
          mRocketBottom { LoadTexture("./resources/images/rocket_bottom1.png") },
          mRocketFront { LoadTexture("./resources/images/rocket_front1.png") },
          mBackground { LoadTexture("./resources/images/background.png") }
    {
        
    }

    ~ResourceManager_t()
    {
        UnloadTexture(mBackground);
        UnloadTexture(mRocketBottom);
        UnloadTexture(mRocketFront);
        UnloadTexture(mPlayer);
    }

    Texture2D GetTexturePlayer()       const { return mPlayer; }
    Texture2D GetTextureRocketBottom() const { return mRocketBottom; }
    Texture2D GetTextureRocketFront()  const { return mRocketFront; }
    Texture2D GetTextureBackground()   const { return mBackground; }
private:
    const Texture2D mPlayer {  };
    const Texture2D mRocketBottom {  };
    const Texture2D mRocketFront {  };
    const Texture2D mBackground {  };
};
