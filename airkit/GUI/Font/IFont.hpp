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

        
        /*
                // 获取字体元数据
                virtual const IFontMeta &getMeta() const = 0;

                // 获取字体图元
                virtual const IGlyph &getGlyphs(uint32_t code) const = 0;
                */
    };

    using FontHolder = Holder<IFont>;

    // 位图字体接口
    struct IBmpFont : public IFont
    {

    protected:
    };
    // 矢量字体接口
    struct IVecFont : public IFont
    {
    };
}

#endif // __IFONT_H__