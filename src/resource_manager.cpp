#include "resource_manager.hpp"
#include "raylib.h"

ResourceManager_t::ResourceManager_t()
    : mPlayer        { LoadTexture("./resources/images/ship.png") },
      mRocketBottom  { LoadTexture("./resources/images/rocket_bottom.png") },
      mRocketFront   { LoadTexture("./resources/images/rocket_front.png") },
      mBulletFire    { LoadTexture("./resources/images/bullet.png") },
      mAsteroid      { LoadTexture("./resources/images/rock.png") },
      mAsteroidSmall { LoadTexture("./resources/images/rock_small.png") },
      mBackground    { LoadTexture("./resources/images/background.png") },
      mAsteroidExplosion { LoadTexture("./resources/images/explosion_asteroid.png") },
      mLaserExplosion { LoadTexture("./resources/images/explosion_laser.png") },
      mShipExplosion  { LoadTexture("./resources/images/explosion_ship.png") }
{
    InitAudioDevice();
    mLaser = LoadSound("./resources/images/laser_sound.wav");
}

ResourceManager_t::~ResourceManager_t()
{
    UnloadSound(mLaser);
    UnloadTexture(mPlayer       );
    UnloadTexture(mRocketBottom );
    UnloadTexture(mRocketFront  );
    UnloadTexture(mBulletFire   );
    UnloadTexture(mAsteroid     );     
    UnloadTexture(mAsteroidSmall);
    UnloadTexture(mBackground   );
    UnloadTexture(mAsteroidExplosion);
    UnloadTexture(mLaserExplosion);
    UnloadTexture(mShipExplosion);
    CloseAudioDevice();
}

Sound ResourceManager_t::GetLaserSound() const { return mLaser; }

Texture2D ResourceManager_t::GetTexturePlayer()        const { return mPlayer;        }
Texture2D ResourceManager_t::GetTextureRocketBottom()  const { return mRocketBottom;  }
Texture2D ResourceManager_t::GetTextureRocketFront()   const { return mRocketFront;   }
Texture2D ResourceManager_t::GetTextureAsteroid()      const { return mAsteroid;      }
Texture2D ResourceManager_t::GetTextureAsteroidSmall() const { return mAsteroidSmall; }
Texture2D ResourceManager_t::GetTextureBulletFire()    const { return mBulletFire;    }
Texture2D ResourceManager_t::GetTextureBackground()    const { return mBackground;    }
Texture2D ResourceManager_t::GetTextureAstroidExplosion() const { return mAsteroidExplosion; }
Texture2D ResourceManager_t::GetTextureLaserExplosion() const { return mLaserExplosion; }
Texture2D ResourceManager_t::GetTextureShipExplosion()  const { return mShipExplosion; }
