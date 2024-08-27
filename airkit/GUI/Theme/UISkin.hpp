#ifndef __UISKIN_H__
#define __UISKIN_H__

#include <airkit/AIR_PCH.hpp>
#include <airkit/GUI/Render/ITexture.hpp>
namespace airkit
{
    // 皮肤项
    struct UISkin
    {
        uint32_t id; // 从0开始编号，不得是-1
        float top;
        float bottom;
        float left;
        float right;
    };
    struct UISkins
    {
        std::map<uint32_t, UISkin> mSkins;
        TextureHolder mTextures;
    };
}
#endif // __UISKIN_H__