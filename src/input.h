#pragma once

#include <cstdint>

#include <glm/glm.hpp>

namespace KtnEm
{
    struct InputAxis
    {
        glm::vec2 Move;
        float Turn;

        InputAxis(glm::vec2 move = glm::vec2(), float turn = 0.0f)
            : Move(move), Turn(turn) {}

        bool IsMoving() const { return Move.x != 0.0f || Move.y != 0.0f; }
    };

    class Input
    {
    public:
        static bool KeyForward();
        static bool KeyBackward();
        static bool KeyLeft();
        static bool KeyRight();

        static bool KeyTurnLeft();
        static bool KeyTurnRight();

        static float GetMoveSpeed();
        static float GetTurnSpeed();
        static void SetMoveSpeed(float speed = 1.0f);
        static void SetTurnSpeed(float speed = 3.141592f);

        static InputAxis GetInputAxis(float delta = 1.0f);

        static bool SubmitKey(uint32_t keyCode, bool state);
    };
}