#ifndef __UIAREA_H__
#define __UIAREA_H__
#include <airkit/GUI/UI/UIPoint.hpp>
namespace airkit
{

    /** 控件坐标系
     * (0,0)
     *  (L,T)----------------------------> X
     *      |                       |
     *      |                       |
     *      |                       |
     *      |-----------------------|(R,B)
     *      |
     *      V
     *      Y
     * width=B-T
     * height=R-L
     */

    // UI 矩形区域
    struct UIArea
    {
        UIArea(float x = 0.0, float y = 0.0,
               float width = 0.0, float height = 0.0)
            : mX(x), mY(y),
              mWidth(width), mHeight(height) {}

        void setArea(float x, float y, float width, float height)
        {
            mX = x;
            mY = y;
            mWidth = width;
            mHeight = height;
        }
        void setPosition(float x, float y)
        {
            mX = x;
            mY = y;
        }
        void setSize(float width, float height)
        {
            mWidth = width;
            mHeight = height;
        }

        float getRight() const { return mX + mWidth; }
        float getBottom() const { return mY + mHeight; }
        float getLeft() const { return mX; }
        float getTop() const { return mY; }

        float getX() const { return mX; }
        float getY() const { return mY; }
        float getWidth() const { return mWidth; }
        float getHeight() const { return mHeight; }

        // 判断点是否在矩形区域内
        bool isInArea(const UIPoint &point) const
        {
            return point.getX() >= mX && point.getX() <= getRight() &&
                   point.getY() >= mY && point.getY() <= getBottom();
        }
        // 判断点是否在矩形区域内
        bool isInArea(float x, float y) const
        {
            return x >= mX && x <= getRight() &&
                   y >= mY && y <= getBottom();
        }

    protected:
        float mX;
        float mY;
        float mWidth;
        float mHeight;
    };
}

#endif // __UIAREA_H__