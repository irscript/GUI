#pragma once

#include <airkit/GUI/Render/RenderEnum.hpp>

namespace airkit
{

    constexpr uint32_t getShaderDataTypeSize(ShaderDataType type);
    constexpr uint32_t getComponentCount(ShaderDataType type);

    // 顶点元素
    struct VertexElement
    {
        std::string mName;
        ShaderDataType mType;
        uint32_t mSize;
        uint32_t mOffset;
        bool mNormalized;

        VertexElement(const std::string &name, ShaderDataType type, bool normalized = false);
        uint32_t getComponentCount() const;
    };
    // 顶点元素布局
    struct VertexLayout
    {
    public:
        VertexLayout() = default;
        VertexLayout(const std::initializer_list<VertexElement> &elements)
            : mElements(elements) { calculateOffsetsAndStride(); }
        const std::vector<VertexElement> &getElements() const { return mElements; }
        uint32_t getStride() const { return mStride; }
        const VertexElement &operator[](uint32_t index) const { return mElements[index]; }

        auto begin() { return mElements.begin(); }
        auto end() { return mElements.end(); }
        auto begin() const { return mElements.begin(); }
        auto end() const { return mElements.end(); }

    private:
        void calculateOffsetsAndStride()
        {
            uint32_t offset = 0;
            mStride = 0;
            for (auto &element : mElements)
            {
                element.mOffset = offset;
                offset += element.mSize;
                mStride += element.mSize;
            }
        }

    private:
        std::vector<VertexElement> mElements;
        uint32_t mStride = 0; // 步长:顶点元素总字节数
    };
}