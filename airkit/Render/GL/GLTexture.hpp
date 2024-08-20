#ifndef __GLTEXTURE_H__
#define __GLTEXTURE_H__

#include <airkit/GUI/Render/ITexture.hpp>

namespace airkit
{
    struct GLTexture2D : public ITexture2D
    {
        GLTexture2D(const TextureSpecification &spec);
        GLTexture2D(const std::string &path);
        virtual ~GLTexture2D();

        virtual void bind(uint32_t slot = 0) override;
        virtual void unbind() override;
        virtual void setData(void *data, uint32_t size) override;

        virtual ColorFormat getColorFormat() const override { return mSpecification.mFormat; }
        virtual const TextureSpecification &getSpecification() const { return mSpecification; }
        virtual bool isLoaded() const override { return mIsLoaded; }
        virtual const std::string &getPath() const override { return mPath; }

        virtual uint32_t getWidth() const override { return mSpecification.mWidth; }
        virtual uint32_t getHeight() const override { return mSpecification.mHeight; }

    private:
        TextureSpecification mSpecification; // 纹理规格描述
        std::string mPath;
        bool mIsLoaded = false;
        uint32_t mResID;
        uint32_t mInternalFormat; // 内部格式
        uint32_t mSlot = -1;      // 绑定的纹理槽
        static uint32_t GLInternalFormat(ColorFormat format);
    };
}
#endif // __GLTEXTURE_H__