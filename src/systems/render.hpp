#pragma once

#include <raylib.h>

#include "types.hpp"

#include <resource_manager.hpp>

struct RenderSystem_t
{
    explicit RenderSystem_t(int width, int height, std::string_view title, ResourceManager_t& res_man)
        : mWidth { width }, mHeight { height }, mResMan { res_man }
    {
        InitWindow(width, height, title.data());
        SetTargetFPS(240);
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
    update(const ECSMan_t& ecs_man, unsigned player_score, unsigned player_max_lives, unsigned player_lives)
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
                });
        const char* score { TextFormat("%d", player_score) };
        DrawText(score, (mWidth - MeasureText(score, mScoreFontSize)) * 0.5f, mStatusY, mScoreFontSize, WHITE);
        auto ship { mResMan.GetTexturePlayer() };
        auto scale { mScaleShip / ship.width };
        unsigned i {  };
        for (; i < player_max_lives - player_lives; ++i) {
            DrawTextureEx(ship, { mWidth - mStatusX - mScaleShip - (mScaleShip + mPadingX) * i, mStatusY }, 0.0f, scale, DARKGRAY);
        }
        for (; i < player_max_lives; ++i) {
            DrawTextureEx(ship, { mWidth - mStatusX - mScaleShip - (mScaleShip + mPadingX) * i, mStatusY }, 0.0f, scale, WHITE);
        }
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
                    DrawRectangle(mStatusX, mStatusY, mBarWidth, mBarHeight, WHITE);
                    DrawRectangle(mStatusX, mStatusY, mBarWidth * percent, mBarHeight, YELLOW);
                    DrawText(TextFormat("%d", bullets), mStatusX + mBarWidth + mPadingX, mStatusY, mStatusFontSize, YELLOW);
                });
        ecs_man.template ForEachEntity<Alive_t>([&](auto& hel, auto&& ent) {
            if constexpr (ECS::IsInstanceOf_v<Player_t, decltype(ent)>) {
                DrawRectangle(mStatusX, mStatusY + mBarHeight + mPadingY, mBarWidth, mBarHeight, WHITE);
                DrawRectangle(mStatusX, mStatusY + mBarHeight + mPadingY, mBarWidth * (hel.health / 100.0f), mBarHeight, GREEN);
                DrawText(TextFormat("%d", hel.health), mStatusX + mBarWidth + mPadingX, mStatusY + mBarHeight + mPadingY, mStatusFontSize, GREEN);
            }
        });
        DrawFPS(10, mHeight - 50);
        EndDrawing();
    }
private:
    constexpr static inline float mScaleShip { 16.0f };

    constexpr static inline int mBarWidth       { 200 };
    constexpr static inline int mBarHeight      { 15 };
    constexpr static inline int mStatusFontSize { 15 };
    constexpr static inline int mScoreFontSize  { 30 };

    constexpr static inline int mStatusX { 20 };
    constexpr static inline int mStatusY { 20 };

    constexpr static inline int mPadingX { 5 };
    constexpr static inline int mPadingY { 5  };

    int mWidth  {  };
    int mHeight {  };
    Texture2D mBackground {  };
    const ResourceManager_t& mResMan;
};
