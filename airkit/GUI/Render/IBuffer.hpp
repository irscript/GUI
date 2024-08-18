#pragma once
#include <airkit/GUI/Render/VertexLayout.hpp>
namespace airkit
{

    struct IBuffer
    {
        virtual ~IBuffer() = default;

        virtual void bind() = 0;
        virtual void unbind() = 0;
    };

        // 顶点缓冲：VBO
    struct IVertexBuffer : public IBuffer
    {
        virtual ~IVertexBuffer() = default;
        virtual const VertexLayout &getLayout() const = 0;
        // 设置顶点数据
        virtual void setData(const void *data, uint32_t size) = 0;
    };

    // 索引缓冲：IBO
    struct IIndexBuffer : public IBuffer
    {
        virtual ~IIndexBuffer() = default;
        virtual uint32_t getCount() const = 0;
        // 设置索引数据
        virtual void setData(const void *data, uint32_t size, uint32_t count) = 0;
    };
    using VBOHolder = std::shared_ptr<IVertexBuffer>;
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

    using VAOHolder = std::shared_ptr<IVertexArray>;
}