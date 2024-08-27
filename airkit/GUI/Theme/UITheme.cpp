#include <airkit/GUI/Theme/UITheme.hpp>
#include "UITheme.hpp"

namespace airkit
{
    
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


