#ifndef __IFONT_H__
#define __IFONT_H__
#include <airkit/GUI/Render/RenderEnum.hpp>
#include <airkit/Core/Uncopyable.hpp>
#include <airkit/GUI/UI/UIArea.hpp>
#include <airkit/GUI/UI/UIColor.hpp>
#include <airkit/GUI/UI/UIDrawData.hpp>
namespace airkit
{
    // 字体接口
    struct IFont : public Uncopyable
    {

        /// @brief 绘制文本
        /// @param drawlist 绘制列表
        /// @param clip 绘制区域
        /// @param text 要绘制的文本
        /// @param flag 文本绘制样式
        /// @param color 文本颜色
        /// @param size 字体大小：单位像素点，默认值 32
        virtual void drawText(UIDrawList &drawlist, const UIArea &clip,
                              const std::string &text, const uint32_t flag,
                              RGBA color, const float size = 32) = 0;

        /// @brief 计算文本尺寸
        /// @param text 计算的文本
        /// @param size 字体大小
        /// @return 计算后的大小
        virtual UISize calcSize(const std::string &text, const float size = 1) = 0;

        // 字体图元
        struct Glyph
        {
            uint32_t code; // 字符编码
            float advance; // 字符宽度:以字体大小为单位
            struct
            {
                float l, r, t, b;
            } image,   // 纹理坐标
                plane; // 平面坐标: 以字体大小为单位，基于原点偏移,用于提示如何绘制该字符
        };
        // 字体元数据
        struct FontMeta
        {
            float size;               // 字体大小
            float lineHeight;         // 行高: 以字体大小为单位
            float distanceRange;      // 距离场
            float underlineY;         // 下划线位置: 以字体大小为单位，基于原点偏移,
            float underlineThickness; // 下划线厚度: 以字体大小为单位
        };
    };

    using FontHolder = Holder<IFont>;

}

#endif // __IFONT_H__