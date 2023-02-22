#include "base.h"
#include "wnd.h"

uint32_t WndScale = 8;

namespace KtnEm
{
    Wnd::Wnd(uint32_t width, uint32_t height)
        : m_Width(width / WndScale), m_Height(height / WndScale), m_WidthFlush(width / WndScale * WndScale), m_HeightFlush(height / WndScale * WndScale), m_Data(nullptr), m_DataFlush(nullptr)
    {
        m_Data = new uint32_t[m_Width * m_Height];
        m_DataFlush = new uint32_t[m_WidthFlush * m_HeightFlush];
    }

    Wnd::~Wnd()
    {
        delete[] m_Data;
        delete[] m_DataFlush;
    }

    void Wnd::Flush() const
    {
        if (!m_FlushFn) return;

        for (uint32_t y = 0; y < m_HeightFlush; y++)
        {
            uint32_t yr = y / WndScale;
            for (uint32_t x = 0; x < m_WidthFlush; x++)
            {
                uint32_t xr = x / WndScale;

                m_DataFlush[x + y * m_WidthFlush] = m_Data[xr + yr * m_Width];
            }
        }

        m_FlushFn(m_WidthFlush, m_HeightFlush, (const void*)m_DataFlush);
    }
}