#ifndef __UITHEMELOADER_H__
#define __UITHEMELOADER_H__
#include <airkit/GUI/Theme/UITheme.hpp>
#include <airkit/3Part/pugixml/pugixml.hpp>
namespace airkit
{
    // 主题XML文件解析器
    struct UIThemeXmlPaser
    {
        // 解析器：每个组件向该解析器注册自己的解析函数
        using Domain = IUIStyle *(*)(pugi::xml_node &node);

        void registerDomain(const std::string &name, Domain domain)
        {
            mDomains[name] = domain;
        }
        // 加载主题文件
        bool parse(const std::string &path, UITheme &theme);

    protected:
        std::map<std::string, Domain> mDomains;
        std::string mError; // 错误信息
    };
}
#endif // __UITHEMELOADER_H__