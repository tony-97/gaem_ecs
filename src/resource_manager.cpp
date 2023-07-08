#include "resource_manager.hpp"
#include "raylib.h"

ResourceManager_t::ResourceManager_t()
    : sLaser             { []() { InitAudioDevice(); return LoadSound("./resources/images/laser_sound.wav"); }() },
      tShip              { LoadTexture("./resources/images/ship.png") },
      tEngine            { LoadTexture("./resources/images/engine.png") },
      tWeapon            { LoadTexture("./resources/images/weapon.png") },
      tEngineEffect      { LoadTexture("./resources/images/engine_effect.png") },
      tProjectileRocket  { LoadTexture("./resources/images/projectile_rocket.png") },
      tAsteroid          { LoadTexture("./resources/images/asteroid.png") },
      tBackground        { LoadTexture("./resources/images/background.png") },
      tAsteroidExplosion { LoadTexture("./resources/images/explosion_asteroid.png") },
      tLaserExplosion    { LoadTexture("./resources/images/explosion_laser.png") },
      tShipExplosion     { LoadTexture("./resources/images/explosion_ship.png") } {  }

ResourceManager_t::~ResourceManager_t()
{
    UnloadSound(sLaser);
    UnloadTexture(tShip       );
    UnloadTexture(tEngine  );
    UnloadTexture(tProjectileRocket   );
    UnloadTexture(tAsteroid     );
    UnloadTexture(tBackground   );
    UnloadTexture(tAsteroidExplosion);
    UnloadTexture(tLaserExplosion);
    UnloadTexture(tShipExplosion);
    CloseAudioDevice();
}
