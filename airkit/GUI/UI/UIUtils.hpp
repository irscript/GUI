#ifndef __UIUTILS_H__
#define __UIUTILS_H__

#include <airkit/GUI/UI/UIDrawData.hpp>

namespace airkit
{
    // UI 数据生成工具 : 与窗口 DPI 无关
    // draw 系列函数：表示绘制图形为空心的，即是线框模式
    // fill 系列函数：表示绘制图形为实体的，即是填充模式
    struct UIUtils
    {
        UIUtils(UIDrawList &list) : mDrawList(list) {}

    public:
        // 绘制线
        void drawLine(const UIPoint &start, const UIPoint &end, RGBA color, float thickness = 1.0f);
        // 绘制矩形线框
        void drawRect(const UIArea &area, RGBA color, float thickness = 1.0);
        // 绘制矩形实体
        void fillRect(const UIArea &area, RGBA color);

    public:
        // 设置颜色
        void setColor(RGBA color) { mColor = color; }
        // 设置线厚度
        void setThickness(float width) { mThickness = width; }

    private:
        RGBA mColor;
        float mThickness;
        UIDrawList &mDrawList;
    };
}

#endif // __UIUTILS_H__