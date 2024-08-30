#include <memory>
#include <cstdio>
#include <vector>
#include <artery-font/artery-font.h>
#include <artery-font/std-artery-font.h>
#include <fstream>
#include <string>
#include <../crc32.inl>
#include <bfnt.hpp>

size_t tsize = 0;
size_t rsize = 0;
int readHelper(void *dst, int length, void *data)
{
    if (rsize + length > tsize)
        return -1;

    memcpy(dst, (char *)data + rsize, length);
    rsize += length;
    return length;
};

int main(int argc, char **argv)
{
    std::ifstream file("F:\\gitee\\gui\\res\\ascii.arfont", std::ios::binary);
    if (!file.is_open())
    {
        printf("Failed to open file! \n");
        return -1;
    }
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer;
    buffer.resize(fileSize);
    if (!file.read((char *)&buffer[0], fileSize))
    {
        printf("Failed to read file! \n");
        return -1;
    }
    tsize = buffer.size();

    artery_font::StdArteryFont<float> arteryFont;

    auto result = artery_font::decode<&readHelper, float, artery_font::StdList,
                                      artery_font::StdByteArray,
                                      artery_font::StdString>(arteryFont, buffer.data());
    float as = 0, ds = 0;
    auto &v = arteryFont.variants[0];
    BinBuffer glyphs, images;
    glyphs.push(uint32_t(v.glyphs.length())); // 字形数量
    for (int i = 0; i < v.glyphs.length(); i++)
    {
        auto &g = v.glyphs[i];
        glyphs.push(0u);
        glyphs.push(g.codepoint);
        glyphs.push(g.advance.h);

        float l= g.imageBounds.l / arteryFont.images[0].width;
        float r= g.imageBounds.r / arteryFont.images[0].width;
        float t= g.imageBounds.t / arteryFont.images[0].height;
        float b= g.imageBounds.b / arteryFont.images[0].height;

        glyphs.push(l);
        glyphs.push(r);
        glyphs.push(t);
        glyphs.push(b);

        glyphs.push(g.planeBounds.l);
        glyphs.push(g.planeBounds.r);
        glyphs.push(g.planeBounds.t);
        glyphs.push(g.planeBounds.b);

        ds = std::min(ds, g.planeBounds.b);
        as = std::max(as, g.planeBounds.t);
    }
    images.push(1u);
    images.push(arteryFont.images[0].width);
    images.push(arteryFont.images[0].height);

    printf("Descent: %f, Ascent: %f\n", v.metrics.descender, v.metrics.ascender);
    printf("Descent: %f, Ascent: %f\n", ds, as);

    BinBuffer metadata;
    metadata.push(uint8_t('b'));
    metadata.push(uint8_t('f'));
    metadata.push(uint8_t('n'));
    metadata.push(uint8_t('t'));

    metadata.push(uint32_t(0));

    metadata.push(v.metrics.fontSize);
    metadata.push(v.metrics.lineHeight);
    metadata.push(v.metrics.distanceRange);
    metadata.push(v.metrics.underlineY);
    metadata.push(v.metrics.underlineThickness);

    uint32_t base = sizeof(FontMeta);
    metadata.push(base);
    metadata.push(base + uint32_t(glyphs.getSize()));

    metadata.push(glyphs);
    metadata.push(images);

    metadata.hash();

    // 10.保存到文件中
    std::string fntBin = "demo.bfnt";
    FILE *fout = fopen(fntBin.c_str(), "wb");
    if (fout == nullptr)
        ::printf("[ %s ]：创建并打开文件失败!\n", fntBin.c_str());
    else
    {
        ::printf("[ %s ]：写入文件...\n", fntBin.c_str());
        fwrite(metadata.getData().data(), sizeof(uint8_t), metadata.getSize(), fout);
        fclose(fout);
    }
    return 0;
}