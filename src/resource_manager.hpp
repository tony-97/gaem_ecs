#pragma once

#include <raylib.h>

#include <helpers.hpp>

struct ResourceManager_t : ECS::Uncopyable_t
{
    explicit ResourceManager_t();
    ~ResourceManager_t();

    Sound  sLaser {  };

    const Texture2D tShip              {  };
    const Texture2D tEngine            {  };
    const Texture2D tWeapon            {  };
    const Texture2D tEngineEffect      {  };
    const Texture2D tProjectileRocket  {  };
    const Texture2D tAsteroid          {  };
    const Texture2D tBackground        {  };
    const Texture2D tAsteroidExplosion {  };
    const Texture2D tLaserExplosion    {  };
    const Texture2D tShipExplosion     {  };
};
