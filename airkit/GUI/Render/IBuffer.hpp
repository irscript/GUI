#ifndef __IBUFFER_H__
#define __IBUFFER_H__

#include <airkit/GUI/Render/VertexLayout.hpp>
#include<airkit/Core/Uncopyable.hpp>
namespace airkit
{

    struct IBuffer:public Uncopyable
    {
        virtual ~IBuffer() = default;

        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual const uint32_t getSize() const = 0;
    };

    // 顶点缓冲：VBO
    struct IVertexBuffer : public IBuffer
    {
        virtual ~IVertexBuffer() = default;
        virtual const VertexLayout &getLayout() const = 0;
        // 设置顶点数据
        virtual void setData(const void *vertices, uint32_t size) = 0;
    };

    // 索引缓冲：IBO
    struct IIndexBuffer : public IBuffer
    {
        virtual ~IIndexBuffer() = default;
        virtual uint32_t getCount() const = 0;
        // 设置索引数据
        virtual void setData(const void *data, uint32_t size, uint32_t count) = 0;
    };
    // vertex buffer
    using VBOHolder = std::shared_ptr<IVertexBuffer>;
    // index buffer
    using IBOHolder = std::shared_ptr<IIndexBuffer>;
    // 顶点数组：VAO
    struct IVertexArray
    {
        virtual ~IVertexArray() = default;
        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual void addVertexBuffer(VBOHolder vbo) = 0;
        virtual void setIndexBuffer(IBOHolder ibo) = 0;
        virtual const std::vector<VBOHolder> &getVertexBuffers() const = 0;
        virtual const IBOHolder &getIndexBuffer() const = 0;
    };
    // vertex array
    using VAOHolder = std::shared_ptr<IVertexArray>;

    // uniform buffer
    struct IUniformBuffer
    {
        virtual ~IUniformBuffer() = default;
        virtual void setData(const void *data, uint32_t size, uint32_t offset = 0) = 0;
    };
    // uniform buffer
    using UBOHolder = std::shared_ptr<IUniformBuffer>;
}
#endif // __IBUFFER_H__