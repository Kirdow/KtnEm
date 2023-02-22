#pragma once

namespace KtnEm
{
    typedef void(*WndFlushFn)(uint32_t, uint32_t, const void*);

    class Wnd
    {
    public:
        Wnd(uint32_t width, uint32_t height);
        ~Wnd();

        template<typename T>
        T GetWidth() const { return (T)m_Width; }
        template<typename T>
        T GetHeight() const { return (T)m_Height; }

        int32_t GetWidthInt() const { return m_Width; }
        int32_t GetHeightInt() const { return m_Height; }

        int32_t GetCanvasWidth() const { return m_WidthFlush; }
        int32_t GetCanvasHeight() const { return m_HeightFlush; }

        const void* GetData() const { return m_Data; }

        void Flush() const;

        uint32_t& At(uint32_t x, uint32_t y)
        {
            return m_Data[x + y * m_Width];
        }

        uint32_t At(uint32_t x, uint32_t y) const
        {
            return m_Data[x + y * m_Width];
        }

        uint32_t& At(uint32_t i)
        {
            return m_Data[i];
        }

        uint32_t At(uint32_t i) const
        {
            return m_Data[i];
        }

        void SetFlush(WndFlushFn fn)
        {
            m_FlushFn = fn;
        }

        
    private:
        uint32_t m_Width, m_Height;
        uint32_t m_WidthFlush, m_HeightFlush;
        uint32_t *m_Data;
        uint32_t *m_DataFlush;
        WndFlushFn m_FlushFn = nullptr;
    };
}