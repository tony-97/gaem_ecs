#pragma once

#include <string_view>

#include <raylib.h>

#include "types.hpp"

#include <components/render.hpp>

struct RenderSystem_t
{
    explicit RenderSystem_t(int width, int height, std::string_view title)
        : mWidth { width }, mHeight { height }
    {
        InitWindow(width, height, title.data());
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
                    const Rectangle dest_rec   { phy.pos.x, phy.pos.y, ren.crop_rec.width, ren.crop_rec.height };
                    const Rectangle dest_recx  { phy.pos.x - mWidth, phy.pos.y, ren.crop_rec.width, ren.crop_rec.height };
                    const Rectangle dest_recy  { phy.pos.x, phy.pos.y - mHeight, ren.crop_rec.width, ren.crop_rec.height };
                    const Rectangle dest_recxy { phy.pos.x - mWidth, phy.pos.y - mHeight, ren.crop_rec.width, ren.crop_rec.height };
                    DrawTexturePro(ren.sprite, ren.crop_rec, dest_rec, phy.orig, phy.ang, RAYWHITE);
                    // x reflection
                    DrawTexturePro(ren.sprite, ren.crop_rec, dest_recx, phy.orig, phy.ang, RAYWHITE);
                    ////DrawTexture(ren.sprite, pos.pos.x - mWidth, pos.pos.y, RAYWHITE);
                    //// y reflection
                    DrawTexturePro(ren.sprite, ren.crop_rec, dest_recy, phy.orig, phy.ang, RAYWHITE);
                    ////DrawTexture(ren.sprite, pos.pos.x, pos.pos.y - mHeight, RAYWHITE);
                    //// xy reflection
                    DrawTexturePro(ren.sprite, ren.crop_rec, dest_recxy, phy.orig, phy.ang, RAYWHITE);
                    //DrawTexture(ren.sprite, pos.pos.x - mWidth, pos.pos.y - mHeight, RAYWHITE);
                });
        DrawFPS(10, 10);
        EndDrawing();
    }
private:
    int mWidth  {  };
    int mHeight {  };
    Texture2D mBackground {  };
};
