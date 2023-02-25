#pragma once

#include <math.h>

#include <glm/glm.hpp>

namespace KtnEm
{
    class Player
    {
    public:
        Player();
        ~Player();

        void Tick(float dt);
        void Frame();

        glm::vec2& GetPosition() { return m_Pos; }
        const glm::vec2& GetPosition() const { return m_Pos; }

        float GetRotation() const { return m_Rot; }
        void SetRotation(float rot) { m_Rot = rot; }

        float GetCosine() const { return m_Cos; }
        float GetSine() const { return m_Sin; }

        void Calculate();

        void ValidateSpawn();
    private:
        void Move(glm::vec2 delta);
        void Move2(glm::vec2 delta);

    private:
        glm::vec2 m_Pos;
        glm::mat2 m_Mat;
        float m_Rot;
        float m_Cos;
        float m_Sin;
    };
}