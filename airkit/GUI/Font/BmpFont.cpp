#include "BmpFont.hpp"
#include <airkit/GUI/Font/BmpFont.hpp>
#include <airkit/Core/RWFile.hpp>
#include <airkit/Core/Crc32.hpp>
#include <airkit/GUI/UI/UIHelper.hpp>
namespace airkit
{
    bool BmpFont::load(const std::string &path, RenderHolder &render)
    {
        if (readFile(path, mData) == false)
            return false;

        FontHeader *header = (FontHeader *)mData.data();
        if (header->magic[0] != 'b' || header->magic[1] != 'f' ||
            header->magic[2] != 'n' || header->magic[3] != 't')
            return false;
        if (false == Crc32::check(mData.data() + sizeof(FontHeader), mData.size() - sizeof(FontHeader), header->hash))
            return false;
        mMeta = (FontMeta *)(mData.data() + sizeof(FontHeader));
        mGlyphs = (GlyphTable *)(mData.data() + mMeta->glyphOffset);
        mImages = (ImageTable *)(mData.data() + mMeta->imageOffset);

        // 创建纹理
        std::string filename = path.substr(0, path.find_last_of('.')) + "_";
        std::string texpath;
        mTextures.resize(mImages->count);
        for (int i = 0; i < mImages->count; ++i)
        {
            texpath = filename + std::to_string(i) + ".png";
            auto tex = render->createTexture2D(texpath);
            mTextures[i] = tex;
        }
        return true;
    }
    void BmpFont::drawText(UIDrawList &drawlist, const UIArea &clip, const std::string &text, const uint32_t flag, RGBA color, const float size)
    {
        if (text.empty())
            return;

        // 计算字体缩放
        float scale = size / mMeta->size;

        // 计算行高
        float lineHeight = mMeta->size * scale * mMeta->lineHeight;
        float x = clip.mX+10;
        float y = clip.mY + size+10;

        auto &cmd = drawlist.begin();
        cmd.mTexture = mTextures[0];
        UIHelper ui(drawlist);
        for (auto ch : text)
        {
            auto glyph = getGlyph(ch);
            if (glyph)
            {
                // 计算顶点
                float t = y - 32;//glyph->plane.t * mMeta->size ;
                float b = y ;//- glyph->plane.b * mMeta->size ;
                float l = x ;//+ glyph->plane.l * mMeta->size ;
                float r = x +32;// glyph->plane.r * mMeta->size ;

                ui.texRect(
                    UIPoint(l, t),
                    UIPoint(glyph->image.l, glyph->image.t),
                    UIPoint(r, b),
                    UIPoint(glyph->image.r, glyph->image.b),
                    color);
                x += glyph->advance  * mMeta->size;
            }
            else
                x += size;
        }
        auto glyph = getGlyph('~');
        auto l=142.5f/204;
        auto r=148.5f/204;
        auto t=168.5f/204;
        auto b=141.5f/204;
        ui.texRect(
                    UIPoint(10, 90),
                    UIPoint(glyph->image.l, glyph->image.t),
                    UIPoint(42, 122),
                    UIPoint(glyph->image.r, glyph->image.b),
                    color);

        
        
        drawlist.end(cmd, DrawFlag::MTSDF);

        
    }
    UISize BmpFont::calcSize(const std::string &text, const float size)
    {
        return UISize();
    }
    BmpFont::Glyph *BmpFont::getGlyph(uint32_t code)
    {
        for (int i = 0; i < mGlyphs->count; ++i)
        {
            if (mGlyphs->table[i].code == code)
                return &mGlyphs->table[i];
        }
        return nullptr;
    }
}
