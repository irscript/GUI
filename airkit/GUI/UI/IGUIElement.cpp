#include <airkit/GUI/UI/IGUIElement.hpp>
#include <airkit/GUI/UI/IWindow.hpp>
#include <airkit/GUI/IPlat.hpp>
#include "IGUIElement.hpp"

namespace airkit
{
    IGUIElement::~IGUIElement()
    {
        auto window = getUIWindow();
        if (window.get() != nullptr)
        {
            auto &win = *(IWindow *)window.get();
            auto &vibe = win.getUIVibe();
            if (vibe.mFocus == this)
                vibe.mFocus = nullptr;
            if (vibe.mHover == this)
                vibe.mHover = nullptr;
        }
    }
    void IGUIElement::onEvent(IEvent &event)
    {
        auto kind = event.getKind();
        switch (kind)
        {
        case EventKind::Keyboard:
        {
            auto &ev = event.as<IKeyEvent>();
            auto action = ev.getAction();
            switch (action)
            {
            case KeyAction::Down:
                return onKeyDown(ev.as<KeyDownEvent>());
            case KeyAction::Up:
                return onKeyUp(ev.as<KeyUpEvent>());
            }
        }
        break;
        case EventKind::Mouse:
        {
            auto &ev = event.as<IMouseEvent>();
            auto action = ev.getAction();
            switch (action)
            {
            case MouseAction::Down:
                return onMouseClick(ev.as<MouseDownEvent>());
            case MouseAction::Move:
                return onMouseMove(ev.as<MouseMoveEvent>());
            case MouseAction::Up:
                return onMouseUp(ev.as<MouseUpEvent>());
            case MouseAction::Enter:
                return onMouseEnter(ev.as<MouseEnterEvent>());
            case MouseAction::Hover:
                return onMouseHover(ev.as<MouseHoverEvent>());
            case MouseAction::Leave:
                return onMouseLeave(ev.as<MouseLeaveEvent>());
            case MouseAction::Wheel:
                return onMouseWheel(ev.as<MouseWheelEvent>());
            }
        }
        break;
        case EventKind::CharInput:
            return onCharInput(event.as<CharInputEvent>());
        case EventKind::UI:
        {
            auto &ev = event.as<UIEvent>();
            auto action = ev.getAction();
            switch (action)
            {
            case UIAction::Focus:
                return onFocus(ev.as<UIFocusEvent>());
            case UIAction::Show:
                return onShow(ev.as<UIShowEvent>());
            case UIAction::Resizing:
                return onSizing(ev.as<UIResizingEvent>());
            case UIAction::Resized:
                return onSized(ev.as<UIResizedEvent>());
            case UIAction::Moving:
                return onMoving(ev.as<UIMovingEvent>());
            case UIAction::Moved:
                return onMoved(ev.as<UIMovedEvent>());
            }
        }
        break;

        default:
            break;
        }
    }

    void IGUIElement::onFocus(UIFocusEvent &event)
    {
    }

    void IGUIElement::onShow(UIShowEvent &event)
    {
    }

    void IGUIElement::onSizing(UIResizingEvent &event)
    {
    }

    void IGUIElement::onSized(UIResizedEvent &event)
    {
    }

    void IGUIElement::onMoving(UIMovingEvent &event)
    {
    }

    void IGUIElement::onMoved(UIMovedEvent &event)
    {
        auto &pos = event.getPos();
        // printf("ui moved:(%f,%f)\n", pos.getX(), pos.getY());
    }
    void IGUIElement::onCharInput(CharInputEvent &event)
    {
        // printf("%d\n", event.getUtf32());
    }

    void IGUIElement::onKeyDown(KeyDownEvent &event)
    {
    }
    void IGUIElement::onKeyUp(KeyUpEvent &event)
    {
    }

    void IGUIElement::onMouseClick(MouseDownEvent &event)
    {
        // printf("mouse down[ %u ]:(%f,%f)\n", event.getButton(), event.getX(), event.getY());
    }

    void IGUIElement::onMouseUp(MouseUpEvent &event)
    {
        // printf("mouse up[ %u ]:(%f,%f)\n", event.getButton(), event.getX(), event.getY());
    }

    void IGUIElement::onMouseMove(MouseMoveEvent &event)
    {
        // printf("mouse move:(%f,%f)\n", event.getX(), event.getY());
    }

    void IGUIElement::onMouseEnter(MouseEnterEvent &event)
    {
        // printf("mouse enter:(%f,%f)\n", event.getX(), event.getY());
    }

    void IGUIElement::onMouseLeave(MouseLeaveEvent &event)
    {
        // printf("mouse leave:(%f,%f)\n", event.getX(), event.getY());
    }

    void IGUIElement::onMouseHover(MouseHoverEvent &event)
    {
        // printf("mouse hover:(%f,%f)\n", event.getX(), event.getY());
    }

    void IGUIElement::onMouseWheel(MouseWheelEvent &event)
    {
        // printf("mouse wheel:(%f,%f)->%f\n", event.getX(), event.getY(), event.getDelta());
    }

    IGUIElement *IGUIElement::onHitTest(const UIHitEvent &event)
    {
        // 先判断是否在本区域内
        if (mArea.isInArea(event.getPos()) == false)
            return nullptr;

        return this;
    }

    void IGUIElement::onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList)
    {
    }
    void IGUIElement::screenToWindow(UIPoint &point) const
    {
        // 先找到窗口指针
        UIHolder parent = getUIParent();
        while (parent.get() != nullptr)
        {
            if (parent->mUIFlag.checkMask(UIFlag::Window) == true)
                break;
            parent = parent->getUIParent();
        }
        // 调用窗口的转换函数
        if (parent.get() == nullptr)
            return;
        auto x = mArea.getX() - point.getX();
        auto y = mArea.getY() - point.getY();
        point.setXY(x, y);
    }
    void IGUIElement::windowToScreen(UIPoint &point) const
    {
        // 先找到窗口指针
        UIHolder parent = getUIParent();
        while (parent.get() != nullptr)
        {
            if (parent->mUIFlag.checkMask(UIFlag::Window) == true)
                break;
            parent = parent->getUIParent();
        }
        // 调用窗口的转换函数
        if (parent.get() != nullptr)
            return;

        auto x = mArea.getX() + point.getX();
        auto y = mArea.getY() + point.getY();
        point.setXY(x, y);
    }
    void IGUIElement::UIToWindow(UIPoint &point) const
    {
        auto x = point.getX() + mArea.getX();
        auto y = point.getY() + mArea.getY();
        // 先找到窗口指针
        UIHolder parent = getUIParent();
        while (parent.get() != nullptr)
        {
            // 父UI是窗口
            if (parent->mUIFlag.checkMask(UIFlag::Window) == true)
                break;
            // 计算坐标
            x += parent->mArea.getX();
            y += parent->mArea.getY();

            // 继续找父UI
            parent = parent->getUIParent();
        }
        point.setXY(x, y);
    }
    void IGUIElement::windowToUI(UIPoint &point) const
    {
        // 先将UI原点转换为窗口坐标
        UIPoint uiPoint(0, 0);
        UIToWindow(uiPoint);

        // 在减去UI的窗口坐标
        auto x = point.getX() - uiPoint.getX();
        auto y = point.getY() - uiPoint.getY();
        point.setXY(x, y);
    }
    void IGUIElement::UIToscreen(UIPoint &point) const
    {
        auto x = point.getX() + mArea.getX();
        auto y = point.getY() + mArea.getY();
        // 先找到窗口指针
        UIHolder parent = getUIParent();
        while (parent.get() != nullptr)
        {
            // 计算坐标
            x += parent->mArea.getX();
            y += parent->mArea.getY();
            // 继续找父UI
            parent = parent->getUIParent();
        }
        point.setXY(x, y);
    }
    void IGUIElement::screenToUI(UIPoint &point) const
    {
        // 先将UI原点转换为屏幕坐标
        UIPoint uiPoint(0, 0);
        UIToscreen(uiPoint);

        // 在减去UI的屏幕坐标
        auto x = point.getX() - uiPoint.getX();
        auto y = point.getY() - uiPoint.getY();
        point.setXY(x, y);
    }

    UIHolder IGUIElement::getUIWindow()
    {
        // 如果自己就是窗口
        if (mUIFlag.checkMask(UIFlag::Window) == true)
            return IPlat::getInstance().getWinHub().findWindow(this);

        UIHolder parent = getUIParent();
        while (parent.get() != nullptr)
        {
            if (parent->mUIFlag.checkMask(UIFlag::Window) == true)
                break;
            parent = parent->getUIParent();
        }
        return parent;
    }

    const UIHolder IGUIElement::getUIWindow() const
    {
        // 如果自己就是窗口
        if (mUIFlag.checkMask(UIFlag::Window) == true)
            return IPlat::getInstance().getWinHub().findWindow(this);

        UIHolder parent = getUIParent();
        while (parent.get() != nullptr)
        {
            if (parent->mUIFlag.checkMask(UIFlag::Window) == true)
                break;
            parent = parent->getUIParent();
        }
        return parent;
    }

    void IGUIElement::setUILimit(const UILimit &limit)
    {
        // 修正水平最小尺寸
        {
            auto h = limit.getH();
            auto min = std::min(h.getMin(), h.getMax());
            auto max = std::max(h.getMin(), h.getMax());
            if (min < 10)
                min = 10;
            if (max < 10)
                max = 10;
            mLimit.setH(UISize(min, max));
        }
        // 修正垂直最小尺寸
        {
            auto v = limit.getV();
            auto min = std::min(v.getMin(), v.getMax());
            auto max = std::max(v.getMin(), v.getMax());
            if (min < 10)
                min = 10;
            if (max < 10)
                max = 10;
            mLimit.setV(UISize(min, max));
        }
    }
    UIHolder IGUIElement::getSelf()
    {
        // 如果自己就是窗口
        if (mUIFlag.checkMask(UIFlag::Window) == true)
            return IPlat::getInstance().getWinHub().findWindow(this);

        auto parent = mParentUI.lock();
        if (parent.get() == nullptr)
            return UIHolder();
        return parent->getHolder(this);
    }
    IGUIElement *IUIParent::onHitTest(const UIHitEvent &event)
    {
        // 先判断是否在本区域内
        if (mArea.isInArea(event.getPos()) == false)
            return nullptr;
        // 判断是否没得子UI
        if (mChildUI.empty())
            return this;

        // 转换到本UI坐标系中
        const UIPoint &pos = event.getPos();
        auto x = pos.getX() - mArea.getX();
        auto y = pos.getY() - mArea.getY();
        UIPoint p(x, y);

        // 遍历子UI进行测试
        for (auto &child : mChildUI)
        {
            if (child->onHitTest(UIHitEvent(p)) != nullptr)
                return child.get();
        }
        // 子UI没有命中
        return this;
    }
    void IUIParent::onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList)
    { // 元素可见且有子UI
        if (mUIFlag.checkMask(UIFlag::Visible) == true && mChildUI.empty() == false)
        {
            // 子元素的裁剪矩阵
            UIArea clip2(clip.getX() + mArea.getX(),
                         clip.getY() + mArea.getY(),
                         mArea.getWidth(), mArea.getHeight());
            for (auto &child : mChildUI)
            {
                child->onRenderFrame(vibe, clip2, drawList);
            }
        }
    }
    UIHolder IUIParent::getHolder(IGUIElement *ui)
    {
        for (auto &child : mChildUI)
        {
            if (child.get() == ui)
                return child;
            auto holder = child->getHolder(ui);
            if (holder.get() != nullptr)
                return holder;
        }
        auto parent = mParentUI.lock();
        if (parent.get() == nullptr)
            return UIHolder();
        return parent->getHolder(this);
    }
}