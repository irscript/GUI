#ifndef __UISKIN_H__
#define __UISKIN_H__

#include <airkit/AIR_PCH.hpp>
#include <airkit/GUI/Render/ITexture.hpp>
namespace airkit
{
    // 皮肤项
    struct UISkin
    {
        uint32_t id;   // 从1开始编号，不得是0
        uint32_t flag; // 标记纹理的类型：普通纹理、MTSDF纹理
        float top;
        float bottom;
        float left;
        float right;
    };
    struct UISkins
    {
        std::string mName;//皮肤纹理文件名称
        std::map<uint32_t, UISkin> mSkins;
        TextureHolder mTextures;
    };
}
#endif // __UISKIN_H__