
#include <airkit/GUI/UI/UIHelper.hpp>
#include "UIHelper.hpp"

namespace airkit
{
    

    void UIHelper::genRect(const UIPoint &tl, RGBA tlc, const UIPoint &tr, RGBA trc,
                           const UIPoint &bl, RGBA blc, const UIPoint &br, RGBA brc)
    {
        auto index = mDrawList.mVertices.size(); // 开始的顶点索引

        UIVertex v0(tl, tlc);
        UIVertex v1(tr, trc);

        UIVertex v2(bl, blc);
        UIVertex v3(br, brc);

        mDrawList.needVertex(4);

        mDrawList.mVertices.push_back(v0);
        mDrawList.mVertices.push_back(v1);
        mDrawList.mVertices.push_back(v2);
        mDrawList.mVertices.push_back(v3);

        mDrawList.needIndex(6);
        // 生成索引数据
        mDrawList.mIndices.push_back(index);
        mDrawList.mIndices.push_back(index + 2);
        mDrawList.mIndices.push_back(index + 1);

        mDrawList.mIndices.push_back(index + 1);
        mDrawList.mIndices.push_back(index + 2);
        mDrawList.mIndices.push_back(index + 3);
    }
}
