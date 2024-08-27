#ifndef __ISHADER_H__
#define __ISHADER_H__

#include <airkit/GUI/Render/RenderEnum.hpp>
#include<airkit/Core/Uncopyable.hpp>
namespace airkit
{

    // 着色器程序
    struct IShader:public Uncopyable
    {
        virtual ~IShader() =default;

        IShader(const std::string &name) : mName(name) {}

        const std::string &getName() const { return mName; }

        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual void setInt(const std::string &name, int32_t value) = 0;
        virtual void setIntArray(const std::string &name, uint32_t count, const int32_t *const value) = 0;
        virtual void setFloat(const std::string &name, float value) = 0;
        virtual void setFloat2(const std::string &name, const float *const value) = 0;
        virtual void setFloat3(const std::string &name, const float *const value) = 0;
        virtual void setFloat4(const std::string &name, const float *const value) = 0;
        virtual void setMat3(const std::string &name, const float *const value) = 0;
        virtual void setMat4(const std::string &name, const float *const value) = 0;

        virtual uint32_t bindUniformBuffer(const std::string &name, const uint32_t binding) = 0;

    private:
        std::string mName;
    };

    using ShaderHolder = Holder<IShader>;
    using ShaderWatcher = Watcher<IShader>;

    // 着色器程序库
    struct ShaderLibrary
    {
        void add(ShaderHolder shader) { add(shader->getName(), shader); }
        void add(const std::string &name, ShaderHolder shader) { mShaders.insert(std::make_pair(name, shader)); }
        ShaderWatcher get(const std::string &name) { return mShaders.find(name)->second; }

        void clear() { mShaders.clear(); }

    private:
        std::unordered_map<std::string, ShaderHolder> mShaders;
    };

}
#endif // __ISHADER_H__