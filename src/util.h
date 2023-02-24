#pragma once

namespace KtnEm::Utils
{
    template<typename T>
    static void Fill(T* dst, const T& value, size_t count)
    {
        T* end = dst + count;
        T* ptr = dst;
        while (ptr != end)
        {
            *ptr = value;
            ++ptr;
        }
    }
}