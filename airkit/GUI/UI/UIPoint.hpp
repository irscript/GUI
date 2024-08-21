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

        // 单位化（归一化）
        void normalize()
        {
            float len = sqrt(mX * mX + mY * mY);
            if (len > 0.0f)
            {
                mX /= len;
                mY /= len;
            }
        }

    //protected:
        float mX;
        float mY;
    };

    struct UISize
    {
        UISize(float width = 0.0, float height = 0.0)
            : mWidth(width), mHeight(height)
        {
        }
        UISize(const UISize &size)
            : mWidth(size.mWidth), mHeight(size.mHeight)
        {
        }
        UISize &operator=(const UISize &size)
        {
            mWidth = size.mWidth;
            mHeight = size.mHeight;
            return *this;
        }
        float getWidth() const { return mWidth; }
        float getHeight() const { return mHeight; }

        float getMin() const { return mMin; }
        float getMax() const { return mMax; }

        void set(float width, float height)
        {
            mWidth = width;
            mHeight = height;
        }

        bool operator==(const UISize &size) const
        {
            return mWidth == size.mWidth &&
                   mHeight == size.mHeight;
        }

   // protected:
        union
        {
            struct
            {
                float mWidth;
                float mHeight;
            };
            struct
            {
                float mMin;
                float mMax;
            };
        };
    };

    // UI元素的大小限制
    struct UILimit
    {
        UILimit() : mH(10, max), mV(10, max) {}
        UILimit(const UISize &h, const UISize &v)
            : mH(h), mV(v) {}
        UILimit(float minWidth, float minHeight, float maxWidth, float maxHeight)
            : mH(minWidth, maxWidth), mV(minHeight, maxHeight) {}
        UILimit(const UILimit &limit) : mH(limit.mH), mV(limit.mV) {}

        UILimit &operator=(const UILimit &limit)
        {
            mH = limit.mH;
            mV = limit.mV;
            return *this;
        }

        void setH(const UISize &h) { mH = h; }
        void setV(const UISize &v) { mV = v; }

        // 水平方向限制值
        const UISize &getH() const { return mH; }
        // 垂直方向限制值
        const UISize &getV() const { return mV; }

        static constexpr float max = 1E30; // 最大限制值

   // protected:
        UISize mH; // 水平方向限制值
        UISize mV; // 垂直方向限制值
    };

}
#endif // __UIPOINT_H__