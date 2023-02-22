#pragma once

#include <cstdint>

namespace KtnEm
{
    class Input
    {
    public:
        static bool KeyForward();
        static bool KeyBackward();
        static bool KeyLeft();
        static bool KeyRight();

        static bool KeyTurnLeft();
        static bool KeyTurnRight();

        static bool SubmitKey(uint32_t keyCode, bool state);
    };
}