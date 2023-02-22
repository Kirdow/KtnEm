#pragma once

#include <cstdint>
#include <cmath>

namespace KtnEm::Math
{
    template<typename T>
    struct Vec2
    {
        T x, y;

        Vec2() : Vec2(T(), T()) {}
        Vec2(T x, T y) : x(x), y(y) {}

        T dot(const Vec2& other) const
        {
            return x * other.x + y * other.y;
        }

        T lengthSqr() const
        {
            return dot(*this);
        }

        T length() const
        {
            return (T)sqrt(lengthSqr());
        }

        Vec2 normal() const
        {
            auto len = length();
            if (len == T()) return Vec2();

            T _x = x / len;
            T _y = y / len;

            return Vec2(_x, _y);
        }
    };

    using Vec2f = Vec2<float>;
    using Vec2d = Vec2<double>;
    using Vec2i = Vec2<int32_t>;
    using Vec2u = Vec2<uint32_t>;
}