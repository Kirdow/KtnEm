#pragma once

#include <cstdint>
#include "noise.h"

namespace KtnEm::Level
{
    enum class LevelTile
    {
        Water = 0,
        Grass,
        Sand,
        Rock
    };

    static LevelTile GetTile(int32_t x, int32_t y)
    {
        float noise = Noise::GetNoiseAt(x, y);
        if (noise < 0.35f) return LevelTile::Rock;
        if (noise < 0.5f) return LevelTile::Sand;
        if (noise < 0.55f) return LevelTile::Water;
        return LevelTile::Grass;
    }

    static uint32_t GetMask(int32_t x, int32_t y)
    {
        auto tile = GetTile(x, y);
        switch (tile)
        {
        case LevelTile::Water: return 0xD35B06;
        case LevelTile::Grass: return 0x37D100;
        case LevelTile::Rock: return 0x515151;
        case LevelTile::Sand: return 0x41DCEA;
        }

        return 0xFFFFFF;
    }

    static uint32_t IsWall(int32_t x, int32_t y)
    {
        float noise = Noise::GetNoiseAt(x, y);
        if (noise < 0.34f) return 1;
        if (noise >= 0.7f) return 2;
        return 0;
    }
    
    static bool IsCollision(int32_t x, int32_t y)
    {
        auto tile = GetTile(x, y);

        switch (tile)
        {
        case LevelTile::Water:
        case LevelTile::Rock:
            return true;
        default:
            break;
        }

        return IsWall(x, y);
    }

}