#ifndef __UICOLOR_H__
#define __UICOLOR_H__
#include <airkit/AIR_PCH.hpp>
namespace airkit
{

    struct RGBA
    {
        RGBA(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255)
            : mRGBA(r | (g << 8) | (b << 16) | (a << 24)) {}

        // 0xRRGGBB
        RGBA(uint32_t rgb, uint32_t a = 255)
        {
            mR = (rgb >> 16) & 0xFF;
            mG = (rgb >> 8) & 0xFF;
            mB = rgb & 0xFF;
            mA = a;
        }
        RGBA(int32_t rgb, uint32_t a = 255)
        {
            mR = (rgb >> 16) & 0xFF;
            mG = (rgb >> 8) & 0xFF;
            mB = rgb & 0xFF;
            mA = a;
        }
        RGBA(const RGBA &rgba) : mRGBA(rgba.mRGBA) {}

        // 0xRRGGBB
        RGBA &operator=(uint32_t rgb)
        {
            mR = (rgb >> 16) & 0xFF;
            mG = (rgb >> 8) & 0xFF;
            mB = rgb & 0xFF;
            mA = 255;
            return *this;
        }
        RGBA &operator=(const RGBA &rgba)
        {
            mRGBA = rgba.mRGBA;
            return *this;
        }

        union
        {
            uint32_t mRGBA;
            struct
            {
                uint8_t mR;
                uint8_t mG;
                uint8_t mB;
                uint8_t mA; // alpha：不透明度，透明为0，不透明为255
            };
        };

        // 0xRRGGBB
        uint32_t toRGB() const
        {
            uint32_t hex = mB | (mG << 8) | (mR << 16);
            return hex;
        }
        // 0xRRGGBBAA
        uint32_t toRGBA() const
        {
            uint32_t hex = mA | (mB << 8) | (mG << 16) | (mR << 24);
            return hex;
        }
        // 0xRRGGBBAA
        static RGBA fromRGBA(uint32_t rgba = -1)
        {
            uint8_t r = (rgba >> 24) & 0xFF;
            uint8_t g = (rgba >> 16) & 0xFF;
            uint8_t b = (rgba >> 8) & 0xFF;
            uint8_t a = rgba & 0xFF;
            return RGBA(r, g, b, a);
        }
        // 0xRRGGBB
        static RGBA fromRGB(uint32_t rgb = -1)
        {
            uint8_t r = (rgb >> 16) & 0xFF;
            uint8_t g = (rgb >> 8) & 0xFF;
            uint8_t b = rgb & 0xFF;
            return RGBA(r, g, b, 1);
        }
    };
}
#endif // __UICOLOR_H__