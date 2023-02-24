#include "base.h"
#include <chrono>
#include <math.h>
#include "emscripten.h"
#include "emscripten/bind.h"
#include "wnd.h"
#include "r3d.h"
#include "level.h"
#include "game.h"
#include "input.h"
#include "resolution.h"

static KtnEm::Game* s_Game = nullptr;
KtnEm::Game* GetGame()
{
    return s_Game;
}

void Start(uint32_t height)
{
    if (s_Game) return;

    s_Game = new KtnEm::Game(KtnEm::GameSpec(height * 4 / 3, height));
    s_Game->Run();
}

void Run()
{
    if (!s_Game) return;

    uint32_t height;
    uint32_t scale;
    if (s_Game->GetResolution()->Poll(nullptr, &height, &scale))
        s_Game->Resize(height * 4 / 3, height, scale);

    s_Game->Run();
}

EMSCRIPTEN_BINDINGS(EMStuff)
{
    emscripten::function("Start", &Start);
    emscripten::function("Run", &Run);
}