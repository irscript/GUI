#pragma once

#include <airkit/GUI/Render/IPipeline.hpp>

namespace airkit
{
    struct GLPipeline : public IPipeline
    {
    public:
        GLPipeline(const std::string &name, const VertexLayout &layout, ShaderWatcher shader);
        ~GLPipeline();

        virtual void bind() override;
        virtual void unbind() override;
        virtual const std::string &getName() const override;
        virtual const VertexLayout &getVertexLayout() const override;
        virtual const ShaderWatcher &getShader() const override;

    protected:
        std::string mName;
        VertexLayout mLayout;
        ShaderWatcher mShader;
    };
}