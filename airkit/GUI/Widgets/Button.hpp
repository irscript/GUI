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

        virtual std::string getStyleName() const{return "button";}
        
    };
}

#endif // __BUTTON_H__