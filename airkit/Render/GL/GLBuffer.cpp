
#include <airkit/Render/GL/GLBuffer.hpp>
#include <airkit/Render/GL/GLRender.hpp>

namespace airkit
{
    GLVertexBuffer::GLVertexBuffer(const VertexLayout &layout, uint32_t size)
        : mLayout(layout), mSize(size)
    {
        auto &gl = getGlDriver();
        gl.CreateBuffers(1, &mResID);
        GL_CHECK();
        gl.BindBuffer(GL_ARRAY_BUFFER, mResID);
        GL_CHECK();
        gl.BufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STREAM_DRAW);
        GL_CHECK();
        gl.BindBuffer(GL_ARRAY_BUFFER, 0);
        GL_CHECK();
    }
    GLVertexBuffer::GLVertexBuffer(const VertexLayout &layout, const void *vertices, uint32_t size)
        : mLayout(layout), mSize(size)
    {
        auto &gl = getGlDriver();
        gl.CreateBuffers(1, &mResID);
        GL_CHECK();
        gl.BindBuffer(GL_ARRAY_BUFFER, mResID);
        GL_CHECK();
        gl.BufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        GL_CHECK();
        gl.BindBuffer(GL_ARRAY_BUFFER, 0);
        GL_CHECK();
    }
    GLVertexBuffer::~GLVertexBuffer()
    {
        auto &gl = getGlDriver();
        gl.DeleteBuffers(1, &mResID);
        GL_CHECK();
    }
    void GLVertexBuffer::bind()
    {
        auto &gl = getGlDriver();
        gl.BindBuffer(GL_ARRAY_BUFFER, mResID);
        GL_CHECK();
    }
    void GLVertexBuffer::unbind()
    {
        auto &gl = getGlDriver();
        gl.BindBuffer(GL_ARRAY_BUFFER, 0);
        GL_CHECK();
    }
    const VertexLayout &GLVertexBuffer::getLayout() const { return mLayout; }

    void GLVertexBuffer::setData(const void *vertices, uint32_t size)
    {
        auto &gl = getGlDriver();
        gl.BindBuffer(GL_ARRAY_BUFFER, mResID);
        GL_CHECK();
        if (size > mSize)
        {
            gl.BufferData(GL_ARRAY_BUFFER, size, vertices, GL_STREAM_DRAW);
            GL_CHECK();
            mSize = size;
        }
        else
        {
            gl.BufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
            GL_CHECK();
        }

        gl.BindBuffer(GL_ARRAY_BUFFER, 0);
        GL_CHECK();
    }

    const uint32_t GLVertexBuffer::getSize() const { return mSize; }

    GLIndexBuffer::GLIndexBuffer(uint32_t size)
        : mCount(0), mSize(size)
    {
        auto &gl = getGlDriver();
        gl.CreateBuffers(1, &mResID);
        GL_CHECK();
        gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mResID);
        GL_CHECK();
        gl.BufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STREAM_DRAW);
        GL_CHECK();
        gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        GL_CHECK();
    }
    GLIndexBuffer::GLIndexBuffer(const void *indices, uint32_t size, uint32_t count)
        : mCount(count)
    {
        auto &gl = getGlDriver();
        gl.CreateBuffers(1, &mResID);
        GL_CHECK();
        gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mResID);
        GL_CHECK();
        gl.BufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
        GL_CHECK();
        gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        GL_CHECK();
    }
    GLIndexBuffer::~GLIndexBuffer()
    {
        auto &gl = getGlDriver();
        gl.DeleteBuffers(1, &mResID);
        GL_CHECK();
    }
    void GLIndexBuffer::bind()
    {
        auto &gl = getGlDriver();
        gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mResID);
        GL_CHECK();
    }
    void GLIndexBuffer::unbind()
    {
        auto &gl = getGlDriver();
        gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mResID);
        GL_CHECK();
    }
    uint32_t GLIndexBuffer::getCount() const { return mCount; }
    void GLIndexBuffer::setData(const void *idata, uint32_t isize, uint32_t icount)
    {
        auto &gl = getGlDriver();
        gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mResID);
        GL_CHECK();
        if (isize > mSize)
        {
            gl.BufferData(GL_ELEMENT_ARRAY_BUFFER, isize, idata, GL_STREAM_DRAW);
            GL_CHECK();
            mSize = isize;
        }
        else
        {
            gl.BufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, isize, idata);
            GL_CHECK();
        }

        mCount = icount;
        gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        GL_CHECK();
    }

    const uint32_t GLIndexBuffer::getSize() const { return mSize; }

    GLVertexArray::GLVertexArray()
    {
        auto &gl = getGlDriver();
        gl.CreateVertexArrays(1, &mResID);
        GL_CHECK();
    }
    GLVertexArray::~GLVertexArray()
    {
        auto &gl = getGlDriver();
        gl.DeleteVertexArrays(1, &mResID);
        GL_CHECK();
    }
    void GLVertexArray::bind()
    {
        auto &gl = getGlDriver();
        gl.BindVertexArray(mResID);
        GL_CHECK();
    }
    void GLVertexArray::unbind()
    {
        auto &gl = getGlDriver();
        gl.BindVertexArray(0);
        GL_CHECK();
    }
    GLenum GLVertexArray::getShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
            return GL_FLOAT;
        case ShaderDataType::Float2:
            return GL_FLOAT;
        case ShaderDataType::Float3:
            return GL_FLOAT;
        case ShaderDataType::Float4:
            return GL_FLOAT;
        case ShaderDataType::Mat3:
            return GL_FLOAT;
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
            return GL_INT;
        case ShaderDataType::Int2:
            return GL_INT;
        case ShaderDataType::Int3:
            return GL_INT;
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
        }
        checkError(false, "Unknown ShaderDataType:{}!", uint32_t(type));
        return 0;
    }
    void GLVertexArray::addVertexBuffer(VBOHolder vbo)
    {
        auto &gl = getGlDriver();
        gl.BindVertexArray(mResID);
        vbo->bind();
        // 设置顶点属性
        const auto &layout = vbo->getLayout();
        for (const auto &element : layout)
        {
            switch (element.mType)
            {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            {
                gl.EnableVertexAttribArray(mVAPos);
                GL_CHECK();
                gl.VertexAttribPointer(mVAPos,
                                       element.getComponentCount(),
                                       getShaderDataTypeToOpenGLBaseType(element.mType),
                                       element.mNormalized ? GL_TRUE : GL_FALSE,
                                       layout.getStride(),
                                       (const void *)(uintptr_t)element.mOffset);
                GL_CHECK();
                mVAPos++;
                break;
            }
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool:
            {
                gl.EnableVertexAttribArray(mVAPos);
                GL_CHECK();
                gl.VertexAttribIPointer(mVAPos,
                                        element.getComponentCount(),
                                        getShaderDataTypeToOpenGLBaseType(element.mType),
                                        layout.getStride(),
                                        (const void *)(uintptr_t)element.mOffset);
                GL_CHECK();
                mVAPos++;
                break;
            }
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
            {
                uint8_t count = element.getComponentCount();
                for (uint8_t i = 0; i < count; i++)
                {
                    gl.EnableVertexAttribArray(mVAPos);
                    GL_CHECK();
                    gl.VertexAttribPointer(mVAPos,
                                           count,
                                           getShaderDataTypeToOpenGLBaseType(element.mType),
                                           element.mNormalized ? GL_TRUE : GL_FALSE,
                                           layout.getStride(),
                                           (const void *)(element.mOffset + sizeof(float) * count * i));
                    GL_CHECK();
                    gl.VertexAttribDivisor(mVAPos, 1);
                    GL_CHECK();
                    mVAPos++;
                }
                break;
            }
            default:
                checkError(false, "Unknown ShaderDataType:{}!", uint32_t(element.mType));
            }
        }

        mVBOs.push_back(vbo);
        gl.BindVertexArray(0);
        GL_CHECK();
        vbo->unbind();
    }
    void GLVertexArray::setIndexBuffer(IBOHolder ibo)
    {
        auto &gl = getGlDriver();
        gl.BindVertexArray(mResID);
        ibo->bind();
        mIBO = ibo;
        gl.BindVertexArray(0);
        GL_CHECK();
    }
    const std::vector<VBOHolder> &GLVertexArray::getVertexBuffers() const { return mVBOs; }
    const IBOHolder &GLVertexArray::getIndexBuffer() const { return mIBO; }

    GLUniformBuffer::GLUniformBuffer(uint32_t size, uint32_t binding)
    {
        auto &gl = getGlDriver();
        gl.CreateBuffers(1, &mResID);
        gl.BindBuffer(GL_UNIFORM_BUFFER, mResID);
        GL_CHECK();
        gl.BufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
        GL_CHECK();
        gl.BindBufferBase(GL_UNIFORM_BUFFER, binding, mResID);
        GL_CHECK();
        gl.BindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    GLUniformBuffer::~GLUniformBuffer()
    {
        auto &gl = getGlDriver();
        gl.DeleteBuffers(1, &mResID);
        GL_CHECK();
    }
    void GLUniformBuffer::setData(const void *data, uint32_t size, uint32_t offset)
    {
        auto &gl = getGlDriver();
        gl.BindBuffer(GL_UNIFORM_BUFFER, mResID);
        GL_CHECK();
        gl.BufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
        GL_CHECK();
        gl.BindBuffer(GL_UNIFORM_BUFFER, 0);
        GL_CHECK();
    }
}
