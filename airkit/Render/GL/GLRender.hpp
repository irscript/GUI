#ifndef __GLRENDER_H__
#define __GLRENDER_H__

#include <airkit/GUI/Render/IRender.hpp>
#include <airkit/GUI/IPlat.hpp>
#include <airkit/3Part/Glad2/glad/gl.h>

namespace airkit
{

    struct GLRender : public IRender
    {
    public:
        GLRender() = default;
        ~GLRender() = default;

        GladGLContext &getGL() { return gl; }

    public:
        virtual RenderAPI getAPI() const override;
        virtual const char *getAPIName() const override;

        virtual bool init(void *data = nullptr) override;
        virtual void shutdown() override;

        virtual void clear() override;
        virtual void clearColor(float r, float g, float b, float a) override;

        // 设置设置视口
        virtual void setViewport(int x, int y, int width, int height) override;
        // 设置裁剪区域
        virtual void setScissor(int x, int y, int width, int height) override;

        virtual ShaderHolder createShader(const std::string &name, const std::string &vertex, const std::string &fragment, bool isfile) override;
        virtual ShaderHolder createShader(const std::string &name, const std::string &src, bool isfile) override;

        // 创建渲染管线
        virtual PipelineHolder createPipeline(const std::string &name, const VertexLayout &layout, const ShaderHolder &shader) override;

        // 创建顶点缓存
        virtual VBOHolder createVertexBuffer(const VertexLayout &layout, const void *vertices, uint32_t size) override;
        virtual VBOHolder createVertexBuffer(const VertexLayout &layout, uint32_t size) override;
        // 创建索引缓存
        virtual IBOHolder createIndexBuffer(const void *indices, uint32_t size, uint32_t count) override;
        virtual IBOHolder createIndexBuffer(uint32_t size) override;
        // 创建顶点数组
        virtual VAOHolder createVertexArray() override;

        // 创建uniform buffer
        virtual UBOHolder createUniformBuffer(uint32_t size, uint32_t binding) override;
        // 创建纹理
        virtual TextureHolder createTexture2D(const TextureSpecification &spec) override;
        virtual TextureHolder createTexture2D(const std::string &path) override;

        // 索引绘制
        virtual void drawIndexs(uint32_t offset, uint32_t count, bool isI32) override;
        // 顶点绘制
        virtual void drawVertices(uint32_t offset, uint32_t count) override;

    private:
        std::string readFile(const std::string &filepath);
        void splitShader(const std::string &src, const std::string &name, std::vector<std::string> &sublist, std::unordered_map<uint32_t, const char *> &result);
        ShaderHolder createShader(const std::string &name, std::unordered_map<uint32_t, const char *> &src);
        static const char *getShaderStageName(uint32_t stage);
        static uint32_t getShaderStage(const std::string &type);

    private:
        GladGLContext gl;
        uint32_t mVersion; // 版本号

    };

    inline GladGLContext &getGlDriver()
    {
        auto rh = IPlat::getInstance().getRender();
        auto glr = (GLRender *)rh.get();
        return glr->getGL();
    }

#ifdef GL_DEBUG
    void glCheckError(GladGLContext &gl, const char *file, int line);
#define GL_CHECK() glCheckError(gl, thisfile(), thisline())
#else
#define GL_CHECK()
#endif

}
#endif // __GLRENDER_H__