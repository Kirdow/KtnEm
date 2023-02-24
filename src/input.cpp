#include "base.h"
#include "input.h"

#include <emscripten.h>
#include <emscripten/bind.h>

namespace KtnEm
{
    struct KeyState
    {
        bool forwardW = false;
        bool backwardS = false;
        bool forwardUp = false;
        bool backwardDown;
        bool left = false;
        bool right = false;
        bool turnLeft = false;
        bool turnRight = false;
    };

    static KeyState s_States;

    bool Input::KeyForward() { return s_States.forwardW || s_States.forwardUp; }
    bool Input::KeyBackward() { return s_States.backwardS || s_States.backwardDown; }
    bool Input::KeyLeft() { return s_States.left; }
    bool Input::KeyRight() { return s_States.right; }
    bool Input::KeyTurnLeft() { return s_States.turnLeft; }
    bool Input::KeyTurnRight() { return s_States.turnRight; }

    bool Input::SubmitKey(uint32_t keyCode, bool state)
    {
        bool* rbool = nullptr;

        switch (keyCode)
        {
        case 37: rbool = &s_States.turnLeft; break;
        case 39: rbool = &s_States.turnRight; break;
        case 38: rbool = &s_States.forwardUp; break;
        case 87: rbool = &s_States.forwardW; break;
        case 40: rbool = &s_States.backwardDown; break;
        case 83: rbool = &s_States.backwardS; break;
        case 65: rbool = &s_States.left; break;
        case 68: rbool = &s_States.right; break;
        }

        if (rbool)
            *rbool = state;

        //std::cout << keyCode << ": " << state << "\n";

        return state;
    }

    class Game;
}
KtnEm::Game* GetGame();

static bool OnKey(uint32_t keyCode, bool state)
{
    if (!GetGame()) return false;

    return KtnEm::Input::SubmitKey(keyCode, state);
}

EMSCRIPTEN_BINDINGS(EMStuff)
{
    emscripten::function("OnKey", &OnKey);
}