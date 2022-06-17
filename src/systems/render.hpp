#pragma once

#include <string_view>

#include <raylib.h>

#include "interface.hpp"
#include "types.hpp"

#include <components/render.hpp>

struct RenderSystem_t
{
    explicit RenderSystem_t(int width, int height, std::string_view title)
        : mWidth { width }, mHeight { height }
    {
        InitWindow(width, height + mStatusBarHeight, title.data());
        SetTargetFPS(60);
    }

    ~RenderSystem_t()
    {
        CloseWindow();
    }

    void set_background(Texture2D background)
    {
        mBackground = background;
    }

    template<class ECSMan_t> constexpr void
    update(const ECSMan_t& ecs_man)
    {
        BeginDrawing();
        DrawTexture(mBackground, 0, 0, RAYWHITE);
        ecs_man.template ForEachEntity<Renderable_t>(
                [&](auto& ren, auto& phy, auto&&)
                {
                    const Rectangle dest_rec   { phy.pos.x, phy.pos.y, ren.crop_rec.width * phy.size, ren.crop_rec.height * phy.size };
                    const Rectangle dest_recx  { phy.pos.x - mWidth, phy.pos.y, ren.crop_rec.width * phy.size, ren.crop_rec.height * phy.size };
                    const Rectangle dest_recy  { phy.pos.x, phy.pos.y - mHeight, ren.crop_rec.width * phy.size, ren.crop_rec.height * phy.size };
                    const Rectangle dest_recxy { phy.pos.x - mWidth, phy.pos.y - mHeight, ren.crop_rec.width * phy.size, ren.crop_rec.height * phy.size };
                    DrawTexturePro(ren.sprite, ren.crop_rec, dest_rec, phy.orig, phy.ang, RAYWHITE);
                    //x reflection
                    DrawTexturePro(ren.sprite, ren.crop_rec, dest_recx, phy.orig, phy.ang, RAYWHITE);
                    //// y reflection
                    DrawTexturePro(ren.sprite, ren.crop_rec, dest_recy, phy.orig, phy.ang, RAYWHITE);
                    //// xy reflection
                    DrawTexturePro(ren.sprite, ren.crop_rec, dest_recxy, phy.orig, phy.ang, RAYWHITE);
                    DrawCircleV(phy.pos, phy.size, GREEN);
                    DrawText(TextFormat("%f", phy.vel.y), phy.pos.x, phy.pos.y, 10, RED);
                });
        DrawRectangle(0, mHeight, mWidth, mStatusBarHeight, BLACK);
        ecs_man.template ForEachEntity<Chargeable_t>([&](auto& chrg, auto&& ent){
                    float percent { static_cast<float>(chrg.max_charge) };
                    unsigned bullets {  };
                    if constexpr (ECS::IsInstanceOf_v<Spawner_t, decltype(ent)>) {
                        auto& spwn { ecs_man.template GetComponent<SpawnComponent_t>(ent) };
                        bullets = spwn.spawned;
                    } else {
                        bullets = chrg.charged;
                    }
                    percent = bullets / percent;
                    DrawRectangle(10, mHeight + 10, 300, 15, WHITE);
                    DrawRectangle(10, mHeight + 10, 300 * percent, 15, YELLOW);
                    DrawText(TextFormat("%d", bullets), 315, mHeight + 10, 15, YELLOW);
                });
        ecs_man.template ForEachEntity<Alive_t>([&](auto& hel, auto&& ent) {
            if constexpr (ECS::IsInstanceOf_v<Player_t, decltype(ent)>) {
                DrawRectangle(10, mHeight + 10 + 15 + 5, 300, 15, WHITE);
                DrawRectangle(10, mHeight + 10 + 15 + 5, 300 * (hel.health / 100.0f), 15, GREEN);
                DrawText(TextFormat("%d", hel.health), 315, mHeight + 10 + 15 + 5, 15, YELLOW);
            }
        });
        DrawFPS(10, 10);
        EndDrawing();
    }
private:
    int mStatusBarHeight { 50 };
    int mWidth  {  };
    int mHeight {  };
    Texture2D mBackground {  };
};
