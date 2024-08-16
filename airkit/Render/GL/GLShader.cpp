#include <airkit/Render/GL/GLShader.hpp>
#include <airkit/Render/GL/GLRender.hpp>
#include <airkit/GUI/IPlat.hpp>
#include "GLShader.hpp"
namespace airkit
{
    inline auto &getGlDriver()
    {
        auto rh = IPlat::getInstance().getRender();
        auto glr = (GLRender *)rh.get();
        return glr->getGL();
    }
    GLShader::~GLShader()
    {
        auto &gl = getGlDriver();
        gl.DeleteProgram(mResID);
    }
    void GLShader::bind()
    {
        auto &gl = getGlDriver();
        gl.UseProgram(mResID);
    }
    void GLShader::unbind()
    {
        auto &gl = getGlDriver();
        gl.UseProgram(0);
    }

    void GLShader::setInt(const std::string &name, int32_t value) { upInt(name, value); }
    void GLShader::setIntArray(const std::string &name, uint32_t count, const int32_t *const value) { upIntArray(name, count, value); }
    void GLShader::setFloat(const std::string &name, float value) { upFloat(name, value); }
    void GLShader::setFloat2(const std::string &name, const float *const value) { upFloat2(name, value); }
    void GLShader::setFloat3(const std::string &name, const float *const value) { upFloat3(name, value); }
    void GLShader::setFloat4(const std::string &name, const float *const value) { upFloat4(name, value); }

    void GLShader::setMat3(const std::string &name, const float *const value) { upMat3(name, value); }
    void GLShader::setMat4(const std::string &name, const float *const value) { upMat4(name, value); }

    void GLShader::upInt(const std::string &name, int32_t value)
    {
        auto &gl = getGlDriver();
        auto loc = gl.GetUniformLocation(mResID, name.c_str());
        gl.Uniform1i(loc, value);
    }

    void GLShader::upIntArray(const std::string &name, uint32_t count, const int32_t *const value)
    {
        auto &gl = getGlDriver();
        auto loc = gl.GetUniformLocation(mResID, name.c_str());
        gl.Uniform1iv(loc, count, value);
    }
    void GLShader::upFloat(const std::string &name, float value)
    {
        auto &gl = getGlDriver();
        auto loc = gl.GetUniformLocation(mResID, name.c_str());
    }
    void GLShader::upFloat2(const std::string &name, const float *const value)
    {
        auto &gl = getGlDriver();
        auto loc = gl.GetUniformLocation(mResID, name.c_str());
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
