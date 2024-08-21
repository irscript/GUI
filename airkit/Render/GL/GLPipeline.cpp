#include <airkit/Render/GL/GLPipeline.hpp>
#include <airkit/Render/GL/GLRender.hpp>
namespace airkit
{
    GLPipeline::GLPipeline(const std::string &name,
                           const VertexLayout &layout,
                           const ShaderHolder &shader)
        : mName(name), mLayout(layout),
          mShader(shader)
    {
    }
    GLPipeline::~GLPipeline()
    {
    }
    void GLPipeline::bind()
    {
        auto &gl=getGlDriver();
        mShader->bind();
        gl.Enable (GL_MULTISAMPLE);
    }
    void GLPipeline::unbind()
    {
        mShader->unbind();
    }
    const std::string &GLPipeline::getName() const { return mName; }
    const VertexLayout &GLPipeline::getVertexLayout() const { return mLayout; }
    const ShaderHolder &GLPipeline::getShader() const { return mShader; }
}
