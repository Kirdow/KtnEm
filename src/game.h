#pragma once

#include "wnd.h"
#include "r3d.h"
#include "frames.h"
#include "resolution.h"

namespace KtnEm
{
    struct GameSpec
    {
        uint32_t Width;
        uint32_t Height;
        std::string Title;

        GameSpec(uint32_t width = 320, uint32_t height = 320, const std::string& title = "KtnEm")
            : Width(width), Height(height), Title(title) {}
    };

    class Player;
    class Game
    {
    public:
        Game(GameSpec spec = GameSpec());
        ~Game();

        Scope<Resolution>& GetResolution() { return m_Res; }

        void Run();
        void Next();

        void Resize(uint32_t width, uint32_t height, int32_t scale = -1);

        const Ref<Player>& GetPlayer() const { return m_Player; }
    private:
        void Tick(float delta);
        void Frame();
    private:
        Ref<Wnd> m_Wnd;
        Ref<R3D> m_R3D;
        Framerater m_Frames;
        Ref<Player> m_Player;
        Scope<Resolution> m_Res;
    };
}