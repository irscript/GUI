#include <airkit/Render/GL/GLShader.hpp>
#include <airkit/Render/GL/GLRender.hpp>
#include <airkit/GUI/IPlat.hpp>
namespace airkit
{

    GLShader::~GLShader()
    {
        auto &gl = getGlDriver();
        gl.DeleteProgram(mResID);
        GL_CHECK();
    }
    void GLShader::bind()
    {
        auto &gl = getGlDriver();
        gl.UseProgram(mResID);
        GL_CHECK();
    }
    void GLShader::unbind()
    {
        auto &gl = getGlDriver();
        gl.UseProgram(0);
        GL_CHECK();
    }

    void GLShader::setInt(const std::string &name, int32_t value) { upInt(name, value); }
    void GLShader::setIntArray(const std::string &name, uint32_t count, const int32_t *const value) { upIntArray(name, count, value); }
    void GLShader::setFloat(const std::string &name, float value) { upFloat(name, value); }
    void GLShader::setFloat2(const std::string &name, const float *const value) { upFloat2(name, value); }
    void GLShader::setFloat3(const std::string &name, const float *const value) { upFloat3(name, value); }
    void GLShader::setFloat4(const std::string &name, const float *const value) { upFloat4(name, value); }

    void GLShader::setMat3(const std::string &name, const float *const value) { upMat3(name, value); }
    void GLShader::setMat4(const std::string &name, const float *const value) { upMat4(name, value); }

    uint32_t GLShader::bindUniformBuffer(const std::string &name, const uint32_t binding)
    {
        auto &gl = getGlDriver();
        uint32_t index = gl.GetUniformBlockIndex(mResID, name.c_str());
        GL_CHECK();
        gl.UniformBlockBinding(mResID, index, binding);
        GL_CHECK();
        int32_t ubosize = 0;
        gl.GetActiveUniformBlockiv(mResID, index, GL_UNIFORM_BLOCK_DATA_SIZE, &ubosize);
        GL_CHECK();
        return ubosize;
    }

    void GLShader::upInt(const std::string &name, int32_t value)
    {
        auto &gl = getGlDriver();
        auto loc = gl.GetUniformLocation(mResID, name.c_str());
        GL_CHECK();
        gl.Uniform1i(loc, value);
        GL_CHECK();
    }

    void GLShader::upIntArray(const std::string &name, uint32_t count, const int32_t *const value)
    {
        auto &gl = getGlDriver();
        auto loc = gl.GetUniformLocation(mResID, name.c_str());
        GL_CHECK();
        gl.Uniform1iv(loc, count, value);
        GL_CHECK();
    }
    void GLShader::upFloat(const std::string &name, float value)
    {
        auto &gl = getGlDriver();
        auto loc = gl.GetUniformLocation(mResID, name.c_str());
        GL_CHECK();
        gl.Uniform1f(loc, value);
        GL_CHECK();
    }
    void GLShader::upFloat2(const std::string &name, const float *const value)
    {
        auto &gl = getGlDriver();
        auto loc = gl.GetUniformLocation(mResID, name.c_str());
        GL_CHECK();
        gl.Uniform2f(loc, value[0], value[1]);
        GL_CHECK();
    }
    void GLShader::upFloat3(const std::string &name, const float *const value)
    {
        auto &gl = getGlDriver();
        auto loc = gl.GetUniformLocation(mResID, name.c_str());
    }
    void GLShader::upFloat4(const std::string &name, const float *const value)
    {
        auto &gl = getGlDriver();
        auto loc = gl.GetUniformLocation(mResID, name.c_str());
    }
    void GLShader::upMat3(const std::string &name, const float *const value)
    {
        auto &gl = getGlDriver();
        auto loc = gl.GetUniformLocation(mResID, name.c_str());
    }
    void GLShader::upMat4(const std::string &name, const float *const value)
    {
        auto &gl = getGlDriver();
        auto loc = gl.GetUniformLocation(mResID, name.c_str());
    }
}
