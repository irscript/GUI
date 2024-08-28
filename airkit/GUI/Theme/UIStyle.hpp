#ifndef __UISTYLE_H__
#define __UISTYLE_H__

#include <airkit/GUI/Theme/UISkin.hpp>
#include <airkit/GUI/UI/UIColor.hpp>
namespace airkit
{

    // UI风格描述项基类
    struct IUIStyle
    {
        IUIStyle(const std::string &name) : mName(name) {}
        virtual ~IUIStyle() = default;

        std::string mName; // 对应的UI组件名称

        template <typename T>
        const T &as() const { return *(T *)(this); }
        template <typename T>
        T &as() { return *(T *)(this); }
    };

    using UIStyleScope = Scope<IUIStyle>;

    // UI 风格类组：类似 CSS 的选择器
    struct UIStyles
    {
        std::string mName;                           // 组名称
        std::map<std::string, UIStyleScope> mStyles; // 组内样式
        const IUIStyle *getStyle(const std::string &name) const
        {
            auto iter = mStyles.find(name);
            if (iter != mStyles.end())
                return iter->second.get();
            return nullptr;
        }
    };

    struct UIStyleFlag
    {
        enum : uint32_t
        {
            use_bg = 1 << 0,     // 使用背景色
            use_border = 1 << 1, // 使用边框色
            use_font = 1 << 2,   // 使用字体色
            use_margin = 1 << 3, // 使用外边距

        };
    };
    // UI 外边距描述项
    struct UIMargin
    {
        float left = 0;
        float right = 0;
        float top = 0;
        float bottom = 0;
    };
    // UI 内边距
    using UIPadding = UIMargin;
    // UI颜色样式
    struct UIClr
    {
        RGBA lt; // 左上
        RGBA rt; // 右上
        RGBA lb; // 左下
        RGBA rb; // 右下
    };
    // UI 边框宽度
    using UIBorderWidth = UIMargin;
    // UI 边框圆角半径
    struct UIBorderRadius
    {
        float lt = 0; // 左上
        float rt = 0; // 右上
        float lb = 0; // 左下
        float rb = 0; // 右下
    };
    // UI边框描述项
    struct UIBDI
    {
        // 边框宽度
        UIBorderWidth width;

        // 边框颜色
        UIClr color;

        // 边框圆角半径
        UIBorderRadius round;
    };

    // UI背景描述项
    struct UIBGI
    {
        UIClr color; // 背景颜色
        union
        {
            uint32_t id;                  // 皮肤id
            const UISkin *skin = nullptr; // 皮肤
        };
    };
    // UI字体描述项
    struct UIFNT
    {
        float size = 32;
        UIClr color;
    };

}

#endif // __UISTYLE_H__