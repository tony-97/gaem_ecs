#pragma once
#include <cmath>

#include <raylib.h>

template <typename T>
struct Vector_t
{
    T x {  };
    T y {  };

    constexpr operator Vector2() const
    {
        return { static_cast<float>(x), static_cast<float>(y) };
    }

    constexpr Vector_t<T>& operator=(T u)
    {
        x = y = u;
        return *this;
    }

    constexpr auto Distance(const Vector_t<T> vr)   const -> T
    {
        const auto diff = *this - vr;
        return diff.Modulus();
    }

    constexpr auto DistanceSQ(const Vector_t<T> vr) const -> T
    {
        const auto xdiff = x - vr.x;
        const auto ydiff = y - vr.y;
        return  xdiff * xdiff + ydiff * ydiff;
    }

    constexpr auto Modulus()                         const -> T
    {
        return std::hypot(x, y);
    }

    constexpr auto ModulusSQ()                       const -> T
    {
        const Vector_t<T> zero {  };
        return DistanceSQ(zero);
    }

    constexpr auto Normalize()                       const -> Vector_t<T>
    {
        return *this / Modulus();
    }

    constexpr auto DotProduct(const Vector_t<T> vr) const -> T
    {
        return x * vr.x + y * vr.y;
    }

    constexpr auto Tangent()                         const -> Vector_t<T>
    {
        return { -y, x };
    }
};

template <typename T, typename U>
constexpr auto operator*=(Vector_t<T>& vl, U value) -> Vector_t<T>&
{
    vl.x *= value;
    vl.y *= value;
    return vl;
}

template <typename T, typename U>
constexpr auto operator*=(U value, Vector_t<T> vl) -> Vector_t<T>&
{
    vl *= value;
    return vl;
}

template <typename T, typename U>
constexpr auto operator*(Vector_t<T> vl, U value) -> Vector_t<T>
{
    vl *= value;
    return vl;
}

template <typename T, typename U>
constexpr auto operator*(U value, Vector_t<T> vl) -> Vector_t<T>
{
    vl *= value;
    return vl;
}

template <typename T, typename U>
constexpr auto operator/=(Vector_t<T>& vl, U value) -> Vector_t<T>&
{
    vl *= (1 / value);
    return vl;
}

template <typename T, typename U>
constexpr auto operator/(Vector_t<T> vl, U value) -> Vector_t<T>
{
    vl /= value;
    return vl;
}

template <typename T>
constexpr auto operator+=(Vector_t<T>& vl, const Vector_t<T> vr) -> Vector_t<T>&
{
    vl.x += vr.x;
    vl.y += vr.y;
    return vl;
}

template <typename T>
constexpr auto operator+(Vector_t<T> vl, const Vector_t<T> vr) -> Vector_t<T>
{
    vl += vr;
    return vl;
}

template <typename T>
constexpr auto operator-=(Vector_t<T>& vl, const Vector_t<T> vr) -> Vector_t<T>&
{
    vl += -(vr);
    return vl;
}

template <typename T>
constexpr auto operator-(Vector_t<T> vl, const Vector_t<T> vr) -> Vector_t<T>
{
    vl -= vr;
    return vl;
}

template <typename T>
constexpr auto operator-(Vector_t<T> vl) -> Vector_t<T>
{
    vl.x = -vl.x;
    vl.y = -vl.y;

    return vl;
}

using Vec2f_t = Vector_t<float>;
