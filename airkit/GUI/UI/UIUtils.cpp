#include "UIUtils.hpp"
#include <airkit/GUI/UI/UIUtils.hpp>
namespace airkit
{
    void UIUtils::drawLine(const UIPoint &start, const UIPoint &end, RGBA color, float thickness)
    {
        addPoint(start, color);
        addPoint(end, color);
        //生成绘制数据
    }

    inline void UIUtils::addPoint(const UIPoint &point, RGBA color) { mPath.push_back({point, color}); }

}
