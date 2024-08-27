#ifndef __ITEXTURE_H__
#define __ITEXTURE_H__

#include <airkit/GUI/Render/RenderEnum.hpp>
#include<airkit/Core/Uncopyable.hpp>
namespace airkit
{
    struct TextureSpecification
    {
        uint32_t mWidth = 1;
        uint32_t mHeight = 1;
        ColorFormat mFormat = ColorFormat::RGBA32;
        bool mGenerateMips = true;
    };

    struct ITexture:public Uncopyable
    {
        virtual void bind(uint32_t slot = 0) = 0;
        virtual void unbind() = 0;
        virtual void setData(void *data, uint32_t size) = 0;

        virtual ColorFormat getColorFormat() const = 0;
        virtual const TextureSpecification &getSpecification() const = 0;
        virtual bool isLoaded() const = 0;
        virtual const std::string &getPath() const = 0;

        virtual ~ITexture() = default;
    };

    struct ITexture2D : public ITexture
    {
        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;
        virtual ~ITexture2D() = default;
    };

    using TextureHolder = Holder<ITexture2D>;
}
#endif // __ITEXTURE_H__