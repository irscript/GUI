#include <airkit/GUI/Theme/UIThemeLoader.hpp>
#include <airkit/GUI/Widgets/Button.hpp>
namespace airkit::xmldomain
{
    IUIStyle *button(pugi::xml_node &node)
    {
        auto style = new Button::Style();
        do
        {
            bool error = false;
            for (auto component = node.first_child();
                 component;
                 component = component.next_sibling())
            {
                std::string name = component.name();
                if (name == "margin")
                {
                    continue;
                }
            }
            if (error == false)
                return style;
        } while (0);
        delete style;
        return nullptr;
    }
}
namespace airkit
{
    bool UIThemeXmlPaser::parse(const std::string &path, UITheme &theme)
    {
        // 先加载主题文件xml
        pugi::xml_document doc;
        auto result = doc.load_file(path.c_str());
        if (result.status != pugi::status_ok)
        {
            mError = "load theme file failed";
            return false;
        }

        // 找到主题的根节点
        pugi::xml_node root = doc.child("theme");
        if (root == nullptr)
        {
            mError = "theme file format error";
            return false;
        }
        // 遍历组
        for (auto group = root.child("group"); group; group = group.next_sibling("group"))
        {
            // 获取组名
            std::string groupName = group.attribute("name").value();
            if (groupName.empty())
            {
                mError = "group name is empty";
                return false;
            }
            auto &gth = theme.mStyles[groupName];
            gth.mName = groupName;
            for (auto component = group.first_child(); component; component = component.next_sibling())
            {
                auto name = component.name();
                auto result = mDomains.find(name);
                if (result == mDomains.end())
                {
                    mError = name;
                    mError += " not support in ";
                    mError += groupName;
                    return false;
                }
                auto domain = result->second;
                auto style = domain(component);
                if (style == nullptr)
                {
                    mError = name;
                    mError += " parse failed in";
                    mError += groupName;
                    return false;
                }
                gth.mStyles.insert(std::make_pair(style->mName, style));
            }
        }
        return true;
    }

#define REGISTER_DOMAIN(domain) \
    mDomains.insert(std::make_pair(#domain, xmldomain::##domain))
    UIThemeXmlPaser::UIThemeXmlPaser()
    {
        REGISTER_DOMAIN(button);
    }
#undef REGISTER_DOMAIN
}
