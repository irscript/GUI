
#include <airkit/GUI/UI/UIHelper.hpp>
#include "UIHelper.hpp"

namespace airkit
{
    void UIHelper::drawPoint(const UIPoint &point, RGBA color, float thickness)
    {
        auto hw = thickness * 0.5f;
        auto top = point.mY + hw;
        auto bottom = point.mY - hw;
        auto left = point.mX - hw;
        auto right = point.mX + hw;

        UIPoint tl{top, left};
        UIPoint tr{top, right};
        UIPoint bl{bottom, left};
        UIPoint br{bottom, right};

        genRect(tl, color, tr, color, bl, color, br, color);
    }

    void UIHelper::drawLine(const UIPoint &start, RGBA color, const UIPoint &end, RGBA color2, float thickness)
    {
        // 计算法向量
        float dy = end.getY() - start.getY();
        float dx = end.getX() - start.getX();
        UIPoint nxy(dx, dy);
        nxy.normalize();
        dy = nxy.getY();
        dx = nxy.getX();

        dx *= (thickness * 0.5f);
        dy *= (thickness * 0.5f);

        UIPoint tl{start.getX() + dy, start.getY() - dx};
        UIPoint tr{end.getX() + dy, end.getY() - dx};
        UIPoint bl{start.getX() - dy, start.getY() + dx};
        UIPoint br{end.getX() - dy, end.getY() + dx};

        genRect(tl, color, tr, color2, bl, color, br, color2);
    }

    void UIHelper::drawTriangle(const UIPoint &a, RGBA color,
                                const UIPoint &b, RGBA color2,
                                const UIPoint &c, RGBA color3, float thickness)
    {
    }

    void UIHelper::fillTriangle(const UIPoint &a, RGBA color,
                                const UIPoint &b, RGBA color2,
                                const UIPoint &c, RGBA color3)
    {
        auto idx = mDrawList.mVertices.size();
        // 先生成顶点
        mDrawList.mVertices.push_back(UIVertex(a, color));
        mDrawList.mVertices.push_back(UIVertex(b, color2));
        mDrawList.mVertices.push_back(UIVertex(c, color3));
        // 在生成索引
        mDrawList.mIndices.push_back(idx);
        mDrawList.mIndices.push_back(idx + 1);
        mDrawList.mIndices.push_back(idx + 2);
    }

    void UIHelper::drawRect(const UIArea &area, RGBA tlc, RGBA trc, RGBA blc, RGBA brc, float thickness)
    {
        auto tl = area.getTL();
        auto tr = area.getTR();
        auto bl = area.getBL();
        auto br = area.getBR();
        float hw = thickness * 0.5f;
        drawLine({tl.mX - hw, tl.mY}, tlc, {tr.mX + hw, tr.mY}, trc, thickness);
        drawLine(tr, trc, br, brc, thickness);
        drawLine({br.mX + hw, br.mY}, brc, {bl.mX - hw, bl.mY}, blc, thickness);
        drawLine(bl, blc, tl, tlc, thickness);
    }

    void UIHelper::drawRect(const UIPoint &tl, const UIPoint &br, RGBA tlc, RGBA trc, RGBA blc, RGBA brc, float thickness)
    {
        auto tr = UIPoint(br.getX(), tl.getY());
        auto bl = UIPoint(tl.getX(), br.getY());
        float hw = thickness * 0.5f;
        drawLine({tl.mX - hw, tl.mY}, tlc, {tr.mX + hw, tr.mY}, trc, thickness);
        drawLine(tr, trc, br, brc, thickness);
        drawLine({br.mX + hw, br.mY}, brc, {bl.mX - hw, bl.mY}, blc, thickness);
        drawLine(bl, blc, tl, tlc, thickness);
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

    void UIHelper::drawRactTex(const UIPoint &tl, const UIPoint &tluv, const UIPoint &br, const UIPoint &bruv, RGBA color)
    {
        auto tr = UIPoint(br.getX(), tl.getY());
        auto bl = UIPoint(tl.getX(), br.getY());

        auto index = mDrawList.mVertices.size(); // 开始的顶点索引

        UIVertex v0(tl, tluv, color);
        UIVertex v1(tr, {tluv.mY, bruv.mX}, color);

        UIVertex v2(bl, {tluv.mX, bruv.mY}, color);
        UIVertex v3(br, bruv, color);

        mDrawList.mVertices.push_back(v0);
        mDrawList.mVertices.push_back(v1);
        mDrawList.mVertices.push_back(v2);
        mDrawList.mVertices.push_back(v3);

        // 生成索引数据
        mDrawList.mIndices.push_back(index);
        mDrawList.mIndices.push_back(index + 1);
        mDrawList.mIndices.push_back(index + 2);

        mDrawList.mIndices.push_back(index + 1);
        mDrawList.mIndices.push_back(index + 3);
        mDrawList.mIndices.push_back(index + 2);
    }

    void UIHelper::drawCircle(const UIPoint &center, float radius, RGBA in, RGBA out, float thickness, uint32_t segments)
    {
        // 先生成线段的端点
        std::vector<UIPoint> points;
        float angleStep = 2.0f * 3.14159265358979323846f / segments;
        for (int32_t i = 0; i < segments; i++)
        {
            float angle = i * angleStep;
            points.push_back(UIPoint(center.mX + radius * cos(angle), center.mY + radius * sin(angle)));
        }
        auto idx = mDrawList.mVertices.size();
        float hw = thickness * 0.5f;
        printf("start\n");
        // 然后计算每个端点生成矩形的两个点
        for (int32_t i = 0; i < segments; i++)
        {
            auto &p = points[i];
            // 计算法向量
            UIPoint n = UIPoint(p.mX - center.mX, p.mY - center.mY);
            n.normalize();
            auto dx = n.mX * hw;
            auto dy = n.mY * hw;

            auto &v = mDrawList.mVertices.emplace_back();
            v.mColor = out;
            v.mXY.mX = p.mX + dx;
            v.mXY.mY = p.mY + dy;

            printf("%f,%f\n", v.mXY.mX, v.mXY.mY);

            auto &v2 = mDrawList.mVertices.emplace_back();
            v2.mColor = in;
            v2.mXY.mX = p.mX - dx;
            v2.mXY.mY = p.mY - dy;
            printf("%f,%f\n", v2.mXY.mX, v2.mXY.mY);
        }
        printf("end\n");
        // 最后通过生成矩形端点绘制矩形
        for (int32_t i = 0; i < segments; i++)
        {
            auto start = i * 2+idx;
            auto end = ((i + 1) % segments) * 2+idx;

            // 生成索引数据
            mDrawList.mIndices.push_back(end);
            mDrawList.mIndices.push_back(end + 1);
            mDrawList.mIndices.push_back(start);

            mDrawList.mIndices.push_back(end + 1);
            mDrawList.mIndices.push_back(start + 1);
            mDrawList.mIndices.push_back(start);
        }
    }
    void UIHelper::fillCircle(const UIPoint &center, float radius, RGBA in, RGBA out, uint32_t segments)
    {
        // 先生成顶点
        float angleStep = 2.0f * 3.14159265358979323846f / segments;
        auto ic = mDrawList.mVertices.size();
        auto &c = mDrawList.mVertices.emplace_back();
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
        // 然后与圆点组成三角形
        for (int32_t i = 0; i < segments; i++)
        {
            auto start = i;
            auto end = ((i + 1) % segments);
            mDrawList.mIndices.push_back(ic);
            mDrawList.mIndices.push_back(ip2 + start);
            mDrawList.mIndices.push_back(ip2 + end);
        }
    }
    void UIHelper::genRect(const UIPoint &tl, RGBA tlc, const UIPoint &tr, RGBA trc,
                           const UIPoint &bl, RGBA blc, const UIPoint &br, RGBA brc)
    {
        auto index = mDrawList.mVertices.size(); // 开始的顶点索引

        UIVertex v0(tl, tlc);
        UIVertex v1(tr, trc);

        UIVertex v2(bl, blc);
        UIVertex v3(br, brc);

        mDrawList.mVertices.push_back(v0);
        mDrawList.mVertices.push_back(v1);
        mDrawList.mVertices.push_back(v2);
        mDrawList.mVertices.push_back(v3);

        // 生成索引数据
        mDrawList.mIndices.push_back(index);
        mDrawList.mIndices.push_back(index + 1);
        mDrawList.mIndices.push_back(index + 2);

        mDrawList.mIndices.push_back(index + 1);
        mDrawList.mIndices.push_back(index + 3);
        mDrawList.mIndices.push_back(index + 2);
    }
}
