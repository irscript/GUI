#include <airkit/Render/GL/GLTexture.hpp>
#include <airkit/Render/GL/GLRender.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <airkit/3Part/stb/stb_image.h>
namespace airkit
{

    inline uint32_t GLTexture2D::GLInternalFormat(ColorFormat format)
    {
        switch (format)
        {
        case ColorFormat::RGB24:
            return GL_RGB;
        case ColorFormat::RGBA32:
            return GL_RGBA;
        }

        checkError(false, "Unknown ColorFormat!");
        return 0;
    }
    GLTexture2D::GLTexture2D(const TextureSpecification &spec)
        : mSpecification(spec)
    {
        mInternalFormat = GLInternalFormat(spec.mFormat);
        auto &gl = getGlDriver();

        gl.CreateTextures(GL_TEXTURE_2D, 1, &mResID);
        GL_CHECK();
        gl.TextureStorage2D(mResID, 1, mInternalFormat,
                            mSpecification.mWidth,
                            mSpecification.mHeight);
        GL_CHECK();

        gl.TextureParameteri(mResID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        GL_CHECK();
        gl.TextureParameteri(mResID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GL_CHECK();
        gl.TextureParameteri(mResID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        GL_CHECK();
        gl.TextureParameteri(mResID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        GL_CHECK();
    }
    GLTexture2D::GLTexture2D(const std::string &path)
        : mPath(path)
    {
        auto &gl = getGlDriver();

        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc *data = nullptr;
        data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        checkError(data, "{}:Failed to load image!", path);
        if (data)
        {
            mIsLoaded = true;
            mSpecification.mWidth = width;
            mSpecification.mHeight = height;

            if (channels == 4)
            {
                mInternalFormat = GL_RGBA;
                mSpecification.mFormat = ColorFormat::RGBA32;
            }
            else if (channels == 3)
            {
                mInternalFormat = GL_RGB;
                mSpecification.mFormat = ColorFormat::RGB24;
            }
            else
            {
                checkError(false, "{}:Unknown image format!", path);
            }

            gl.CreateTextures(GL_TEXTURE_2D, 1, &mResID);
            GL_CHECK();
            //gl.TextureStorage2D(mResID, 1, mInternalFormat, width, height);
            //GL_CHECK();

            // 设置纹理采样参数
            gl.TextureParameteri(mResID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            GL_CHECK();
            gl.TextureParameteri(mResID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            GL_CHECK();
            gl.TextureParameteri(mResID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            GL_CHECK();
            gl.TextureParameteri(mResID, GL_TEXTURE_WRAP_T, GL_REPEAT);
            GL_CHECK();
            // 设置纹理数据
            gl.BindTexture(GL_TEXTURE_2D, mResID);
            GL_CHECK();
            gl.TexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, width, height, 0, mInternalFormat, GL_UNSIGNED_BYTE, data);
            GL_CHECK();

            stbi_image_free(data);
        }
    }
    GLTexture2D::~GLTexture2D()
    {
        auto &gl = getGlDriver();
        gl.DeleteTextures(1, &mResID);
        GL_CHECK();
    }
    void GLTexture2D::bind(uint32_t slot)
    {
        auto &gl = getGlDriver();
        gl.BindTextureUnit(slot, mResID);
        //gl.BindTexture(GL_TEXTURE_2D, mResID);
        GL_CHECK();
        mSlot = slot;
    }
    void GLTexture2D::unbind()
    {
        auto &gl = getGlDriver();
        gl.BindTextureUnit(mSlot, 0);
        GL_CHECK();
        mSlot = -1;
    }
    void GLTexture2D::setData(void *data, uint32_t size)
    {
        auto &gl = getGlDriver();
        uint32_t bpp = mInternalFormat == GL_RGBA ? 4 : 3;
        auto w = mSpecification.mWidth;
        auto h = mSpecification.mHeight;
        checkError(size == w * h * bpp, "Data must be entire texture!");
        gl.TextureSubImage2D(mResID, 0, 0, 0, w, h, mInternalFormat, GL_UNSIGNED_BYTE, data);
        GL_CHECK();
    }

}
