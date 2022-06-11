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

int main()
{
    InitWindow(640, 480, "Test!");
    Rectangle rec { 0.0f, 0.0f, 100, 20.0f };
    Vector2 orig { rec.width / 2.0f, rec.height / 2.0f };
    Vector2 pos {  };
    float rot { 0.0f };

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        auto v1 { rotate_vector(Vector2{ rec.x, rec.y }, orig, rot) };
        auto v2 { rotate_vector(Vector2{ rec.x + rec.width, rec.y }, orig, rot) };
        auto v3 { rotate_vector(Vector2{ rec.x, rec.y + rec.height }, orig, rot) };
        auto v4 { rotate_vector(Vector2{ rec.x + rec.width, rec.y + rec.height }, orig, rot) };
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircleV(v1, 5, RED);
        DrawCircleV(v2, 5, YELLOW);
        DrawCircleV(v3, 5, PINK);
        DrawCircleV(v4, 5, GREEN);
        DrawCircleV(orig, 5, BLUE);
        EndDrawing();
   
        if (IsKeyDown(KEY_SPACE)) {
            ++rot;
        }
    }
    return 0;
}
