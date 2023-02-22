#pragma once

#include <math.h>

namespace KtnEm
{
    struct EPos
    {
        float x = 0.0f;
        float y = 0.0f;
        float rot = 0.0f;

        float cosine, sine;

        void Calculate()
        {
            cosine = cosf(rot);
            sine = sinf(rot);
        }

        float SetRad(float v)
        {
            return rot = v;
        }

        float SetDeg(float v)
        {
            rot = v * 3.141592f / 180.0f;
            return v;
        }

        float GetRad() const { return rot; }
        float GetDeg() const { return rot * 180.0f / 3.141592f; }
    };
}