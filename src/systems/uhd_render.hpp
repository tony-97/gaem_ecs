#pragma once

#include <raylib.h>

#include <types.hpp>
#include <resource_manager.hpp>

struct UHDRenderSystem_t
{
    constexpr explicit UHDRenderSystem_t(int width, int height, const ResourceManager_t& res_man)
        : mWidth { width }, mHeight { height }, mResMan { res_man } {  }

    template<class ECSMan_t>
    constexpr void update(const ECSMan_t& ecs_man, unsigned player_score, unsigned player_max_lives, unsigned player_lives)
    {
        const char* score { TextFormat("%d", player_score) };
        DrawText(score, (mWidth - MeasureText(score, mScoreFontSize)) * 0.5f, mStatusY, mScoreFontSize, WHITE);
        const auto ship { mResMan.tShip };
        const auto scale { mScaleShip / ship.width };
        auto i { 0u };
        for (; i < player_max_lives - player_lives; ++i) {
            DrawTextureEx(ship, { mWidth - mStatusX - mScaleShip - (mScaleShip + mPadingX) * i, mStatusY }, 0.0f, scale, DARKGRAY);
        }
        for (; i < player_max_lives; ++i) {
            DrawTextureEx(ship, { mWidth - mStatusX - mScaleShip - (mScaleShip + mPadingX) * i, mStatusY }, 0.0f, scale, WHITE);
        }
        ecs_man.template ForEach<Chargeable_t>([&](auto& chrg, auto ent){
                    auto percent { static_cast<float>(chrg.max_charge) };
                    unsigned bullets {  };
                    bullets = chrg.charged;
                    ecs_man.template Match<Spawner_t>(ent, [&](auto e) {
                                auto& spwn { ecs_man.template GetComponent<SpawnComponent_t>(e) };
                                bullets = spwn.to_spawn;
                            });
                    percent = bullets / percent;
                    DrawRectangle(mStatusX, mStatusY, mBarWidth, mBarHeight, WHITE);
                    DrawRectangle(mStatusX, mStatusY, mBarWidth * percent, mBarHeight, YELLOW);
                    DrawText(TextFormat("%d", bullets), mStatusX + mBarWidth + mPadingX, mStatusY, mStatusFontSize, YELLOW);
                });
        ecs_man.template ForEach<Player_t>([&](auto ent) {
                auto& hel { ecs_man.template GetComponent<HealthComponent_t>(ent) };
                DrawRectangle(mStatusX, mStatusY + mBarHeight + mPadingY, mBarWidth, mBarHeight, WHITE);
                DrawRectangle(mStatusX, mStatusY + mBarHeight + mPadingY, mBarWidth * (hel.health / 100.0f), mBarHeight, GREEN);
                DrawText(TextFormat("%d", hel.health), mStatusX + mBarWidth + mPadingX, mStatusY + mBarHeight + mPadingY, mStatusFontSize, GREEN);
        });
        DrawFPS(10, mHeight - 50);
        DrawText(TextFormat("Renderables: %u", ecs_man.template Size<Renderable_t>()), 10, 50, 20, GREEN);
    }
private:
                     int mWidth  {  };
    [[maybe_unused]] int mHeight {  };
    const ResourceManager_t& mResMan;

    constexpr static inline float mScaleShip { 24.0f };

    constexpr static inline int mBarWidth       { 200 };
    constexpr static inline int mBarHeight      { 15 };
    constexpr static inline int mStatusFontSize { 15 };
    constexpr static inline int mScoreFontSize  { 30 };

    constexpr static inline int mStatusX { 20 };
    constexpr static inline int mStatusY { 20 };

    constexpr static inline int mPadingX { 5 };
    constexpr static inline int mPadingY { 5  };
};
