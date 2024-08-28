#ifndef __XMLDOMAIN_H__
#define __XMLDOMAIN_H__
#include <airkit/3Part/pugixml/pugixml.hpp>
#include <airkit/GUI/Widgets/Button.hpp>
namespace airkit::xmldomain
{
    namespace helper
    {
        // 解析外边距:内边距
        bool getMargin(pugi::xml_node &node, UIMargin &margin)
        {
            for (auto attri = node.first_attribute();
                 attri;
                 attri = attri.next_attribute())
            {
                std::string name = attri.name();
                if (name == "w")
                {
                    auto w = attri.as_float();
                    margin.left = w;
                    margin.right = w;
                    margin.top = w;
                    margin.bottom = w;
                    return true;
                }
                if (name == "l")
                    margin.left = attri.as_float();
                else if (name == "r")
                    margin.right = attri.as_float();
                else if (name == "t")
                    margin.top = attri.as_float();
                else if (name == "b")
                    margin.bottom = attri.as_float();
                else
                    return false;
            }
            return true;
        }
        // 解析内边距
        bool getPadding(pugi::xml_node &node, UIPadding &padding) { return getMargin(node, padding); }
        // 解析边框宽度
        bool getBorderWidth(pugi::xml_node &node, UIBorderWidth &border) { return getMargin(node, border); }
        // 解析边框圆角
        bool getBorderRadius(pugi::xml_node &node, UIBorderRadius &radius)
        {
            for (auto attri = node.first_attribute();
                 attri;
                 attri = attri.next_attribute())
            {
                std::string name = attri.name();
                if (name == "r")
                {
                    auto r = attri.as_float();
                    radius.lt = r;
                    radius.rt = r;
                    radius.rb = r;
                    radius.lb = r;
                    return true;
                }
                if (name == "lt")
                    radius.lt = attri.as_float();
                else if (name == "rt")
                    radius.rt = attri.as_float();
                else if (name == "rb")
                    radius.rb = attri.as_float();
                else if (name == "lb")
                    radius.lb = attri.as_float();
                else
                    return false;
            }
            return true;
        }
        RGBA as_color(const pugi::char_t *str)
        {
            str++; // 过滤#
            uint32_t rgba = strtoul(str, nullptr, 16);
            return RGBA::fromRGBA(rgba);
        }
        // 解析颜色
        bool getColor(pugi::xml_node &node, UIClr &color)
        {
            for (auto attri = node.first_attribute();
                 attri;
                 attri = attri.next_attribute())
            {
                std::string name = attri.name();
                if (name == "r")
                {
                    auto r = as_color(attri.value());
                    color.lt = r;
                    color.rt = r;
                    color.rb = r;
                    color.lb = r;
                    return true;
                }
                if (name == "lt")
                    color.lt = as_color(attri.value());
                else if (name == "rt")
                    color.rt = as_color(attri.value());
                else if (name == "rb")
                    color.rb = as_color(attri.value());
                else if (name == "lb")
                    color.lb = as_color(attri.value());
                else
                    return false;
            }
            return true;
        }

        // 解析背景
        bool getBackground(pugi::xml_node &node, UIBGI &bgi)
        {
            auto skin = node.attribute("skin");
            if (skin.empty() == false)
            {
                bgi.id = skin.as_uint();
            }

            for (auto child = node.first_child();
                 child;
                 child = child.next_sibling())
            {
                std::string name = child.name();
                if (name == "color")
                {
                    if (getColor(child, bgi.color) == false)
                        return false;
                }
                else
                    return false;
            }
            return true;
        }
        // 解析边框
        bool getBorder(pugi::xml_node &node, UIBDI &border)
        {
            for (auto child = node.first_child();
                 child;
                 child = child.next_sibling())
            {
                std::string name = child.name();
                if (name == "width")
                {
                    if (getBorderWidth(child, border.width) == false)
                        return false;
                }
                else if (name == "radius")
                {
                    if (getBorderRadius(child, border.round) == false)
                        return false;
                }
                else if (name == "color")
                {
                    if (getColor(child, border.color) == false)
                        return false;
                }
                else
                    return false;
            }
            return true;
        }

        // 解析字体
        bool getFont(pugi::xml_node &node, UIFNT &fnt)
        {
            auto size = node.attribute("size");
            if (size.empty() == false)
                fnt.size = size.as_float();

            for (auto child = node.first_child();
                 child;
                 child = child.next_sibling())
            {
                std::string name = child.name();
                if (name == "color")
                {
                    if (getColor(child, fnt.color) == false)
                        return false;
                }
                else
                    return false;
            }
            return true;
        }

        // 解析按钮状态
        bool getButtonState(pugi::xml_node &node, Button::Style::State& state)
        {
            for (auto child = node.first_child();
                 child;
                 child = child.next_sibling())
            {
                std::string name = child.name();
                if (name == "background")
                {
                    if (getBackground(child, state.mBg) == false)
                        return false;
                }
                else if (name == "border")
                {
                    if (getBorder(child, state.mBorder) == false)
                        return false;
                }
                else if (name == "font")
                {
                    if (getFont(child, state.mFont) == false)
                        return false;
                }
                else
                    return false;
            }
            return true;
        }
    }
    // 解析button的样式
    IUIStyle *button(pugi::xml_node &node)
    {
        auto style = new Button::Style();
        bool error = false;

        for (auto component = node.first_child();
             component;
             component = component.next_sibling())
        {
            std::string name = component.name();
            if (name == "margin")
            {
                if (helper::getMargin(component, style->mMargin))
                {
                    error = true;
                    break;
                }
                style->margin = true;
                continue;
            }
            if (name == "normal")
            {
                if (helper::getButtonState(component, style->mNormal) == false)
                {
                    error = true;
                    break;
                }
                style->normal = true;
                continue;
            }
            if (name == "hover")
            {
                if (helper::getButtonState(component, style->mHover) == false)
                {
                    error = true;
                    break;
                }
                style->hover = true;
                continue;
            }
            if (name == "active")
            {
                if (helper::getButtonState(component, style->mActive) == false)
                {
                    error = true;
                    break;
                }
                style->active = true;
                continue;
            }
            if (name == "disabled")
            {
                if (helper::getButtonState(component, style->mDisabled) == false)
                {
                    error = true;
                    break;
                }
                style->disabled = true;
                continue;
            }
            error = true;
            break;
        }

        if (error == true)
        {
            delete style;
            style = nullptr;
        }
        return style;
    }
}
#endif // __XMLDOMAIN_H__