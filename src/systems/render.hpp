#pragma once

#include <string_view>

#include <raylib.h>

struct RenderSystem_t
{
    explicit RenderSystem_t(int width, int height, std::string_view title)
    {
        InitWindow(width, height, title.data());
    }

    ~RenderSystem_t()
    {
        CloseWindow();
    }

    template<class ECSMan_t> constexpr void
    update(const ECSMan_t& ecs_man)
    {
        ecs_man.template ForEachEntity<Renderable_t>(
                [](auto& ren, auto& pos, auto&&)
                {
                    DrawTexture(ren.sprite, pos.pos.x, pos.pos.y, RAYWHITE);
                });
    }
};
