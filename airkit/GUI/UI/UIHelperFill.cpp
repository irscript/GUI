
#include <airkit/GUI/UI/UIHelper.hpp>
#include "UIHelper.hpp"

namespace airkit
{
    void UIHelper::fillTriangle(const UIPoint &a, RGBA color,
                                const UIPoint &b, RGBA color2,
                                const UIPoint &c, RGBA color3)
    {
        auto idx = mDrawList.mVertices.size();
        // 先生成顶点
        mDrawList.needVertex(3);
        mDrawList.mVertices.push_back(UIVertex(a, color));
        mDrawList.mVertices.push_back(UIVertex(b, color2));
        mDrawList.mVertices.push_back(UIVertex(c, color3));
        // 在生成索引
        mDrawList.needIndex(3);
        mDrawList.mIndices.push_back(idx);
        mDrawList.mIndices.push_back(idx + 2);
        mDrawList.mIndices.push_back(idx + 1);
    }

    void UIHelper::fillRect(const UIArea &area, RGBA tlc, RGBA trc, RGBA blc, RGBA brc)
    {
        auto tl = area.getTL();
        auto tr = area.getTR();
        auto bl = area.getBL();
        auto br = area.getBR();
        genRect(tl, tlc, tr, trc, bl, blc, br, brc);
    }

    void UIHelper::fillRect(const UIPoint &tl, const UIPoint &br, RGBA tlc, RGBA trc, RGBA blc, RGBA brc)
    {
        auto tr = UIPoint(br.getX(), tl.getY());
        auto bl = UIPoint(tl.getX(), br.getY());
        genRect(tl, tlc, tr, trc, bl, blc, br, brc);
    }

    void UIHelper::fillCircle(const UIPoint &center, float radius, RGBA in, RGBA out, uint32_t segments)
    {
        // 先生成顶点
        float angleStep = 2.0f * 3.14159265358979323846f / segments;
        mDrawList.needVertex(segments + 1);
        auto ic = mDrawList.mVertices.size();
        auto &c = mDrawList.mVertices.emplace_back(); // 圆心
        c.mColor = in;
        c.mXY = center;
        auto ip2 = mDrawList.mVertices.size();
        for (int32_t i = 0; i < segments; i++)
        {
            float angle = i * angleStep;
            auto &c = mDrawList.mVertices.emplace_back();
            c.mColor = out;
            c.mXY.mX = center.mX + radius * cos(angle);
            c.mXY.mY = center.mY + radius * sin(angle);
        }
        mDrawList.needIndex(segments * 3);
        // 然后与圆点组成三角形
        for (int32_t i = 0; i < segments; i++)
        {
            auto start = i;
            auto end = ((i + 1) % segments);
            mDrawList.mIndices.push_back(ic);
            mDrawList.mIndices.push_back(ip2 + end);
            mDrawList.mIndices.push_back(ip2 + start);
        }
    }
    void UIHelper::fillSector(const UIPoint &center, float radius,
                              float startAngle, float endAngle,
                              RGBA in, RGBA out, uint32_t segments)
    {
        // 先生成线段的端点
        auto sradian = 3.14159265358979323846f / 180.0f; // 0.01745329252
        auto subRadian = (endAngle - startAngle) * sradian;
        float angleStep = subRadian / segments;

        uint32_t count = segments + 1;
        mDrawList.needVertex(count + 1);
        // 圆心
        auto ic = mDrawList.mVertices.size();
        auto &c = mDrawList.mVertices.emplace_back();
        c.mColor = in;
        c.mXY = center;
        auto ip2 = mDrawList.mVertices.size();
        for (int32_t i = 0; i < count; i++)
        {
            float angle = startAngle + i * angleStep;
            auto &c = mDrawList.mVertices.emplace_back();
            c.mColor = out;
            c.mXY.mX = center.mX + radius * cos(angle);
            c.mXY.mY = center.mY + radius * sin(angle);
        }
        mDrawList.needIndex(segments * 3);
        // 然后与圆点组成三角形
        for (int32_t i = 0; i < segments; i++)
        {
            auto start = i;
            auto end = (i + 1);
            mDrawList.mIndices.push_back(ic);
            mDrawList.mIndices.push_back(ip2 + end);
            mDrawList.mIndices.push_back(ip2 + start);
        }
    }

}
