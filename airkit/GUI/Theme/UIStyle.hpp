#ifndef __UISTYLE_H__
#define __UISTYLE_H__

#include <airkit/GUI/Theme/UISkin.hpp>
#include <airkit/GUI/UI/UIColor.hpp>
namespace airkit
{

    // UI风格描述项基类
    struct IUIStyle
    {
        IUIStyle(const std::string &name):mName(name){}
        virtual ~IUIStyle() = default;

        std::string mName; // 对应的UI组件名称
                           /*
                                   uint32_t mFlag; // 样式标志:用于是否使用边框、背景等信息
                   
                                   UIBDI mBDI; // 边框描述
                                   UIBGI mBGI; // 背景描述
                                   UIFNT mFNT; // 字体描述
                   
                                   UIMargin mMargin;   // 外边距
                                   UIPadding mPadding; // 内边距
                           */
        enum : uint32_t
        {
            use_bg = 1 << 0,     // 使用背景色
            use_border = 1 << 1, // 使用边框色
            use_font = 1 << 2,   // 使用字体色
            use_margin = 1 << 3, // 使用外边距

        };
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

    // UI边框描述项
    struct UIBDI
    {
        // 边框宽度
        struct
        {
            float left;
            float right;
            float top;
            float bottom;
        } width;

        // 边框颜色
        struct
        {
            RGBA left;
            RGBA right;
            RGBA top;
            RGBA bottom;
        } color;

        // 边框圆角半径
        struct
        {
            float left;
            float right;
            float top;
            float bottom;
        } round;
    };

    // UI背景描述项
    struct UIBGI
    {
        struct
        {
            RGBA left;
            RGBA right;
            RGBA top;
            RGBA bottom;
        } color; // 背景颜色
        union
        {
            uint32_t id;        // 皮肤id
            const UISkin *skin; // 皮肤
        };
    };
    // UI字体描述项
    struct UIFNT
    {
        float size;
        struct
        {
            RGBA left;
            RGBA right;
            RGBA top;
            RGBA bottom;
        } color;
    };
    // UI 外边距描述项
    struct UIMargin
    {
        float left;
        float right;
        float top;
        float bottom;
    };
    // UI 内边距
    struct UIPadding
    {
        float left;
        float right;
        float top;
        float bottom;
    };

}

#endif // __UISTYLE_H__