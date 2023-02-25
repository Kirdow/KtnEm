#include "base.h"
#include "player.h"

#include "level.h"
#include "input.h"

namespace KtnEm
{
    static bool CanMoveTo(glm::vec2 pos);
    static bool CanMoveTo2(glm::vec2 pos);

    Player::Player()
        : m_Pos(0.0f, 0.0f), m_Rot(0.0f)
    {
        Input::SetMoveSpeed(2.5f);
        Input::SetTurnSpeed(3.141592f);

        Calculate();
    }

    Player::~Player()
    {
    }

    void Player::Tick(float dt)
    {
        // Fetch input
        InputAxis input = Input::GetInputAxis(dt);

        // Rotation
        m_Rot += input.Turn;

        // Calculate cos/sin
        Calculate();

        // Position
        if (input.IsMoving())
            Move(m_Mat * input.Move);
    }

    void Player::Frame()
    {
    }

    void Player::Calculate()
    {
        m_Cos = cosf(m_Rot);
        m_Sin = sinf(m_Rot);
        m_Mat = glm::mat2(glm::vec2(m_Cos, -m_Sin), glm::vec2(m_Sin, m_Cos));
    }

    void Player::Move(glm::vec2 delta)
    {
        Move2({ delta.x, 0.0f });
        Move2({ 0.0f, delta.y });
    }

    void Player::Move2(glm::vec2 delta)
    {
        if (CanMoveTo(m_Pos + delta)) m_Pos += delta;
    }

    void Player::ValidateSpawn()
    {
        while (!CanMoveTo(m_Pos)) m_Pos.x++;
    }

    static bool CanMoveTo(glm::vec2 pos)
    {
        float rad = 0.25f;

        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (!CanMoveTo2(pos + glm::vec2(i, j) * rad)) return false;
            }
        }

        return true;
    }

    static bool CanMoveTo2(glm::vec2 pos)
    {
        return !KtnEm::Level::IsCollision((int32_t)floorf(pos.x + 0.5f), (int32_t)floorf(pos.y + 0.5f));
    }


}