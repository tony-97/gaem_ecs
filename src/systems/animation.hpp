#pragma once

#include <types.hpp>

struct AnimationSystem_t
{
    constexpr explicit AnimationSystem_t() = default;

    template<class ECSMan_t>
    constexpr static void update(ECSMan_t& ecs_man, float frame_time)
    {
        ecs_man.template ASYNCForEach<Animable_t>([&](auto& anim, auto& ren){
                    if (anim.elapsed >= anim.frame_time) {
                        anim.current_frame = (anim.current_frame + 1) % anim.frames;
                        anim.elapsed = 0.0f;
                        ren.crop_rec.x = anim.current_frame * ren.crop_rec.width;
                    }
                    anim.elapsed += frame_time;
                });
    }
};
