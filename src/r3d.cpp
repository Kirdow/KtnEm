#include "base.h"
#include "r3d.h"
#include "noise.h"
#include "level.h"
#include "mathhelp.h"

template<typename T>
constexpr T absT(T value) noexcept
{
    return value < 0 ? -value : value;
}

template<typename T>
constexpr T clampT(T value, T min, T max) noexcept
{
    return value < min ? min : (value > max ? max : value);
}

constexpr uint32_t maskcol(uint32_t col, double factor) noexcept
{
    double r = ((col >> 16) & 0xFF) / 255.0 * factor;
    double g = ((col >> 8) & 0xFF) / 255.0 * factor;
    double b = ((col) & 0xFF) / 255.0 * factor;

    uint32_t ir = clampT<uint32_t>((uint32_t)(r * 255.0), 0, 255);
    uint32_t ig = clampT<uint32_t>((uint32_t)(g * 255.0), 0, 255);
    uint32_t ib = clampT<uint32_t>((uint32_t)(b * 255.0), 0, 255);

    return 0xFF << 24 | ir << 16 | ig << 8 | ib;
}

constexpr uint32_t makeargb(uint32_t gray, uint32_t mask = 0x00FFFFFF) noexcept
{
    uint32_t mr = (mask >> 16) & 0xFF;
    uint32_t mg = (mask >> 8) & 0xFF;
    uint32_t mb = (mask) & 0xFF;

    uint32_t r = gray * mr / 255;
    uint32_t g = gray * mg / 255;
    uint32_t b = gray * mb / 255;

    return 0xFF << 24 | r << 16 | g << 8 | b;
}

constexpr uint32_t getargb(int32_t x, int32_t y, uint32_t mask = 0x00FFFFFF) noexcept
{
    uint32_t a = static_cast<uint32_t>(x & 7);
    uint32_t b = static_cast<uint32_t>(y & 7);

    uint32_t c = ((a ^ b ^ ((a + b) % 8)) != 0 ? (16 - a - b) : (a + b + 1)) * 127 / 16 + 128;
    //uint32_t c = ((a == 0 || b == 0) ? 0 : ((a == 7 || b == 7) ? 16 : (a + b + 1))) * 255 / 16;

    return makeargb(c & 0xFF, mask);
}

namespace KtnEm
{
    R3D::R3D(const Ref<Wnd>& wnd) : m_Wnd(wnd)
    {
        m_ZBuffer = new double[wnd->GetWidthInt() * wnd->GetHeightInt()];
        m_ZBufferWall = new double[wnd->GetWidthInt()];
        m_Sky = new uint32_t[64 * 64];

        uint32_t* sky = new uint32_t[64 * 64];
        for (uint32_t x = 0; x < 64; x++)
        {
            for (uint32_t y = 0; y < 64; y++)
            {
                sky[x + y * 64] = maskcol(0xFF9400, (float)Noise::ValueNoise_2D(x * 64, y * 64) * 0.5f + 0.5f);
            }
        }

        #define POSU(x) (((x) + 64) % 64)
        #define POS(x, y) (POSU(x)), (POSU(y))

        #define PRAD 2

        for (int32_t x = 0; x < 64; x++)
        {
            for (int32_t y = 0; y < 64; y++)
            {
                int32_t count = 0;
                double red = 0.0;
                double green = 0.0;
                double blue = 0.0;

                for (int32_t j = -PRAD; j <= PRAD; j++)
                {
                    int32_t jp = POSU(x + j);

                    for (int32_t k = -PRAD; k <= PRAD; k++)
                    {
                        int kp = POSU(y + k);

                        uint32_t col = sky[jp + kp * 64];

                        red += ((col >> 16) & 0xFF) / 255.0;
                        green += ((col >> 8) & 0xFF) / 255.0;
                        blue += ((col) & 0xFF) / 255.0;
                        count++;
                    }
                }

                red /= count;
                green /= count;
                blue /= count;

                uint32_t c = 0xFF << 24 | (uint32_t)(red * 255.0) << 16 | (uint32_t)(green * 255.0) << 8 | (uint32_t)(blue * 255.0);

                m_Sky[x + y * 64] = c;
            }
        }
        delete[] sky;
    }

    R3D::~R3D()
    {
        delete[] m_ZBuffer;
        delete[] m_ZBufferWall;
        delete[] m_Sky;
    }

    void R3D::DrawFrame(EPos& pos)
    {
        ClearBuffers();

        pos.Calculate();

        xCam = pos.x - pos.sine * 0.3;
        yCam = pos.y - pos.cosine * 0.3;

        DrawWalls(pos);
        DrawFloors(pos);
        DrawFog(pos);
    }

    void R3D::DrawFloors(EPos& pos)
    {
        uint32_t h = m_Wnd->GetHeightInt();
        uint32_t w = m_Wnd->GetWidthInt();
        double h2 = h / 2.0;
        double w2 = w / 2.0;
        double h1 = 1.0 / h;

        for (int32_t y = 0; y < h; y++)
        {
            double ceiling = (y - h2) * h1;

            double span = 2.0 - 1.0 / 7.0;
            double z = span / ceiling;
            bool floor = true;
            if (ceiling < 0)
            {
                floor = false;
                z = span / -ceiling;
            }

            for (int32_t x = 0; x < w; x++)
            {
                if (GetZBuffer(x, y) <= z) continue;
                double depth = (x - w2) * h1 * z;

                double xx = depth * pos.cosine + z * pos.sine + (xCam + 0.5) * 4;
                double yy = z * pos.cosine - depth * pos.sine + (yCam + 0.5) * 4;

                int32_t xPix = (int32_t)(floorf(xx * 2));
                int32_t yPix = (int32_t)(floorf(yy * 2));

                int32_t xTile = xPix >> 3;
                int32_t yTile = yPix >> 3;
                uint32_t mask = Level::GetMask(xTile, yTile);

                if (!floor) {
                    GetZBuffer(x, y) = -1;
                } else {
                    GetZBuffer(x, y) = z;
                    m_Wnd->At(x, y) = getargb(xPix, yPix, mask);
                }
            }
        }
    }

    static uint32_t s_Walls[2] = {
        0xCCCCCC,
        0x1C3151
    };

    void R3D::DrawWalls(EPos& pos)
    {
        int32_t tx = (int32_t)floorf(xCam);
        int32_t ty = (int32_t)floorf(yCam);

        for (int32_t x = tx - 10; x <= tx + 10; x++)
        {
            //if (x < 0) continue;
            for (int32_t y = ty - 10; y <= ty + 10; y++)
            {
                //if (y < 0) continue;
                auto self = Level::IsWall(x, y);
                auto east = Level::IsWall(x + 1, y);
                auto south = Level::IsWall(x, y + 1);

                if (self)
                {
                    if (!east)
                    {
                        DrawWall(pos, x + 1, x + 1, y, y + 1, self - 1);
                    }
                    if (!south)
                    {
                        DrawWall(pos, x + 1, x, y + 1, y + 1, self - 1);
                    }
                }
                else
                {
                    if (east)
                    {
                        DrawWall(pos, x + 1, x + 1, y + 1, y, east - 1);
                    }
                    if (south)
                    {
                        DrawWall(pos, x, x + 1, y + 1, y + 1, south - 1);
                    }
                }
            }
        }
    }

    void R3D::DrawWall(EPos& pos, double xLeft, double xRight, double zDistanceLeft, double zDistanceRight, uint32_t tidx)
    {
        double xcLeft = ((xLeft - 0.5) - xCam) * 2.0;
        double zcLeft = ((zDistanceLeft - 0.5) - yCam) * 2.0;

        double rotLeftSideX = xcLeft * pos.cosine - zcLeft * pos.sine;
        double yCornerTL = ((-0.5)) * 2.0;
        double yCornerBL = ((0.5)) * 2.0;
        double rotLeftSideZ = zcLeft * pos.cosine + xcLeft * pos.sine;

        double xcRight = ((xRight - 0.5) - xCam) * 2.0;
        double zcRight = ((zDistanceRight - 0.5) - yCam) * 2.0;

        double rotRightSideX = xcRight * pos.cosine - zcRight * pos.sine;
        double yCornerTR = ((-0.5)) * 2.0;
        double yCornerBR = ((0.5)) * 2.0;
        double rotRightSideZ = zcRight * pos.cosine + xcRight * pos.sine;

        double tex30 = 0;
        double tex40 = 8;
        double clip = 0.2;

        if (rotLeftSideZ < clip && rotRightSideZ < clip) {
            return;
        }

        if (rotLeftSideZ < clip) {
            double clip0 = (clip - rotLeftSideZ) / (rotRightSideZ - rotLeftSideZ);
            rotLeftSideZ = rotLeftSideZ + (rotRightSideZ - rotLeftSideZ) * clip0;
            rotLeftSideX = rotLeftSideX + (rotRightSideX - rotLeftSideX) * clip0;
            tex30 = tex30 + (tex40 - tex30) * clip0;
        }

        if (rotRightSideZ < clip) {
            double clip0 = (clip - rotLeftSideZ) / (rotRightSideZ - rotLeftSideZ);
            rotRightSideZ = rotLeftSideZ + (rotRightSideZ - rotLeftSideZ) * clip0;
            rotRightSideX = rotLeftSideX + (rotRightSideX - rotLeftSideX) * clip0;
            tex40 = tex30 + (tex40 - tex30) * clip0;
        }

        double xPixelLeft = (rotLeftSideX / rotLeftSideZ * m_Wnd->GetHeight<double>() + m_Wnd->GetWidth<double>() / 2.0);
        double xPixelRight = (rotRightSideX / rotRightSideZ * m_Wnd->GetHeight<double>() + m_Wnd->GetWidth<double>() / 2.0);

        if (xPixelLeft >= xPixelRight) {
            return;
        }

        int32_t xPixelLeftInt = (int32_t)xPixelLeft;
        int32_t xPixelRightInt = (int32_t)xPixelRight;

        if (xPixelLeftInt < 0) xPixelLeftInt = 0;
        if (xPixelRightInt > m_Wnd->GetWidthInt()) xPixelRightInt = m_Wnd->GetWidthInt();

        double yPixelLeftTop = (int32_t)(yCornerTL / rotLeftSideZ * m_Wnd->GetHeight<double>() + m_Wnd->GetHeight<double>() / 2.0);
        double yPixelLeftBottom = (int32_t)(yCornerBL / rotLeftSideZ * m_Wnd->GetHeight<double>() + m_Wnd->GetHeight<double>() / 2.0);
        double yPixelRightTop = (int32_t)(yCornerTR / rotRightSideZ * m_Wnd->GetHeight<double>() + m_Wnd->GetHeight<double>() / 2.0);
        double yPixelRightBottom = (int32_t)(yCornerBR / rotRightSideZ * m_Wnd->GetHeight<double>() + m_Wnd->GetHeight<double>() / 2.0);

        double tex1 = 1.0 / rotLeftSideZ;
        double tex2 = 1.0 / rotRightSideZ;
        double tex3 = tex30 / rotLeftSideZ;
        double tex4 = tex40 / rotRightSideZ - tex3;

        for (int32_t x = xPixelLeftInt; x < xPixelRightInt; x++)
        {
            double pixelRotation = (x - xPixelLeft) / (xPixelRight - xPixelLeft);
            double zWall = (tex1 + (tex2 - tex1) * pixelRotation);

            int32_t xTexture = (int32_t)((tex3 + tex4 * pixelRotation) / zWall);

            if (GetZBufferWall(x) > zWall) {
                continue;
            }

            GetZBufferWall(x) = zWall;
        
            double yPixelTop = yPixelLeftTop + (yPixelRightTop - yPixelLeftTop) * pixelRotation - 0.5;
            double yPixelBottom = yPixelLeftBottom + (yPixelRightBottom - yPixelLeftBottom) * pixelRotation;

            int32_t yPixelTopInt = (int32_t)ceil(yPixelTop);
            int32_t yPixelBottomInt = (int32_t)ceil(yPixelBottom);

            if (yPixelTopInt < 0) yPixelTopInt = 0;
            if (yPixelBottomInt > m_Wnd->GetHeightInt()) yPixelBottomInt = m_Wnd->GetHeightInt();

            for (int32_t y = yPixelTopInt; y < yPixelBottomInt; y++)
            {
                double pixelRotationY = (y - yPixelTop) / (yPixelBottom - yPixelTop);
                int32_t yTexture = (int32_t)(8 * pixelRotationY);

                m_Wnd->At(x, y) = getargb(xTexture, yTexture, s_Walls[tidx]);
                GetZBuffer(x, y) = 1.0 / zWall * 2.0;
            }
        }
    }

    void R3D::DrawFog(EPos& pos)
    {
        auto s = m_Wnd->GetWidthInt() * m_Wnd->GetHeightInt();

        const double DEL = 24.0;
        const double SCALE = 6.0;
        int32_t w = m_Wnd->GetWidthInt();
        int32_t h = m_Wnd->GetHeightInt();
        for (uint32_t i = 0; i < s; i++)
        {
            double z = m_ZBuffer[i];
            if (z < 0)
            {
                int32_t xx = ((int) floorf((i % w) / 8 + pos.rot * 64 / (3.141592f * 2.0f) * 8)) & 63;
                int32_t yy = i / w;
                double grad = 0.2 - (double)yy / (double)h * 0.2;
                if (grad < 0.0) grad = 0.0;
                m_Wnd->At(i) = maskcol(m_Sky[xx + (yy % 64) * 64], grad);
            }
            else
            {
                uint32_t color = m_Wnd->At(i);

                int32_t xp = i % w;
                int32_t yp = (i / w) * 14;

                double xx = ((i % w - w / 2.0) / w);
                int32_t brightness = (int32_t)(300 - z * 15 * (xx * xx * 2 + 1));
                brightness = (brightness + ((xp + yp) & 3) * 4) >> 4 << 4;
                if (brightness < 0) brightness = 0;
                if (brightness > 255) brightness = 255;


                //if (z > DEL) z = DEL + (z - DEL) * SCALE;

                //uint32_t brightness = (uint32_t)(1500.0 / z);

                //if (brightness > 255) brightness = 255;

                uint32_t r = (color >> 16) & 0xFF;
                uint32_t g = (color >> 8) & 0xFF;
                uint32_t b = (color) & 0xFF;

                r = r * brightness / 255;
                g = g * brightness / 255;
                b = b * brightness / 255;

                m_Wnd->At(i) = 0xFF << 24 | r << 16 | g << 8 | b;
            }
        }
    }

    void R3D::ClearBuffers()
    {
        uint32_t size = m_Wnd->GetWidthInt() * m_Wnd->GetHeightInt();
        for (uint32_t i = 0; i < size; i++)
        {
            m_ZBuffer[i] = 100000.0;
        }

        for (uint32_t i = 0; i < m_Wnd->GetWidthInt(); i++)
        {
            m_ZBufferWall[i] = 0.0;
        }
    }
}