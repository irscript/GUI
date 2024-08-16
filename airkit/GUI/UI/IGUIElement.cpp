#include <airkit/GUI/UI/IGUIElement.hpp>
#include <airkit/GUI/UI/IWindow.hpp>
#include <airkit/GUI/IPlat.hpp>
#include "IWindow.hpp"

airkit::IGUIElement::~IGUIElement() {}

void airkit::IGUIElement::screenToWindow(UIPoint &point) const
{
    // 先找到窗口指针
    UIHolder parent = getParentUI();
    while (parent.get() != nullptr)
    {
        if (parent->mUIFlag.check(UIFlag::Window) == true)
            break;
        parent = parent->getParentUI();
    }
    // 调用窗口的转换函数
    if (parent.get() == nullptr)
        return;
    auto x = mArea.getX() - point.getX();
    auto y = mArea.getY() - point.getY();
    point.setXY(x, y);
}
void airkit::IGUIElement::windowToScreen(UIPoint &point) const
{
    // 先找到窗口指针
    UIHolder parent = getParentUI();
    while (parent.get() != nullptr)
    {
        if (parent->mUIFlag.check(UIFlag::Window) == true)
            break;
        parent = parent->getParentUI();
    }
    // 调用窗口的转换函数
    if (parent.get() != nullptr)
        return;

    auto x = mArea.getX() + point.getX();
    auto y = mArea.getY() + point.getY();
    point.setXY(x, y);
}
void airkit::IGUIElement::UIToWindow(UIPoint &point) const
{
    auto x = point.getX() + mArea.getX();
    auto y = point.getY() + mArea.getY();
    // 先找到窗口指针
    UIHolder parent = getParentUI();
    while (parent.get() != nullptr)
    {
        // 父UI是窗口
        if (parent->mUIFlag.check(UIFlag::Window) == true)
            break;
        // 计算坐标
        x += parent->mArea.getX();
        y += parent->mArea.getY();

        // 继续找父UI
        parent = parent->getParentUI();
    }
    point.setXY(x, y);
}
void airkit::IGUIElement::windowToUI(UIPoint &point) const
{
    // 先将UI原点转换为窗口坐标
    UIPoint uiPoint(0, 0);
    UIToWindow(uiPoint);

    // 在减去UI的窗口坐标
    auto x = point.getX() - uiPoint.getX();
    auto y = point.getY() - uiPoint.getY();
    point.setXY(x, y);
}
void airkit::IGUIElement::UIToscreen(UIPoint &point) const
{
    auto x = point.getX() + mArea.getX();
    auto y = point.getY() + mArea.getY();
    // 先找到窗口指针
    UIHolder parent = getParentUI();
    while (parent.get() != nullptr)
    {
        // 计算坐标
        x += parent->mArea.getX();
        y += parent->mArea.getY();
        // 继续找父UI
        parent = parent->getParentUI();
    }
    point.setXY(x, y);
}
void airkit::IGUIElement::screenToUI(UIPoint &point) const
{
    // 先将UI原点转换为屏幕坐标
    UIPoint uiPoint(0, 0);
    UIToscreen(uiPoint);

    // 在减去UI的屏幕坐标
    auto x = point.getX() - uiPoint.getX();
    auto y = point.getY() - uiPoint.getY();
    point.setXY(x, y);
}

airkit::UIHolder airkit::IGUIElement::getWindowUI()
{
    // 如果自己就是窗口
    if (mUIFlag.check(UIFlag::Window) == true)
        return IPlat::getInstance().getWinHub().findWindow(this);

    UIHolder parent = getParentUI();
    while (parent.get() != nullptr)
    {
        if (parent->mUIFlag.check(UIFlag::Window) == true)
            break;
        parent = parent->getParentUI();
    }
    return parent;
}

const airkit::UIHolder airkit::IGUIElement::getWindowUI() const
{
    // 如果自己就是窗口
    if (mUIFlag.check(UIFlag::Window) == true)
        return IPlat::getInstance().getWinHub().findWindow(this);

    UIHolder parent = getParentUI();
    while (parent.get() != nullptr)
    {
        if (parent->mUIFlag.check(UIFlag::Window) == true)
            break;
        parent = parent->getParentUI();
    }
    return parent;
}


airkit::IWindow::~IWindow() {}
void airkit::IWindow::setShouldClose(bool shouldClose)
{
    shouldClose == true ? mUIFlag.set(UIFlag::Closed) : mUIFlag.reset(UIFlag::Closed);
}

bool airkit::IWindow::shouldClose() const
{
    return mUIFlag.check(UIFlag::Closed);
}
