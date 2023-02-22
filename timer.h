#pragma once

#include <chrono>
#include <iostream>

namespace KtnEm
{
    class Timer
    {
    public:
        Timer()
        {
            Reset();
        }

        void Reset()
        {
            m_Start = std::chrono::high_resolution_clock::now();
        }

        float Elapsed()
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
        }

        float ElapsedMillis()
        {
            return Elapsed() * 1000.0f;
        }
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
    };

    class ScopedTimer
    {
    public:
        ScopedTimer(const std::string& name = "")
            : m_Name(name) {}
        ~ScopedTimer()
        {
            Log();
        }

        void Log()
        {
            if (m_Name.empty())
                std::cout << "ScopedTimer: ";
            else
                std::cout << "ScopedTimer[" << m_Name << "]: ";

            std::cout << m_Timer.ElapsedMillis() << "ms\n";
        }
    private:
        std::string m_Name;
        Timer m_Timer;
    };
}