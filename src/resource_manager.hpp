#pragma once

#include <raylib.h>

#include <helpers.hpp>
#include <type_traits>

#include "types.hpp"

struct ResourceManager_t : ECS::Uncopyable_t
{
    explicit ResourceManager_t()
        : mPlayer        { LoadTexture("./resources/images/ship.png") },
          mRocketBottom  { LoadTexture("./resources/images/rocket_bottom.png") },
          mRocketFront   { LoadTexture("./resources/images/rocket_front.png") },
          mBulletFire    { LoadTexture("./resources/images/fire_red.png") },
          mAsteroid      { LoadTexture("./resources/images/rock.png") },
          mAsteroidSmall { LoadTexture("./resources/images/rock_small.png") },
          mBackground    { LoadTexture("./resources/images/background.png") }
    {
        InitAudioDevice();
        mLaser = LoadSound("./resources/sounds/laser_sound.wav");
    }

    ~ResourceManager_t()
    {
        UnloadSound(mLaser);
        UnloadTexture(mPlayer       );
        UnloadTexture(mRocketBottom );
        UnloadTexture(mRocketFront  );
        UnloadTexture(mBulletFire   );
        UnloadTexture(mAsteroid     );     
        UnloadTexture(mAsteroidSmall);
        UnloadTexture(mBackground   );
        CloseAudioDevice();
    }

    Sound GetLaserSound() const { return mLaser; }

    Texture2D GetTexturePlayer()        const { return mPlayer;        }
    Texture2D GetTextureRocketBottom()  const { return mRocketBottom;  }
    Texture2D GetTextureRocketFront()   const { return mRocketFront;   }
    Texture2D GetTextureAsteroid()      const { return mAsteroid;      }
    Texture2D GetTextureAsteroidSmall() const { return mAsteroidSmall; }
    Texture2D GetTextureBulletFire()    const { return mBulletFire;    }
    Texture2D GetTextureBackground()    const { return mBackground;    }
private:
    Sound  mLaser {  };

    const Texture2D mPlayer        {  };
    const Texture2D mRocketBottom  {  };
    const Texture2D mRocketFront   {  };
    const Texture2D mBulletFire    {  };
    const Texture2D mAsteroid      {  };
    const Texture2D mAsteroidSmall {  };
    const Texture2D mBackground    {  };
};
