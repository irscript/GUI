#ifndef __UIHELPER_H__
#define __UIHELPER_H__

#include <airkit/GUI/UI/UIDrawData.hpp>

namespace airkit
{
    // UI 数据生成工具 : 与窗口 DPI 无关
    // draw 系列函数：表示绘制图形为空心的，即是线框模式
    // fill 系列函数：表示绘制图形为实体的，即是填充模式
    struct UIHelper
    {
        UIHelper(UIDrawList &list) : mDrawList(list) {}

    public:
        // 绘制线
        void drawLine(const UIPoint &start, const UIPoint &end, RGBA color, float thickness = 1.0f) { drawLine(start, color, end, color, thickness); }
        void drawLine(const UIPoint &start, RGBA color, const UIPoint &end, RGBA color2, float thickness = 1.0f);
        // 绘制矩形线框
        void drawRect(const UIArea &area, RGBA color, float thickness = 1.0) { drawRect(area, color, color, color, color, thickness); }
        void drawRect(const UIArea &area, RGBA left, RGBA right, float thickness = 1.0) { drawRect(area, left, right, left, right, thickness); }
        void drawRect2(const UIArea &area, RGBA top, RGBA bottom, float thickness = 1.0) { drawRect(area, top, top, bottom, bottom, thickness); }
        void drawRect(const UIArea &area, RGBA tlc, RGBA trc, RGBA blc, RGBA brc, float thickness = 1.0);

        void drawRect(const UIPoint &tl, const UIPoint &br, RGBA color, float thickness = 1.0) { drawRect(tl, br, color, color, color, color, thickness); }
        void drawRect(const UIPoint &tl, const UIPoint &br, RGBA left, RGBA right, float thickness = 1.0) { drawRect(tl, br, left, right, left, right, thickness); }
        void drawRect2(const UIPoint &tl, const UIPoint &br, RGBA top, RGBA bottom, float thickness = 1.0) { drawRect(tl, br, top, top, bottom, bottom, thickness); }
        void drawRect(const UIPoint &tl, const UIPoint &br, RGBA tlc, RGBA trc, RGBA blc, RGBA brc, float thickness = 1.0);
        // 绘制矩形实体
        void fillRect(const UIArea &area, RGBA color) { fillRect(area, color, color, color, color); }
        void fillRect(const UIArea &area, RGBA left, RGBA right) { fillRect(area, left, right, left, right); }
        void fillRect2(const UIArea &area, RGBA top, RGBA bottom) { fillRect(area, top, top, bottom, bottom); }
        void fillRect(const UIArea &area, RGBA tlc, RGBA trc, RGBA blc, RGBA brc);

        void fillRect(const UIPoint &tl, const UIPoint &br, RGBA color) { fillRect(tl, br, color, color, color, color); }
        void fillRect(const UIPoint &tl, const UIPoint &br, RGBA left, RGBA right) { fillRect(tl, br, left, right, left, right); }
        void fillRect2(const UIPoint &tl, const UIPoint &br, RGBA top, RGBA bottom) { fillRect(tl, br, top, top, bottom, bottom); }
        void fillRect(const UIPoint &tl, const UIPoint &br, RGBA tlc, RGBA trc, RGBA blc, RGBA brc);
        // 绘制矩形纹理
        void drawRactTex(const UIPoint &tl, const UIPoint &tluv, const UIPoint &br, const UIPoint &bruv, RGBA color = RGBA(-1));

    public:
        // 设置颜色
        void setColor(RGBA color) { mColor = color; }
        // 设置线厚度
        void setThickness(float width) { mThickness = width; }

    private:
        RGBA mColor;
        float mThickness;
        UIDrawList &mDrawList;

    private:
        struct Point
        {
            UIPoint point;
            RGBA color;
        };
        std::vector<Point> mPath; // 绘制路径点集
        // 添加绘制路径点
        inline void addPoint(const UIPoint &point, RGBA color);

        void genRect(const UIPoint &tl, RGBA tlc, const UIPoint &tr, RGBA trc,
                     const UIPoint &bl, RGBA blc, const UIPoint &br, RGBA brc);
    };
}

#endif // __UIHELPER_H__
