#pragma once

#include <cstdint>
#include <cmath>
#include <math.h>

namespace KtnEm::Math
{
    template<typename T>
    constexpr T rescale(T src, T srcMin, T srcMax, T dstMin, T dstMax)
    {
        T factor = (src - srcMin) / (srcMax - srcMin);
        return dstMin + factor * (dstMax - dstMin);
    }
}