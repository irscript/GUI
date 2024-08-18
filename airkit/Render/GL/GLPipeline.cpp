#include <airkit/Render/GL/GLPipeline.hpp>
#include "GLPipeline.hpp"
namespace airkit
{
    GLPipeline::GLPipeline(const std::string &name,
                           const VertexLayout &layout,
                           ShaderWatcher shader)
        : mName(name), mLayout(layout),
          mShader(shader)
    {
    }
    GLPipeline::~GLPipeline()
    {
    }
    void GLPipeline::bind()
    {
        mShader.lock()->bind();
    }
    void GLPipeline::unbind()
    {
        mShader.lock()->unbind();
    }
    const std::string &GLPipeline::getName() const { return mName; }
    const VertexLayout &GLPipeline::getVertexLayout() const { return mLayout; }
    const ShaderWatcher &GLPipeline::getShader() const { return mShader; }
}
