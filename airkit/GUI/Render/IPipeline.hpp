#pragma once

#include <airkit/GUI/Render/IShader.hpp>
#include <airkit/GUI/Render/VertexLayout.hpp>

namespace airkit
{

    // 渲染管线：顶点属性布局+固定着色程序
    struct IPipeline
    {
        virtual ~IPipeline() = default;

        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual const std::string &getName() const = 0;
        virtual const VertexLayout &getVertexLayout() const = 0;
        virtual const ShaderHolder &getShader() const = 0;
    };

    using PipelineHolder = std::shared_ptr<IPipeline>;
}
