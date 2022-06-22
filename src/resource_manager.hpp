#pragma once

#include <raylib.h>

#include <helpers.hpp>

struct ResourceManager_t : ECS::Uncopyable_t
{
    explicit ResourceManager_t();
    ~ResourceManager_t();
    Sound GetLaserSound() const;

    Texture2D GetTexturePlayer()        const;
    Texture2D GetTextureRocketBottom()  const;
    Texture2D GetTextureRocketFront()   const;
    Texture2D GetTextureAsteroid()      const;
    Texture2D GetTextureAsteroidSmall() const;
    Texture2D GetTextureBulletFire()    const;
    Texture2D GetTextureBackground()    const;
    Texture2D GetTextureAstroidExplosion() const;
    Texture2D GetTextureLaserExplosion() const;
private:
    Sound  mLaser {  };

    const Texture2D mPlayer        {  };
    const Texture2D mRocketBottom  {  };
    const Texture2D mRocketFront   {  };
    const Texture2D mBulletFire    {  };
    const Texture2D mAsteroid      {  };
    const Texture2D mAsteroidSmall {  };
    const Texture2D mBackground    {  };
    const Texture2D mAsteroidExplosion {  };
    const Texture2D mLaserExplosion {  };
};
