#ifndef __UITHEME_H__
#define __UITHEME_H__

#include <airkit/GUI/Theme/UIStyle.hpp>
namespace airkit
{

    struct UITheme
    {
        UITheme() = default;
        ~UITheme() = default;

        // 获取样式
        const IUIStyle *getStyle(const std::string &name) const;
        // 获取纹理
        const TextureHolder &getTextures() const { return mSkins.mTextures; }
        // 获取名称
        const std::string &getName() const { return mName; }

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
        friend class UIThemeXmlPaser;
        using Styles = std::map<std::string, UIStyles>;
        Styles mStyles;                 // 风格组
        UISkins mSkins;                 // 皮肤
        std::vector<UIStyles *> mUsing; // 正在使用的样式组
        std::string mName;              // 主题名称
        std::string mSkin;              // 皮肤纹理的文件相对路径
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