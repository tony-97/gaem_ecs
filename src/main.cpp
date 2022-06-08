#include <raylib.h>

#include "types.hpp"

#include "game_factory.hpp"
#include "resource_manager.hpp"

#include "systems/render.hpp"
#include "systems/input.hpp"
#include "systems/animation.hpp"

#include <stdlib.h>

int main()
{
    constexpr int screen_width  { 1280 };
    constexpr int screen_height { 720 };

    RenderSystem_t ren_sys { screen_width, screen_height, "GAEM ECS!" };
    InputSystem_t inp_sys {  };
    AnimationSystem_t anim_sys {  };

    ResourceManager_t res_man {  };

    ren_sys.set_background(res_man.GetTextureBackground());

    ECSMan_t ecs_man {  };

    GameFactory_t game_fact { ecs_man, res_man };

    game_fact.CreatePlayer(screen_width, screen_height);

    while (!WindowShouldClose()) {
        ren_sys.update(ecs_man);
        inp_sys.update(ecs_man, game_fact);
        anim_sys.update(ecs_man, GetFrameTime());
        ecs_man.ForEachEntity<Movable_t>(
                [&](auto& phy, auto&&) {
                    phy.vel.x += phy.acel.x * GetFrameTime();
                    phy.vel.y += phy.acel.y * GetFrameTime();

                    phy.pos.x += phy.vel.x * GetFrameTime();
                    phy.pos.y += phy.vel.y * GetFrameTime();
                    if (phy.pos.x + phy.orig.x > screen_width) {
                        phy.pos.x = phy.pos.x + phy.orig.x - screen_width;
                    } else if (phy.pos.x - phy.orig.x < 0) {
                        phy.pos.x = screen_width - phy.pos.x;
                    }
                    if (phy.pos.y > screen_height) {
                        phy.pos.y = 0.0f;
                    } else if (phy.pos.y < 0) {
                        phy.pos.y = screen_height;
                    }
                });
    }
    return 0;
}
