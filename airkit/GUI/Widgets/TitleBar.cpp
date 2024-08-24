#include <airkit/GUI/Widgets/TitleBar.hpp>
#include <airkit/GUI/UI/UIHelper.hpp>
#include <airkit/GUI/UI/IWindow.hpp>
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
            // 在标题栏上
            return this;
        }

        // 判断是否在用户元素区域
        element = mUIUseArea.get();
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
        ui.fillRect(mArea, RGBA(0xffdde1));
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
        UIArea area = mArea;
        area.mX += clip.mX;
        area.mY += clip.mY;
        cmd.setClipRect(area);

        UIHelper ui(drawList);

        auto center = area.getCenter();
        center.mY += area.getHeight() / 6;
        center.mX -= area.getWidth() / 5;
        auto r=std::min(area.getWidth(), area.getHeight()) / 4;

        ui.drawCircle(center, r, RGBA(142,45,226),RGBA(74,14,0), 8,3);

        drawList.end(cmd, DrawFlag::Vertex);
    }
    void TitleBar::ButtonMaximize::onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList)
    {
        auto &cmd = drawList.begin();
        UIArea area = mArea;
        area.mX += clip.mX;
        area.mY += clip.mY;
        cmd.setClipRect(area);

        UIHelper ui(drawList);
        RGBA color = vibe.mHover == this ? RGBA(0, 0, 255, 0) : RGBA(0xee9ca7);

        float y = area.getHeight() / 4;
        float x = area.getWidth() / 5;

        auto tl = area.getTL(), br = area.getBR();
        tl.mY += y * 2;
        tl.mX += x;
        br.mY -= y;
        br.mX -= x*2;

        ui.drawRect(tl, br, color, 2);

        drawList.end(cmd, DrawFlag::Vertex);
    }
    void TitleBar::ButtonMaximize::onMouseUp(MouseUpEvent &event)
    {
        if (event.getButton() == MouseButton::Left)
        {
            auto window = getUIWindow();
            if (window.get() == nullptr)
                return;
            auto &winptr = window->as<IWindow>();
            mIsMaximize == true ? winptr.restore() : winptr.maximize();
            mIsMaximize = !mIsMaximize;
        }
    }
    void TitleBar::ButtonMinimize::onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList)
    {
        auto &cmd = drawList.begin();
        UIArea area = mArea;
        area.mX += clip.mX;
        area.mY += clip.mY;
        cmd.setClipRect(area);

        UIHelper ui(drawList);
        RGBA color = vibe.mHover == this ? RGBA(0, 0, 255, 0) : RGBA(0xee9ca7);
        UIPoint start = area.getTL(), end = area.getTR();

        float y = area.getHeight() / 5 * 3;
        float x = area.getWidth() / 4;
        start.mY += y;
        start.mX += x;
        end.mY += y;
        end.mX -= x;

        ui.drawLine(start, end, color, 2);

        drawList.end(cmd, DrawFlag::Vertex);
    }
    void TitleBar::ButtonMinimize::onMouseUp(MouseUpEvent &event)
    {
        if (event.getButton() == MouseButton::Left)
        {
            auto window = getUIWindow();
            if (window.get() == nullptr)
                return;
            auto &winptr = window->as<IWindow>();
            winptr.minimize();
        }
    }
    void TitleBar::ButtonClose::onRenderFrame(const UIVibe &vibe, const UIArea &clip, UIDrawList &drawList)
    {
        auto &cmd = drawList.begin();
        UIArea area = mArea;
        area.mX += clip.mX;
        area.mY += clip.mY;
        cmd.setClipRect(area);

        UIHelper ui(drawList);
        RGBA color = vibe.mHover == this ? RGBA(0, 0, 255, 0) : RGBA(0xee9ca7);

        float y = area.getHeight() / 4;
        float x = area.getWidth() / 5;

        auto tl = area.getTL(), br = area.getBR();
        tl.mY += y * 2;
        //tl.mX += x;
        br.mY -= y;
        br.mX -= x*3;

        UIPoint tr(br.mX, tl.mY);
        UIPoint bl(tl.mX, br.mY);

        ui.drawLine(tl, br, color, 4);
        ui.drawLine(bl, tr, color, 4);
        drawList.end(cmd, DrawFlag::Vertex);
    }
    void TitleBar::ButtonClose::onMouseUp(MouseUpEvent &event)
    {
        if (event.getButton() == MouseButton::Left)
        {
            auto window = getUIWindow();
            if (window.get() == nullptr)
                return;
            auto &winptr = window->as<IWindow>();
            winptr.close();
        }
    }
}
