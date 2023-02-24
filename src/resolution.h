#pragma once

namespace KtnEm
{
    class Resolution
    {
    public:
        Resolution(uint32_t height, uint32_t scale);
        ~Resolution();

        bool Poll(uint32_t* width, uint32_t* height, uint32_t* scale, bool reset = true);

        uint32_t GetScale() const;
        uint32_t GetHeight() const { return m_Height; }
        std::array<uint32_t, 2> GetSize() const { return { m_Height * 4 / 3, m_Height }; }

        void SetScale(uint32_t scale);
        void SetHeight(uint32_t height);
    private:
        void Release();
        void CreateScale(uint32_t scale);
    private:
        uint32_t *m_NewScale;
        uint32_t m_Height;
    };
}