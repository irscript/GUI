#ifndef __BUTTON_H__
#define __BUTTON_H__
#include <airkit/GUI/UI/IGUIElement.hpp>
#include <airkit/GUI/Theme/UIStyle.hpp>
namespace airkit
{

    struct Button : public IGUIElement
    {
        Button(const UIPoint &pos, const UISize &size)
            : IGUIElement()
        {
            mArea.setArea(pos, size);
        }

        virtual std::string getStyleName() const { return "button"; }

        struct Style : public IUIStyle
        {
            Style() : IUIStyle("button"), mFlag(0) {}

            // 通用
            UIMargin mMargin; // 外边距
            struct State
            {
                UIBGI mBg;     // 背景
                UIBDI mBorder; // 边框
                UIFNT mFont;   // 字体
            };
            State mNormal;   // 正常状态
            State mHover;    // 鼠标悬浮
            State mActive;   // 激活状态
            State mDisabled; // 禁用状态
            union
            {
                uint32_t mFlag;
                struct
                {
                    uint32_t margin : 1; // 有外边距
                };
            };
        };
    };
}

#endif // __BUTTON_H__