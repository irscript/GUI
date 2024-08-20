#ifndef __GLPIPELINE_H__
#define __GLPIPELINE_H__

#include <airkit/GUI/Render/IPipeline.hpp>

namespace airkit
{
    struct GLPipeline : public IPipeline
    {
    public:
        GLPipeline(const std::string &name, const VertexLayout &layout, const ShaderHolder &shader);
        ~GLPipeline();

        virtual void bind() override;
        virtual void unbind() override;
        virtual const std::string &getName() const override;
        virtual const VertexLayout &getVertexLayout() const override;
        virtual const ShaderHolder &getShader() const override;

    protected:
        std::string mName;
        VertexLayout mLayout;
        ShaderHolder mShader;

    };
}
#endif // __GLPIPELINE_H__