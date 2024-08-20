#ifndef __UICOLOR_H__
#define __UICOLOR_H__
#include <airkit/AIR_PCH.hpp>
namespace airkit
{

    struct RGBA
    {
        RGBA(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0)
            : mRGBA(r | (g << 8) | (b << 16) | (a << 24)) {}
        RGBA(uint32_t rgba) : mRGBA(rgba) {}
        RGBA(int32_t rgba) : mRGBA(rgba) {}
        RGBA(const RGBA &rgba) : mRGBA(rgba.mRGBA) {}
        RGBA &operator=(uint32_t rgba)
        {
            mRGBA = rgba;
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
    };
}
#endif // __UICOLOR_H__