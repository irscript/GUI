#ifndef __GLSHADER_H__
#define __GLSHADER_H__

#include <airkit/GUI/Render/IShader.hpp>

namespace airkit
{

    struct GLShader : public IShader
    {
        GLShader(const std::string &name, uint32_t id) : IShader(name), mResID(id) {}
        virtual ~GLShader();

        virtual void bind() override;
        virtual void unbind() override;

        virtual void setInt(const std::string &name, int32_t value) override;
        virtual void setIntArray(const std::string &name, uint32_t count, const int32_t *const value) override;
        virtual void setFloat(const std::string &name, float value) override;
        virtual void setFloat2(const std::string &name, const float *const value) override;
        virtual void setFloat3(const std::string &name, const float *const value) override;
        virtual void setFloat4(const std::string &name, const float *const value) override;
        virtual void setMat3(const std::string &name, const float *const value) override;
        virtual void setMat4(const std::string &name, const float *const value) override;

        void upInt(const std::string &name, int32_t value);
        void upIntArray(const std::string &name, uint32_t count, const int32_t *const value);
        void upFloat(const std::string &name, float value);
        void upFloat2(const std::string &name, const float *const value);
        void upFloat3(const std::string &name, const float *const value);
        void upFloat4(const std::string &name, const float *const value);
        void upMat3(const std::string &name, const float *const value);
        void upMat4(const std::string &name, const float *const value);

    private:
        uint32_t mResID; // 着色器资源ID
    };
}

#endif // __GLSHADER_H__