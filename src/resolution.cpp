#include "base.h"
#include "resolution.h"

extern uint32_t WndScale;

namespace KtnEm
{
    Resolution::Resolution(uint32_t height, uint32_t scale)
    : m_NewScale(nullptr), m_Height(height) {}

    Resolution::~Resolution()
    {
        Release();
    }

    bool Resolution::Poll(uint32_t* width, uint32_t* height, uint32_t* scale, bool reset)
    {
        if (!m_NewScale) return false;

        if (width) *width = m_Height * 4 / 3;
        if (height) *height = m_Height;
        if (scale) *scale = *m_NewScale;

        if (reset)
            Release();

        return true;
    }

    uint32_t Resolution::GetScale() const
    {
        return WndScale;
    }

    void Resolution::SetScale(uint32_t scale)
    {
        CreateScale(scale);
    }

    void Resolution::SetHeight(uint32_t height)
    {
        m_Height = height;
        SetScale(GetScale());
    }

    void Resolution::Release()
    {
        if (m_NewScale)
        {
            delete m_NewScale;
            m_NewScale = nullptr;
        }
    }

    void Resolution::CreateScale(uint32_t scale)
    {
        Release();

        m_NewScale = new uint32_t(scale);
    }
}