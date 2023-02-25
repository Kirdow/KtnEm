#pragma once

#include "wnd.h"
#include "player.h"

namespace KtnEm
{
    class R3D
    {
    public:
        R3D(const Ref<Wnd>& wnd);
        ~R3D();

        void DrawFrame(const Ref<Player>& player);

        void Resize();
    private:
        void ClearBuffers();

        void DrawFloors(const Ref<Player>& player);
        void DrawWalls(const Ref<Player>& player);
        void DrawFog(const Ref<Player>& player);

        void DrawWall(const Ref<Player>& player, double xLeft, double xRight, double zDistanceLeft, double zDistanceRight, uint32_t tidx);

        double& GetZBuffer(uint32_t x, uint32_t y)
        {
            return m_ZBuffer[x + y * m_Wnd->GetWidthInt()];
        }

        double& GetZBufferWall(uint32_t x)
        {
            return m_ZBufferWall[x];
        }
    private:
        Ref<Wnd> m_Wnd;
        double* m_ZBuffer;
        double* m_ZBufferWall;

        double xCam, yCam, zCam;
        uint32_t* m_Sky;
    };
}