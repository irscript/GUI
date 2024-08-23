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
        UIArea(const UIPoint &pos, const UISize &size)
            : mX(pos.getX()), mY(pos.getY()), mWidth(size.getWidth()), mHeight(size.getHeight()) {}

        void setArea(float x, float y, float width, float height)
        {
            mX = x;
            mY = y;
            mWidth = width;
            mHeight = height;
        }
        void setArea(const UIPoint &pos, const UISize &size)
        {
            mX = pos.getX();
            mY = pos.getY();
            mWidth = size.getWidth();
            mHeight = size.getHeight();
        }
        void setPos(float x, float y)
        {
            mX = x;
            mY = y;
        }
        void setSize(float width, float height)
        {
            mWidth = width;
            mHeight = height;
        }
        void setX(float x){mX = x;}
        void setY(float y){mY = y;}

        // 获取矩形在父容器的位置
        UIPoint getPos() const { return UIPoint(mX, mY); }
        // 获取矩形的尺寸
        UISize getSize() const { return UISize(mWidth, mHeight); }

        float getRight() const { return mX + mWidth; }
        float getBottom() const { return mY + mHeight; }
        float getLeft() const { return mX; }
        float getTop() const { return mY; }

        UIPoint getTL() const { return {mX, mY}; }
        UIPoint getTR() const { return {mX + mWidth, mY}; }
        UIPoint getBL() const { return {mX, mY + mHeight}; }
        UIPoint getBR() const { return {mX + mWidth, mY + mHeight}; }

        UIPoint getCenter() const { return {mX + mWidth / 2, mY + mHeight / 2}; }

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