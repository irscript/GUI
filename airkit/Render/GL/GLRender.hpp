#ifndef __GLRENDER_H__
#define __GLRENDER_H__

#include <airkit/GUI/Render/IRender.hpp>
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

        virtual ShaderWatcher createShader(const std::string &name, const std::string &vertex, const std::string &fragment,bool isfile) override;
        virtual ShaderWatcher createShader(const std::string &name, const std::string &file) override;

    private:
        std::string readFile(const std::string &filepath);
        ShaderWatcher createShader(const std::string &name, std::unordered_map<uint32_t, const char *> &src);
        static const char *getShaderStageName(uint32_t stage);
        static uint32_t getShaderStage( const std::string& type);

    private:
        GladGLContext gl;
        uint32_t mVersion; // 版本号
    };
}
#endif // __GLRENDER_H__