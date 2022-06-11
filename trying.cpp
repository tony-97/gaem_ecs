#include <raylib.h>
#include <cmath>

Vector2 rotate_vector(Vector2 p, Vector2 orig, float rot)
{
    auto dist { std::hypot(p.x - orig.x, p.y - orig.y) };
    auto ang  { std::atan2(p.y - orig.y, p.x - orig.x) };
    p.x = orig.x + dist * std::cos(ang + rot * DEG2RAD);
    p.y = orig.y + dist * std::sin(ang + rot * DEG2RAD); 

    return p;
}

Vector2 to_polar_vector(Vector2 p, Vector2 orig, float rot)
{
    auto dist { std::hypot(p.x - orig.x, p.y - orig.y) };
    auto ang  { std::atan2(p.y - orig.y, p.x - orig.x) };

    return { dist, ang * RAD2DEG + rot };
}

Vector2 to_rect_vector(Vector2 polar, Vector2 pos)
{
    return { pos.x + polar.x * std::cos(polar.y * DEG2RAD),
             pos.y + polar.x * std::sin(polar.y * DEG2RAD) };
}

int main()
{
    InitWindow(640, 480, "Test!");
    const Rectangle rec { 0.0f, 0.0f, 100, 20.0f };
    const Vector2 orig { rec.width / 2.0f, rec.height / 2.0f };
    Vector2 pos { 200.0f, 300.0f };
    float rot { 0.0f };

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        pos = GetMousePosition();
        auto v1 { to_polar_vector(Vector2{ rec.x, rec.y }, orig, rot) };
        auto v2 { to_polar_vector(Vector2{ rec.x + rec.width, rec.y }, orig, rot) };
        auto v3 { to_polar_vector(Vector2{ rec.x, rec.y + rec.height }, orig, rot) };
        auto v4 { to_polar_vector(Vector2{ rec.x + rec.width, rec.y + rec.height }, orig, rot) };
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircleV(to_rect_vector(v1, pos), 5, RED);
        DrawCircleV(to_rect_vector(v2, pos), 5, YELLOW);
        DrawCircleV(to_rect_vector(v3, pos), 5, PINK);
        DrawCircleV(to_rect_vector(v4, pos), 5, GREEN);
        DrawCircleV(pos, 5, BLUE);
        EndDrawing();
   
        if (IsKeyDown(KEY_SPACE)) {
            ++rot;
        }
    }
    return 0;
}
