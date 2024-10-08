#ifndef __GLBUFFER_H__
#define __GLBUFFER_H__

#include <airkit/GUI/Render/IBuffer.hpp>
#include <airkit/Render/GL/GLRender.hpp>
namespace airkit
{

    struct GLVertexBuffer : public IVertexBuffer
    {
    public:
        GLVertexBuffer(const VertexLayout &layout, uint32_t isize);
        GLVertexBuffer(const VertexLayout &layout, const void *vertices, uint32_t size);
        ~GLVertexBuffer();

        void bind() override;
        void unbind() override;

        virtual const VertexLayout &getLayout() const override;
        // 设置顶点数据
        virtual void setData(const void *data, uint32_t size) override;
        virtual const uint32_t getSize() const override;

    private:
        uint32_t mSize;
        uint32_t mResID;
        const VertexLayout &mLayout;
        // 禁止拷贝
        NO_COPY(GLVertexBuffer);
    };

    struct GLIndexBuffer : public IIndexBuffer
    {
    public:
        GLIndexBuffer(uint32_t isize);
        GLIndexBuffer(const void *indices, uint32_t isize, uint32_t icount);
        ~GLIndexBuffer();

        void bind() override;
        void unbind() override;
        virtual uint32_t getCount() const override;
        // 设置索引数据
        virtual void setData(const void *idata, uint32_t isize, uint32_t icount) override;
        virtual const uint32_t getSize() const override;

    private:
        uint32_t mSize;
        uint32_t mResID;
        uint32_t mCount;
    };

    struct GLVertexArray : public IVertexArray
    {
    public:
        GLVertexArray();
        ~GLVertexArray();

        void bind() override;
        void unbind() override;

        virtual void addVertexBuffer(VBOHolder vbo) override;
        virtual void setIndexBuffer(IBOHolder ibo) override;
        virtual const std::vector<VBOHolder> &getVertexBuffers() const override;
        virtual const IBOHolder &getIndexBuffer() const override;

    private:
        uint32_t mResID;
        uint32_t mVAPos = 0; // 绑定的顶点属性的位置索引
        std::vector<VBOHolder> mVBOs;
        IBOHolder mIBO;

        static GLenum getShaderDataTypeToOpenGLBaseType(ShaderDataType type);
    };

    struct GLUniformBuffer : public IUniformBuffer
    {
    public:
        GLUniformBuffer(uint32_t size, uint32_t binding);
        ~GLUniformBuffer();

        virtual void setData(const void *data, uint32_t size, uint32_t offset = 0) override;

    private:
        uint32_t mResID;
    };
}
#endif // __GLBUFFER_H__