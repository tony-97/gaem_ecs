#include <raylib.h>

#include <ecs_manager.hpp>
#include <type_traits>

struct RenderComponent_t
{
    Texture2D sprite {  };
};

struct PositionComponent_t
{
    Vector2 pos {  };
};

struct PhysicsComponent_t
{
    Vector2 phy {  };
};

struct InputComponent_t
{
    KeyboardKey left  { KEY_LEFT  };
    KeyboardKey up    { KEY_UP    };
    KeyboardKey right { KEY_RIGHT };
    KeyboardKey down  { KEY_DOWN  };
};

int main()
{
    using Renderable_t     = ECS::Base_t<RenderComponent_t, PositionComponent_t>;
    using Movable_t        = ECS::Base_t<PositionComponent_t, PhysicsComponent_t>;
    using Inputable        = ECS::Base_t<InputComponent_t, PhysicsComponent_t>;
    using Invicible        = ECS::Derived_t<Inputable, Movable_t>;
    using BasicCharacter_t = ECS::Derived_t<Renderable_t, Movable_t, Inputable>;

    using ECSMan_t = ECS::ECSManager_t<Renderable_t, Movable_t, Inputable, BasicCharacter_t, Invicible>;

    InitWindow(640, 480, "ECS Gaem!");
    auto sp1 { LoadTexture("./sprite.png") };
    Args::Arguments_t player_ren_args { Args::For_v<RenderComponent_t>, sp1 };
    Args::Arguments_t player_pos_args { Args::For_v<PositionComponent_t>, 100.0f, 100.0f };

    ECSMan_t ecs_man {  };

    ecs_man.CreateEntity<BasicCharacter_t>(player_ren_args, player_pos_args);

    //SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawFPS(10, 10);
        ecs_man.ForEachEntity<Renderable_t>(
                [](auto& ren, auto& pos, auto&&)
                {
                    DrawTexture(ren.sprite, pos.pos.x, pos.pos.y, RAYWHITE);
                });
        EndDrawing();
        ecs_man.ForEachEntity<Inputable>(
                [&](auto& inp, auto& phy, auto&& ent_key) {
                    if (IsKeyDown(inp.up))    phy.phy.y = -100.0f;
                    if (IsKeyDown(inp.down))  phy.phy.y = +100.0f;
                    if (IsKeyDown(inp.left))  phy.phy.x = -100.0f;
                    if (IsKeyDown(inp.right)) phy.phy.x = +100.0f;
                    if constexpr (std::is_same_v<ECS::Entity_t<BasicCharacter_t>, typename std::remove_reference_t<decltype(ent_key)>::value_type>) {
                        if (IsKeyPressed(KEY_SPACE)) {
                            ecs_man.TransformTo<Invicible>(ent_key);
                        }
                    }
                    else if constexpr (std::is_same_v<ECS::Entity_t<Invicible>, typename std::remove_reference_t<decltype(ent_key)>::value_type>) {
                        if (IsKeyPressed(KEY_R)) {
                            ecs_man.TransformTo<BasicCharacter_t>(ent_key, player_ren_args);
                        }
                    }
                });
        ecs_man.ForEachEntity<Movable_t>(
                [](auto& pos, auto& phy, auto&&) {
                    pos.pos.x += phy.phy.x * GetFrameTime();
                    pos.pos.y += phy.phy.y * GetFrameTime();
                });
    }
    UnloadTexture(sp1);
    CloseWindow();
    return 0;
}
