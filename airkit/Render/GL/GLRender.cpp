#include <airkit/Render/GL/GLShader.hpp>
#include <airkit/Render/GL/GLRender.hpp>
#include <airkit/Render/GL/GLBuffer.hpp>
#include <airkit/Render/GL/GLTexture.hpp>
#include <airkit/Render/GL/GLPipeline.hpp>
#include <airkit/GUI/IPlat.hpp>
#include "GLRender.hpp"

namespace airkit
{
    RenderAPI GLRender::getAPI() const { return RenderAPI::OpenGL; }
    const char *GLRender::getAPIName() const { return "OpenGL"; }

    bool GLRender::init(void *data)
    {
        mVersion = gladLoadGLContext(&gl, (GLADloadfunc)data);
        return mVersion != 0;
    }
    void GLRender::shutdown()
    {
    }
    void GLRender::clear() { gl.Clear(GL_COLOR_BUFFER_BIT); }
    void GLRender::clearColor(float r, float g, float b, float a) { gl.ClearColor(r, g, b, a); }
    void GLRender::setViewport(int x, int y, int width, int height) { gl.Viewport(x, y, width, height); }
    void GLRender::setScissor(int x, int y, int width, int height) { gl.Scissor(x, y, width, height); }
    ShaderHolder GLRender::createShader(const std::string &name, const std::string &vertex, const std::string &fragment, bool isfile)
    {
        std::unordered_map<uint32_t, const char *> codes;
        if (isfile == true)
        {
            codes[GL_VERTEX_SHADER] = readFile(vertex).c_str();
            codes[GL_FRAGMENT_SHADER] = readFile(fragment).c_str();
        }
        else
        {
            codes[GL_VERTEX_SHADER] = vertex.c_str();
            codes[GL_FRAGMENT_SHADER] = fragment.c_str();
        }
        return createShader(name, codes);
    }
    ShaderHolder GLRender::createShader(const std::string &name, const std::string &src, bool isfile)
    {
        std::unordered_map<uint32_t, const char *> codes;
        std::vector<std::string> srclist;

        // 解析分段
        if (true == isfile)
        {
            std::string source = readFile(src);
            splitShader(source, name, srclist, codes);
        }
        else
        {
            splitShader(src, name, srclist, codes);
        }
        // 创建着色器
        return createShader(name, codes);
    }
    PipelineHolder GLRender::createPipeline(const std::string &name,
                                            const VertexLayout &layout,
                                            const ShaderHolder &shader)
    {
        return PipelineHolder(new GLPipeline(name, layout, shader));
    }
    VBOHolder GLRender::createVertexBuffer(const VertexLayout &layout, const void *data, uint32_t size) { return VBOHolder(new GLVertexBuffer(layout, data, size)); }
    VBOHolder GLRender::createVertexBuffer(const VertexLayout &layout, uint32_t size) { return VBOHolder(new GLVertexBuffer(layout, size)); }
    IBOHolder GLRender::createIndexBuffer(const void *data, uint32_t size, uint32_t count) { return IBOHolder(new GLIndexBuffer(data, size, count)); }
    IBOHolder GLRender::createIndexBuffer(uint32_t size) { return IBOHolder(new GLIndexBuffer(size)); }
    VAOHolder GLRender::createVertexArray() { return VAOHolder(new GLVertexArray()); }
    UBOHolder GLRender::createUniformBuffer(uint32_t size, uint32_t binding) { return UBOHolder(new GLUniformBuffer(size, binding)); }
    TextureHolder GLRender::createTexture2D(const TextureSpecification &spec) { return TextureHolder(new GLTexture2D(spec)); }
    TextureHolder GLRender::createTexture2D(const std::string &path) { return TextureHolder(new GLTexture2D(path)); }
    void GLRender::drawIndexs(uint32_t offset, uint32_t count, bool isI32)
    {
        uint32_t type;
        uintptr_t realoffset;
        if (isI32 == true)
        {
            type = GL_UNSIGNED_INT;
            realoffset = offset * sizeof(uint32_t);
        }
        else
        {
            type = GL_UNSIGNED_SHORT;
            realoffset = offset * sizeof(uint16_t);
        }

        gl.DrawElements(GL_TRIANGLES, count, type, (void *)realoffset);
        GL_CHECK();
    }
    void GLRender::drawVertices(uint32_t offset, uint32_t count)
    {
        gl.DrawArrays(GL_TRIANGLES, offset, count);
        GL_CHECK();
    }

}
