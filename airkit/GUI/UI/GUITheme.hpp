#ifndef __GUITHEME_H__
#define __GUITHEME_H__

#include <airkit/AIR_PCH.hpp>
#include <airkit/GUI/UI/UIColor.hpp>
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

    // UI风格描述项
    struct UIStyle
    {
        std::string mName; // 对应的UI组件名称

        uint32_t mFlag; // 样式标志:用于是否使用边框、背景等信息

        UIBDI mBDI; // 边框描述
        UIBGI mBGI; // 背景描述
        UIFNT mFNT; // 字体描述

        UIMargin mMargin;   // 外边距
        UIPadding mPadding; // 内边距

        enum : uint32_t
        {
            use_bg = 1 << 0,     // 使用背景色
            use_border = 1 << 1, // 使用边框色
            use_font = 1 << 2,   // 使用字体色
            use_margin = 1 << 3  // 使用外边距
        };
    };

    // UI 风格类组：类似 CSS 的选择器
    struct UIStyles
    {
        std::string mName;                      // 组名称
        std::map<std::string, UIStyle> mStyles; // 组内样式
        const UIStyle *getStyle(const std::string &name) const
        {
            auto iter = mStyles.find(name);
            if (iter != mStyles.end())
                return &iter->second;
            return nullptr;
        }
    };

    struct UITheme
    {
        UITheme() = default;
        ~UITheme() = default;

        // 加载主题文件
        bool load(const std::string &path);
        // 获取样式
        const UIStyle *getStyle(const std::string &name) const;
        // 获取纹理
        const TextureHolder &getTextures() const { return mSkins.mTextures; }

    protected:
        friend class UIStyleSelector;
        // 选择一个样式组
        const UIStyles *select(const std::string &name)
        {
            auto iter = mStyles.find(name);
            if (iter == mStyles.end())
                return nullptr;
            auto result = &iter->second;
            mUsing.push_back(result);
            return result;
        }
        // 取消选择一个样式组
        void unselect(const UIStyles *group)
        {
            if (mUsing.empty() == true)
                return;
            if (mUsing.back() != group)
                return;
            mUsing.pop_back();
        }

    protected:
        using Styles = std::map<std::string, UIStyles>;
        Styles mStyles;                 // 风格组
        UISkins mSkins;                 // 皮肤
        std::vector<UIStyles *> mUsing; // 正在使用的样式组
    };
    // 样式组选择器
    struct UIStyleSelector
    {
        UIStyleSelector(UITheme &theme, const std::string &group) : theme(theme)
        {
            styles = theme.select(group);
        }
        ~UIStyleSelector()
        {
            if (styles != nullptr)
                theme.unselect(styles);
        }

    private:
        const UIStyles *styles;
        UITheme &theme;
    };

}

#endif // __GUITHEME_H__