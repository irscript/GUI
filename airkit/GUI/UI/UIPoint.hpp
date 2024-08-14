#ifndef __UIPOINT_H__
#define __UIPOINT_H__

namespace airkit
{
    struct UIPoint
    {
        UIPoint(float x = 0.0, float y = 0.0)
            : mX(x), mY(y)
        {
        }
        UIPoint(const UIPoint &point)
            : mX(point.mX), mY(point.mY)
        {
        }
        UIPoint &operator=(const UIPoint &point)
        {
            mX = point.mX;
            mY = point.mY;
            return *this;
        }

        float getX() const { return mX; }
        float getY() const { return mY; }

        void setXY(float x, float y)
        {
            mX = x;
            mY = y;
        }

    protected:
        float mX;
        float mY;
    };
}
#endif // __UIPOINT_H__