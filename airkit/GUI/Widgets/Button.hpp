#ifndef __BUTTON_H__
#define __BUTTON_H__
#include <airkit/GUI/UI/IGUIElement.hpp>

namespace airkit
{
    struct Button : public IGUIElement
    {
        Button(const UIPoint &pos, const UISize &size)
            : IGUIElement()
        {
            mArea.setArea(pos, size);
        }
    };
}

#endif // __BUTTON_H__