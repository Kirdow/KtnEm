#include "base.h"
#include "game.h"
#include "input.h"
#include "level.h"
#include <array>
#include <glm/glm.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

extern uint32_t WndScale;

namespace KtnEm
{
    Game::Game(GameSpec spec)
    {
        m_Wnd = CreateRef<Wnd>(spec.Width, spec.Height);
        m_R3D = CreateRef<R3D>(m_Wnd);

        m_Res = CreateScope<Resolution>(m_Wnd->GetCanvasHeight(), WndScale);

        m_Player = CreateRef<Player>();

        auto& pos = m_Player->GetPosition();
        pos.x = 8;
        pos.y = 8;

        m_Frames.AddCallback([](uint32_t fps)
        {
            EM_ASM_({
                Module['fps'].innerText = `FPS: ${$0}`;
            }, fps);
        });

        m_Wnd->SetCallback([](uint32_t w, uint32_t h, const void* data)
        {
            EM_ASM_({
                const data = Module.HEAPU8.slice($0, $0 + $1 * $2 * 4);
                const context = Module['context'];
                const imageData = context.getImageData(0, 0, $1, $2);
                imageData.data.set(data);
                context.putImageData(imageData, 0, 0);
            }, data, w, h);
        },[](uint32_t w, uint32_t h)
        {
            EM_ASM_({
                const canvas = Module['canvas'];
                canvas.width = $0;
                canvas.height = $1;
                canvas.style.margin = `100px calc(50% - ${Math.floor($0 / 2)}px)`;
                Module['context'] = canvas.getContext('2d', { willReadFrequently: true });
            }, w, h);
        });

        EM_ASM_({
            const canvas = document.createElement('canvas');
            canvas.id = "wasmcanvas";
            canvas.width = $0;
            canvas.height = $1;
            canvas.style.margin = `100px calc(50% - ${Math.floor($0 / 2)}px)`;
            document.body.style.background = '#181A1B';
            document.body.style.color = '#fff';
            Module['canvas'] = canvas;
            Module['context'] = canvas.getContext('2d', { willReadFrequently: true });

            const fps = document.createElement('div');
            Module['fps'] = fps;
            document.body.appendChild(fps);
            document.body.appendChild(canvas);

            const controls = document.createElement('div');
            let text = "";
            const addt = (t) => {
                if (text.length) text += "<br>";
                text += t;
            };

            addt("Controls: ");
            addt("WASD - Move");
            addt("Left/Right Arrow - Turn");
            addt("Up/Down Arrow - Move");

            controls.innerHTML = text;
            controls.style.width = '100%';
            controls.style.textAlign = 'center';
            document.body.appendChild(controls);

            window.addEventListener('keydown', e => Module.OnKey(e.keyCode, true), true);
            window.addEventListener('keyup', e => Module.OnKey(e.keyCode, false), true);
        }, m_Wnd->GetCanvasWidth(), m_Wnd->GetCanvasHeight());

        m_Player->ValidateSpawn();
    }

    Game::~Game()
    {

    }

    void Game::Resize(uint32_t width, uint32_t height, int32_t scale)
    {
        if (scale > 0 && scale <= 64)
            WndScale = scale;
        m_Wnd->Resize(width, height);
        m_R3D->Resize();
    }

    void Game::Run()
    {
        Next();

        EM_ASM_({
            window.requestAnimationFrame(() => {
                Module.Run();
            });
        });
    }

    void Game::Next()
    {
        float delta = m_Frames.NextDelta();
        Tick(delta);

        Frame();
        m_Frames.Frame();
    }

    void Game::Tick(float delta)
    {
        m_Player->Tick(delta);
    }

    void Game::Frame()
    {
        m_R3D->DrawFrame(m_Player);
        m_Wnd->Flush();
    }
}

KtnEm::Game* GetGame();

static KtnEm::Scope<KtnEm::Resolution>& GetRes()
{
    return GetGame()->GetResolution();
}

uint32_t SetScale(uint32_t scale)
{
    auto& res = GetRes();
    res->SetScale(scale);
    return res->GetScale();
}

uint32_t GetScale()
{
    return GetRes()->GetScale();
}

void SetHeight(uint32_t height)
{
    auto scale = GetScale();
    if (height < scale * 4) height = scale * 4;
    GetRes()->SetHeight(height);
}

std::array<uint32_t, 2> GetSize()
{
    return GetRes()->GetSize();
}

EMSCRIPTEN_BINDINGS(EMStuff)
{
    emscripten::function("SetScale", &SetScale);
    emscripten::function("GetScale", &GetScale);
    emscripten::function("SetHeight", &SetHeight);
    emscripten::function("GetSize", &GetSize);

    emscripten::value_array<std::array<uint32_t, 2>>("array_int_2")
        .element(emscripten::index<0>())
        .element(emscripten::index<1>());
}
