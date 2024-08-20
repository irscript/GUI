#include <airkit/Render/GL/GLShader.hpp>
#include <airkit/Render/GL/GLRender.hpp>
#include <airkit/Render/GL/GLBuffer.hpp>
#include <airkit/Render/GL/GLPipeline.hpp>
#include <airkit/GUI/IPlat.hpp>
#define FMT_HEADER_ONLY
#include <airkit/3Part/Fmt/core.h>

namespace airkit
{
   std::string GLRender::readFile(const std::string &filepath)
    {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII

        checkError(in, "Could not open file '{0}'", filepath);

        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        checkError(size != -1, "Could not read from file '{0}'", filepath);

        result.resize(size);
        in.seekg(0, std::ios::beg);
        in.read(&result[0], size);

        return result;
    }
    void GLRender::splitShader(const std::string &source, const std::string &name,
                               std::vector<std::string> &srclist,
                               std::unordered_map<uint32_t, const char *> &codes)
    {
        const char *typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); // Start of shader type declaration line
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); // End of shader type declaration line
            checkError(eol != std::string::npos, "shader file '{0}' syntax error", name);

            size_t begin = pos + typeTokenLength + 1; // Start of shader type name (after "#type " keyword)
            std::string type = source.substr(begin, eol - begin);
            auto stage = getShaderStage(type);
            checkError(stage != 0, "shader file '{0}' Invalid shader type specified {1}", name, type);

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); // Start of shader code after shader type declaration line
            checkError(nextLinePos != std::string::npos, "shader file '{0}' syntax error", name);

            pos = source.find(typeToken, nextLinePos); // Start of next shader type declaration line
            auto src = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
            srclist.push_back(src);
            codes[stage] = srclist.back().c_str();
        }
    }
    ShaderHolder GLRender::createShader(const std::string &name, std::unordered_map<uint32_t, const char *> &src)
    {

        // 先编译着色器
        std::vector<uint32_t> shaders;
        for (auto &it : src)
        {
            auto stage = it.first;
            auto sid = shaders.emplace_back(gl.CreateShader(stage));
            const char *code = it.second;
            gl.ShaderSource(sid, 1, &code, nullptr);
            gl.CompileShader(sid);
            int32_t status;
            gl.GetShaderiv(sid, GL_COMPILE_STATUS, &status);
            if (status == 0)
            {
                GLint len = 0;
                gl.GetShaderiv(sid, GL_INFO_LOG_LENGTH, &len);
                std::string log;
                log.resize(len);
                gl.GetShaderInfoLog(sid, len, nullptr, log.data());
                checkError(false, "Shader '{0}.{1}' compile failed!\nlog: {2}", name, getShaderStageName(stage), log);
            }
        }
        // 再链接着色器
        uint32_t pid = gl.CreateProgram();
        for (auto &it : shaders)
            gl.AttachShader(pid, it);
        gl.LinkProgram(pid);
        int32_t status;
        gl.GetProgramiv(pid, GL_LINK_STATUS, &status);
        if (status == 0)
        {
            auto code = gl.GetError();
            GLint len = 0;
            gl.GetProgramiv(pid, GL_INFO_LOG_LENGTH, &len);
            std::string log;
            log.resize(len);
            gl.GetProgramInfoLog(pid, len, nullptr, log.data());
            checkError(false, "Program '{0}' compile failed!\ncode: {1}\nlog: {2}", name, code, log);
        }
        for (auto &it : shaders)
            gl.DeleteShader(it);

        // 返回Shader
        ShaderHolder shader(new GLShader(name, pid));

        return shader;
    }
    const char *GLRender::getShaderStageName(uint32_t stage)
    {
        switch (stage)
        {
        case GL_VERTEX_SHADER:
            return "opengl.vert";
        case GL_FRAGMENT_SHADER:
            return "opengl.frag";
        }
        checkError(false, "unknown shader stage:{0}", stage);
        return nullptr;
    }
    uint32_t GLRender::getShaderStage(const std::string &type)
    {
        if (type == "vertex")
            return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel")
            return GL_FRAGMENT_SHADER;

        return 0;
    }

#ifdef GL_DEBUG
    void glCheckError(GladGLContext &gl, const char *file, int line)
    {
        GLenum err = gl.GetError();
        if (err != GL_NO_ERROR)
        {
            std::string error;
            switch (err)
            {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            default:
                error = "UNKNOWN_ERROR";
                break;
            }

            printf("[OpenGL Error] %s:%d> %d => %s\n", file, line, err, error.c_str());
        }
    }
#endif
}
