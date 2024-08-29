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
        //------------------draw 绘制系列函数--------------------

        // 绘制点
        void drawPoint(const UIPoint &point, RGBA color, float thickness = 1.0f);
        // 绘制线
        void drawLine(const UIPoint &start, const UIPoint &end, RGBA color, float thickness = 1.0f) { drawLine(start, color, end, color, thickness); }
        void drawLine(const UIPoint &start, RGBA color, const UIPoint &end, RGBA color2, float thickness = 1.0f);

        // 绘制三角形（逆时针三个点）:只适用于等边三角形，其他三角环形效果不好，
        void drawTriangle(const UIPoint &a, const UIPoint &b, const UIPoint &c, RGBA color, float thickness = 1.0f) { drawTriangle(a, color, b, color, c, color, thickness); }
        void drawTriangle(const UIPoint &a, RGBA color, const UIPoint &b, RGBA color2, const UIPoint &c, RGBA color3, float thickness = 1.0f);
        // 绘制矩形线框
        void drawRect(const UIArea &area, RGBA color, float thickness = 1.0) { drawRect(area, color, color, color, color, thickness); }
        void drawRect(const UIArea &area, RGBA left, RGBA right, float thickness = 1.0) { drawRect(area, left, right, left, right, thickness); }
        void drawRect2(const UIArea &area, RGBA top, RGBA bottom, float thickness = 1.0) { drawRect(area, top, top, bottom, bottom, thickness); }
        void drawRect(const UIArea &area, RGBA tlc, RGBA trc, RGBA blc, RGBA brc, float thickness = 1.0);

        void drawRect(const UIPoint &tl, const UIPoint &br, RGBA color, float thickness = 1.0) { drawRect(tl, br, color, color, color, color, thickness); }
        void drawRect(const UIPoint &tl, const UIPoint &br, RGBA left, RGBA right, float thickness = 1.0) { drawRect(tl, br, left, right, left, right, thickness); }
        void drawRect2(const UIPoint &tl, const UIPoint &br, RGBA top, RGBA bottom, float thickness = 1.0) { drawRect(tl, br, top, top, bottom, bottom, thickness); }
        void drawRect(const UIPoint &tl, const UIPoint &br, RGBA tlc, RGBA trc, RGBA blc, RGBA brc, float thickness = 1.0);

        // 绘制圆形
        void drawCircle(const UIPoint &center, float radius, RGBA color, float thickness = 1.0f, uint32_t segments = 32) { drawCircle(center, radius, color, color, thickness, segments); }
        void drawCircle(const UIPoint &center, float radius, RGBA in, RGBA out, float thickness = 1.0f, uint32_t segments = 32);

        // 绘制弧：极坐标下角度是逆时针方向增大
        void drawArc(const UIPoint &center, float radius,
                     float startAngle, float endAngle,
                     RGBA color, float thickness = 1.0f,
                     uint32_t segments = 32) { drawArc(center, radius, startAngle, endAngle,
                                                       color, color, thickness, segments); }
        void drawArc(const UIPoint &center, float radius,
                     float startAngle, float endAngle,
                     RGBA in, RGBA out, float thickness = 1.0f, uint32_t segments = 32);

        //------------------fill 填充系列函数--------------------
        void fillTriangle(const UIPoint &a, const UIPoint &b, const UIPoint &c, RGBA color) { fillTriangle(a, color, b, color, c, color); }
        void fillTriangle(const UIPoint &a, RGBA color, const UIPoint &b, RGBA color2, const UIPoint &c, RGBA color3);

        // 绘制矩形实体
        void fillRect(const UIArea &area, RGBA color) { fillRect(area, color, color, color, color); }
        void fillRect(const UIArea &area, RGBA left, RGBA right) { fillRect(area, left, right, left, right); }
        void fillRect2(const UIArea &area, RGBA top, RGBA bottom) { fillRect(area, top, top, bottom, bottom); }
        void fillRect(const UIArea &area, RGBA tlc, RGBA trc, RGBA blc, RGBA brc);

        void fillRect(const UIPoint &tl, const UIPoint &br, RGBA color) { fillRect(tl, br, color, color, color, color); }
        void fillRect(const UIPoint &tl, const UIPoint &br, RGBA left, RGBA right) { fillRect(tl, br, left, right, left, right); }
        void fillRect2(const UIPoint &tl, const UIPoint &br, RGBA top, RGBA bottom) { fillRect(tl, br, top, top, bottom, bottom); }
        void fillRect(const UIPoint &tl, const UIPoint &br, RGBA tlc, RGBA trc, RGBA blc, RGBA brc);

        /// @brief 绘制圆角矩形
        /// @param area 绘制的矩形区域
        /// @param clr4 4个顶点的颜色，顺序为左上，右上，左下，右下
        /// @param r4 圆角半径为4个值，顺时针顺序为左上，右上，左下，右下
        void fillRoundRect(const UIArea &area, const RGBA *const clr4, const float *r4);

        // 填充圆形
        void fillCircle(const UIPoint &center, float radius, RGBA color, uint32_t segments = 32) { fillCircle(center, radius, color, color, segments); }
        void fillCircle(const UIPoint &center, float radius, RGBA in, RGBA out, uint32_t segments = 32);

        // 填充扇形
        void fillSector(const UIPoint &center, float radius,
                        float startAngle, float endAngle,
                        RGBA color, uint32_t segments = 32) { fillSector(center, radius, startAngle, endAngle,
                                                                         color, color, segments); }
        void fillSector(const UIPoint &center, float radius,
                        float startAngle, float endAngle,
                        RGBA in, RGBA out, uint32_t segments = 32);

        //------------------tex 纹理系列函数--------------------
        /// @brief 绘制矩形纹理
        /// @param tl 左上角顶点位置
        /// @param tluv  左上角顶点纹理坐标
        /// @param br 右下角顶点位置
        /// @param bruv 右下角顶点纹理坐标
        /// @param color 4个顶点的颜色
        void texRect(const UIPoint &tl, const UIPoint &tluv,
                     const UIPoint &br, const UIPoint &bruv,
                     RGBA color = RGBA::fromRGBA())
        {
            RGBA clrarr[4] = {color, color, color, color};
            texRect(tl, tluv, br, bruv, clrarr);
        }

        /// @brief 绘制矩形纹理
        /// @param tl 左上角顶点位置
        /// @param tluv  左上角顶点纹理坐标
        /// @param br 右下角顶点位置
        /// @param bruv 右下角顶点纹理坐标
        /// @param color 4个顶点的颜色，顺序为左上，右上，左下，右下
        void texRect(const UIPoint &tl, const UIPoint &tluv,
                     const UIPoint &br, const UIPoint &bruv,
                     const RGBA *const color);
        /// @brief 绘制圆角矩形纹理
        /// @param tl 左上角顶点位置
        /// @param tluv  左上角顶点纹理坐标
        /// @param br 右下角顶点位置
        /// @param bruv 右下角顶点纹理坐标
        /// @param color 4个顶点的颜色，顺序为左上，右上，左下，右下
        /// @param radius 4个圆角半径，顺时针顺序为左上，右上，左下，右下
        void texRoundRect(const UIPoint &tl, const UIPoint &tluv,
                     const UIPoint &br, const UIPoint &bruv,
                     const RGBA *const color, const float *radius);

    public:
    private:
        UIDrawList &mDrawList;

    private:
        void genRect(const UIPoint &tl, RGBA tlc, const UIPoint &tr, RGBA trc,
                     const UIPoint &bl, RGBA blc, const UIPoint &br, RGBA brc);

    };
}

#endif // __UIHELPER_H__
