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
        DrawTexture(mBackground, 0, mStatusBarHeight, RAYWHITE);
        ecs_man.template ForEachEntity<Renderable_t>(
                [&](auto& ren, auto& phy, auto&&)
                {
                    const Rectangle dest_rec   { phy.pos.x, phy.pos.y + mStatusBarHeight, ren.crop_rec.width * phy.size, ren.crop_rec.height * phy.size };
                    const Rectangle dest_recx  { phy.pos.x - mWidth, phy.pos.y + mStatusBarHeight, ren.crop_rec.width * phy.size, ren.crop_rec.height * phy.size };
                    const Rectangle dest_recy  { phy.pos.x, phy.pos.y - mHeight + mStatusBarHeight, ren.crop_rec.width * phy.size, ren.crop_rec.height * phy.size };
                    const Rectangle dest_recxy { phy.pos.x - mWidth, phy.pos.y - mHeight + mStatusBarHeight, ren.crop_rec.width * phy.size, ren.crop_rec.height * phy.size };
                    DrawTexturePro(ren.sprite, ren.crop_rec, dest_rec, phy.orig, phy.ang, RAYWHITE);
                    //x reflection
                    DrawTexturePro(ren.sprite, ren.crop_rec, dest_recx, phy.orig, phy.ang, RAYWHITE);
                    //// y reflection
                    DrawTexturePro(ren.sprite, ren.crop_rec, dest_recy, phy.orig, phy.ang, RAYWHITE);
                    //// xy reflection
                    DrawTexturePro(ren.sprite, ren.crop_rec, dest_recxy, phy.orig, phy.ang, RAYWHITE);
                });
        DrawRectangle(0, 0, mWidth, mStatusBarHeight, BLACK);
        DrawFPS(10, 10);
        EndDrawing();
    }
private:
    int mStatusBarHeight { 50 };
    int mWidth  {  };
    int mHeight {  };
    Texture2D mBackground {  };
};
