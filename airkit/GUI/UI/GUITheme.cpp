#include <airkit/GUI/UI/GUITheme.hpp>

namespace airkit
{
    bool UITheme::load(const std::string &path)
    {
        return false;
    }
    const UIStyle *UITheme::getStyle(const std::string &name) const
    {
        for (auto group : mUsing)
        {
            auto res = group->getStyle(name);
            if (res != nullptr)
                return res;
            continue;
        }
        return nullptr;
    }
}
