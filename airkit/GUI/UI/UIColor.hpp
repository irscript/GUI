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
            return RGBA(r, g, b);
        }

        /// @brief 颜色混合
        /// @param x 指定起始点颜色
        /// @param y 指定结束点颜色
        /// @param t 插值因子，取值范围为[0,1]
        /// @return 混合之后的颜色
        static RGBA mix(const RGBA &x, const RGBA &y, float t)
        {
            uint8_t r = (uint8_t)(x.mR + t * (y.mR - x.mR));
            uint8_t g = (uint8_t)(x.mG + t * (y.mG - x.mG));
            uint8_t b = (uint8_t)(x.mB + t * (y.mB - x.mB));
            uint8_t a = (uint8_t)(x.mA + t * (y.mA - x.mA));

            return RGBA(r, g, b, a);
        }
    };
}
#endif // __UICOLOR_H__