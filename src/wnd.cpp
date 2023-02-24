#include "base.h"
#include "wnd.h"
#include "util.h"

uint32_t WndScale = 8;

namespace KtnEm
{
    Wnd::Wnd(uint32_t width, uint32_t height)
        : m_Width(0), m_Height(0), m_CanvasWidth(0), m_CanvasHeight(0), m_Data(nullptr), m_CanvasData(nullptr)
    {
        Resize(width, height);
    }

    Wnd::~Wnd()
    {
        Release();
    }

    void Wnd::Resize(uint32_t width, uint32_t height)
    {
        Release();

        m_Width = width / WndScale;
        m_Height = height / WndScale;
        m_CanvasWidth = m_Width * WndScale;
        m_CanvasHeight = m_Height * WndScale;

        m_Data = new uint32_t[m_Width * m_Height];
        m_CanvasData = new uint32_t[m_CanvasWidth * m_CanvasHeight];

        Utils::Fill<uint32_t>(m_Data, 0, m_Width * m_Height);
        Utils::Fill<uint32_t>(m_CanvasData, 0, m_CanvasWidth * m_CanvasHeight);

        if (m_ResizeFn)
            m_ResizeFn(m_CanvasWidth, m_CanvasHeight);
    }

    void Wnd::Release()
    {
        if (m_Data)
        {
            delete[] m_Data;
            m_Data = nullptr;
        }

        if (m_CanvasData)
        {
            delete[] m_CanvasData;
            m_CanvasData = nullptr;
        }

        m_Width = 0;
        m_Height = 0;
        m_CanvasWidth = 0;
        m_CanvasHeight = 0;
    }

    void Wnd::Flush() const
    {
        if (!m_FlushFn) return;

        for (uint32_t y = 0; y < m_CanvasHeight; y++)
        {
            uint32_t yr = y / WndScale;
            for (uint32_t x = 0; x < m_CanvasWidth; x++)
            {
                uint32_t xr = x / WndScale;

                m_CanvasData[x + y * m_CanvasWidth] = m_Data[xr + yr * m_Width];
            }
        }

        m_FlushFn(m_CanvasWidth, m_CanvasHeight, (const void*)m_CanvasData);
    }
}