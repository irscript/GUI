#ifndef __IFONT_H__
#define __IFONT_H__
#include <airkit/GUI/Render/RenderEnum.hpp>
#include <airkit/Core/Uncopyable.hpp>
#include <airkit/GUI/UI/UIArea.hpp>
namespace airkit
{
    // 字体接口
    struct IFont : public Uncopyable
    {

        // 绘制文本
        virtual void drawText(const UIArea &clip, const std::string &text, const uint32_t flag, const float scale = 1) = 0;
        // 计算文本尺寸
        virtual UISize calcSize(const std::string &text, const float scale = 1) = 0;
    };
}

#endif // __IFONT_H__