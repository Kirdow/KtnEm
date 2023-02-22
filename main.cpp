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

static KtnEm::Game* s_Game = nullptr;

bool OnKey(uint32_t keyCode, bool state)
{
    if (!s_Game) return false;

    return KtnEm::Input::SubmitKey(keyCode, state);

 /*   if (keyCode >= 37 && keyCode <= 40)
    {
        bool* value = nullptr;
        switch (keyCode)
        {
        case 37: value = &s_Data.KA; break;
        case 38: value = &s_Data.KW; break;
        case 39: value = &s_Data.KD; break;
        case 40: value = &s_Data.KS; break;
        }

        *value = state;
        return true;
    }

    return false;*/
}

static uint32_t FixScale(uint32_t scale)
{
    if (scale <= 1) scale = 1;
    else if (scale <= 2) scale = 2;
    else if (scale <= 4) scale = 4;
    else if (scale <= 8) scale = 8;
    else if (scale <= 16) scale = 16;
    else if (scale <= 32) scale = 32;
    else scale = 64;

    return scale;
}

extern uint32_t WndScale;
static uint32_t* s_NewScale = nullptr;
static uint32_t s_Height = 0;

uint32_t SetScale(uint32_t scale);
void Start(uint32_t height)
{
    if (s_Game) return;
    WndScale = FixScale(height / 120);
    s_Height = height;

    s_Game = new KtnEm::Game(KtnEm::GameSpec(height * 4 / 3, height));
    s_Game->Run();

    if (s_NewScale)
    {
        delete s_NewScale;
        s_NewScale = nullptr;
    }
}

void Run()
{
    if (s_NewScale)
    {
        WndScale = *s_NewScale;
        delete s_NewScale;
        s_NewScale = nullptr;

        EM_ASM_({
            document.body.querySelectorAll("div").forEach(e => document.body.removeChild(e));
            document.body.querySelectorAll("canvas").forEach(e => document.body.removeChild(e));
        });

        delete s_Game;
        s_Game = new KtnEm::Game(KtnEm::GameSpec(s_Height * 4 / 3, s_Height));
    }    

    if (!s_Game) return;

    s_Game->Run();
}


uint32_t SetScale(uint32_t scale)
{
    scale = FixScale(scale);

    s_NewScale = new uint32_t(scale);
    return scale;
}

uint32_t GetScale()
{
    return WndScale;
}

EMSCRIPTEN_BINDINGS(EMStuff)
{
    emscripten::function("Start", &Start);
    emscripten::function("OnKey", &OnKey);
    emscripten::function("Run", &Run);
    emscripten::function("SetScale", &SetScale);
    emscripten::function("GetScale", &GetScale);

//    emscripten::class_<KtnEm::EPos>("EPos")
//        .property("x", &KtnEm::EPos::x)
//        .property("y", &KtnEm::EPos::y)
//        .property("rot", &KtnEm::EPos::rot)
//        .property("deg", &KtnEm::EPos::GetDeg, &KtnEm::EPos::SetDeg)
//        .class_function("getPlayerPos", &GetPlayerPos, emscripten::allow_raw_pointers());
}