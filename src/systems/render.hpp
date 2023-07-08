#pragma once

#include <raylib.h>

#include <types.hpp>

struct RenderSystem_t
{
    explicit RenderSystem_t(int width, int height, std::string_view title)
        : mWidth { width }, mHeight { height }
    {
        InitWindow(width, height, title.data());
        //SetWindowState(FLAG_FULLSCREEN_MODE);
        //SetTargetFPS(240);
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
        DrawTexture(mBackground, 0, 0, RAYWHITE);
        ecs_man.template ForEach<Renderable_t>(
                [&](auto& ren, auto& phy)
                {
                    const Rectangle dest_rec   { phy.position.x, phy.position.y, ren.crop_rec.width * ren.scale, ren.crop_rec.height * ren.scale };
                    const Rectangle dest_recx  { phy.position.x - mWidth, phy.position.y, ren.crop_rec.width * ren.scale, ren.crop_rec.height * ren.scale };
                    const Rectangle dest_recy  { phy.position.x, phy.position.y - mHeight, ren.crop_rec.width * ren.scale, ren.crop_rec.height * ren.scale };
                    const Rectangle dest_recxy { phy.position.x - mWidth, phy.position.y - mHeight, ren.crop_rec.width * ren.scale, ren.crop_rec.height * ren.scale };
                    DrawTexturePro(ren.sprite, ren.crop_rec, dest_rec, phy.origin, phy.rotation, RAYWHITE);
                    //x reflection
                    DrawTexturePro(ren.sprite, ren.crop_rec, dest_recx, phy.origin, phy.rotation, RAYWHITE);
                    //// y reflection
                    DrawTexturePro(ren.sprite, ren.crop_rec, dest_recy, phy.origin, phy.rotation, RAYWHITE);
                    //// xy reflection
                    DrawTexturePro(ren.sprite, ren.crop_rec, dest_recxy, phy.origin, phy.rotation, RAYWHITE);
                });
        
    }
private:
    int mWidth  {  };
    int mHeight {  };
    Texture2D mBackground {  };
};
