#pragma once

#include <raylib.h>
#include <cmath>

constexpr Vector2 to_polar_vector(Vector2 p, Vector2 orig, float rot)
{
    const auto dist { std::hypot(p.x - orig.x, p.y - orig.y) };
    const auto ang  { std::atan2(p.y - orig.y, p.x - orig.x) };

    return { dist, ang * RAD2DEG + rot };
}

constexpr Vector2 to_rect_vector(Vector2 polar, Vector2 pos)
{
    return { pos.x + polar.x * std::cos(polar.y * DEG2RAD),
             pos.y + polar.x * std::sin(polar.y * DEG2RAD) };
}
