
#include <airkit/GUI/UI/UIHelper.hpp>
#include "UIHelper.hpp"

namespace airkit
{

    void UIHelper::texRect(const UIPoint &tl, const UIPoint &tluv, const UIPoint &br, const UIPoint &bruv, const RGBA *const color)
    {
        auto tr = UIPoint(br.getX(), tl.getY());
        auto bl = UIPoint(tl.getX(), br.getY());

        auto index = mDrawList.mVertices.size(); // 开始的顶点索引

        UIVertex v0(tl, tluv, color[0]);
        UIVertex v1(tr, {tluv.mY, bruv.mX}, color[1]);

        UIVertex v2(bl, {tluv.mX, bruv.mY}, color[2]);
        UIVertex v3(br, bruv, color[3]);

        mDrawList.needVertex(4);

        mDrawList.mVertices.push_back(v0);
        mDrawList.mVertices.push_back(v1);
        mDrawList.mVertices.push_back(v2);
        mDrawList.mVertices.push_back(v3);

        // 生成索引数据
        mDrawList.needIndex(6);

        mDrawList.mIndices.push_back(index);
        mDrawList.mIndices.push_back(index + 2);
        mDrawList.mIndices.push_back(index + 1);

        mDrawList.mIndices.push_back(index + 1);
        mDrawList.mIndices.push_back(index + 2);
        mDrawList.mIndices.push_back(index + 3);
    }

    void UIHelper::texRoundRect(const UIPoint &tl, const UIPoint &tluv,
                                const UIPoint &br, const UIPoint &bruv,
                                const RGBA *const color, const float *radius)
    {
        auto sradian = 3.14159265358979323846f / 180.0f; // 0.01745329252
        auto subRadian = 90 * sradian;
        // 左上圆角
        float rtl = radius[0];
        uint32_t rtli[3] = {};
        if (rtl > 0.0f)
        {
            //计算圆角段
            //uint32_t segments = std::max(segments, static_cast<int>(rtl * 0.1f));
        }
    }
}
