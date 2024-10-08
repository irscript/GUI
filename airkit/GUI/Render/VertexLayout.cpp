#include <airkit/GUI/Render/VertexLayout.hpp>
#include <airkit/GUI/IPlat.hpp>
#include "VertexLayout.hpp"

namespace airkit
{
    uint32_t getShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
            return 4;
        case ShaderDataType::Float2:
            return 4 * 2;
        case ShaderDataType::Float3:
            return 4 * 3;
        case ShaderDataType::Float4:
            return 4 * 4;
        case ShaderDataType::Mat3:
            return 4 * 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4 * 4;
        case ShaderDataType::Int:
            return 4;
        case ShaderDataType::Int2:
            return 4 * 2;
        case ShaderDataType::Int3:
            return 4 * 3;
        case ShaderDataType::Int4:
            return 4 * 4;
        case ShaderDataType::Bool:
            return 1;
        }

        checkError(false, "Unknown ShaderDataType:{}!", uint32_t(type));
        return 0;
    }
    uint32_t getComponentCount(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
            return 1;
        case ShaderDataType::Float2:
            return 2;
        case ShaderDataType::Float3:
            return 3;
        case ShaderDataType::Float4:
            return 4;
        case ShaderDataType::Mat3:
            return 3; // 3* float3
        case ShaderDataType::Mat4:
            return 4; // 4* float4
        case ShaderDataType::Int:
            return 1;
        case ShaderDataType::Int2:
            return 2;
        case ShaderDataType::Int3:
            return 3;
        case ShaderDataType::Int4:
            return 4;
        case ShaderDataType::Bool:
            return 1;
        }

        checkError(false, "Unknown ShaderDataType:{}!", uint32_t(type));
        return 0;
    }
    uint32_t VertexElement::getComponentCount() const { return ::airkit::getComponentCount(mType); }

    VertexElement::VertexElement(const std::string &name, ShaderDataType type, bool normalized)
        : mName(name), mType(type),
          mSize(getShaderDataTypeSize(type)),
          mOffset(0), mNormalized(normalized) {}

}
