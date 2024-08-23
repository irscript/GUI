#include <airkit/GUI/Widgets/TitleBar.hpp>
#include "TitleBar.hpp"
#include <airkit/GUI/UI/UIHelper.hpp>
namespace airkit
{
    IGUIElement *TitleBar::onHitTest(const UIHitEvent &event)
    {
        auto &pos = event.getPos();
        // 不在区域内
        if (mArea.isInArea(pos) == false)
            return nullptr;

        // 判断在ICON区域
        IGUIElement *element = mUIIcon.get();
        if (element != nullptr && element->getArea().isInArea(pos))
            return element;

        // 判断在标题区域
        element = mUITitle.get();
        if (element != nullptr && element->getArea().isInArea(pos))
            return element;

        // 判断在按钮区域
        if (mBtnArea.isInArea(pos))
        {
            element = mUIMaximize.get();
            if (element != nullptr && element->getArea().isInArea(pos))
                return element;
            element = mUIMinimize.get();
            if (element != nullptr && element->getArea().isInArea(pos))
                return element;
            element = mUIClose.get();
            if (element != nullptr && element->getArea().isInArea(pos))
                return element;
        }

        // 判断是否在用户元素区域
        element = mUIClose.get();
        if (element != nullptr)
        {
            // 转换坐标
            auto child = element->onHitTest(event);
            if (child == element)
                return this;
            return child;
        }
        // 在标题栏上
        return this;
    }
    void TitleBar::onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList)
    {
        auto &cmd = drawList.begin();
        cmd.setClipRect(mArea);
        UIHelper ui(drawList);
        ui.fillRect(mArea, RGBA(0xee9ca7), RGBA(0xffdde1));
        drawList.end(cmd, DrawFlag::Vertex);

        if (mUIIcon.get() != nullptr)
            mUIIcon->onRenderFrame(vibe, clip, drawList);
        if (mUITitle.get() != nullptr)
            mUITitle->onRenderFrame(vibe, clip, drawList);

        if (mUIClose.get() != nullptr)
            mUIClose->onRenderFrame(vibe, clip, drawList);
        if (mUIMaximize.get() != nullptr)
            mUIMaximize->onRenderFrame(vibe, clip, drawList);
        if (mUIMinimize.get() != nullptr)
            mUIMinimize->onRenderFrame(vibe, clip, drawList);
    }
    void TitleBar::ButtonIcon::onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList)
    {
        auto &cmd = drawList.begin();
        cmd.setClipRect(mArea);

        UIHelper ui(drawList);

        ui.drawCircle(mArea.getCenter(), mArea.getWidth() / 2, RGBA(0xee9ca7), RGBA(0xffdde1), 4, 8);

        drawList.end(cmd, DrawFlag::Vertex);
    }
    void TitleBar::ButtonMaximize::onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList)
    {
        auto &cmd = drawList.begin();
        cmd.setClipRect(mArea);

        UIHelper ui(drawList);
        RGBA color = vibe.mHover == this ? RGBA(0xffdde1) : RGBA(0xee9ca7);

        ui.drawRect2(mArea.getCenter(), mArea.getWidth() / 2, color, 4);

        drawList.end(cmd, DrawFlag::Vertex);
    }
    void TitleBar::ButtonMinimize::onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList)
    {
        auto &cmd = drawList.begin();
        cmd.setClipRect(mArea);

        UIHelper ui(drawList);
        RGBA color = vibe.mHover == this ? RGBA(0xffdde1) : RGBA(0xee9ca7);
        UIPoint start = mArea.getTL(), end = mArea.getTR();

        ui.drawLine(start, end, RGBA(0xffdde1), 4);

        drawList.end(cmd, DrawFlag::Vertex);
    }
    void TitleBar::ButtonClose::onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList)
    {
        auto &cmd = drawList.begin();
        cmd.setClipRect(mArea);
        UIHelper ui(drawList);
        RGBA color = vibe.mHover == this ? RGBA(0xffdde1) : RGBA(0xee9ca7);
        float thickness = /* mArea.getHeight() / 4;
         if (thickness < 4)
             thickness =*/
            4;
        ui.drawLine(mArea.getTL(), mArea.getBR(), RGBA(0xffdde1), thickness);
        ui.drawLine(mArea.getBL(), mArea.getTR(), RGBA(0xffdde1), thickness);
        drawList.end(cmd, DrawFlag::Vertex);
    }
}
