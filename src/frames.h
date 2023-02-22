#pragma once

#include "timer.h"
#include <cstdint>
#include <vector>

namespace KtnEm
{
    typedef void(*FramerateTickFn)(uint32_t);

    class Framerater
    {
    public:
        Framerater() = default;
        void Frame()
        {
            ++m_Frames;
            if (m_Timer.Elapsed() >= 1.0f)
            {
                m_FPS = m_Frames;
                m_Frames = 0;
                m_Timer.Reset();

                Emit();
            }
        }

        inline uint32_t GetFPS() const { return m_FPS; }

        void AddCallback(FramerateTickFn fn)
        {
            m_Callbacks.push_back(fn);
        }

        float NextDelta()
        {
            m_Delta = m_DeltaTimer.Elapsed();
            m_DeltaTimer.Reset();
            return m_Delta;
        }

        float GetDelta() const { return m_Delta; }
    private:
        void Emit()
        {
            for (auto fn : m_Callbacks)
            {
                fn(m_FPS);
            }
        }
    private:
        uint32_t m_Frames = 0;
        uint32_t m_FPS = 0;
        float m_Delta = 0.0f;
        Timer m_Timer;
        Timer m_DeltaTimer;
        std::vector<FramerateTickFn> m_Callbacks;
    };
}