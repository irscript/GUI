#include <airkit/GUI/Theme/UITheme.hpp>

namespace airkit
{
    bool UITheme::load(const std::string &path)
    {
        return false;
    }
    const IUIStyle *UITheme::getStyle(const std::string &name) const
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
