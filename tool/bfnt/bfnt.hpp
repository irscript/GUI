#ifndef __BFNT_H__
#define __BFNT_H__

#include <cstdint>

// 字体图元
struct Glyph
{
    uint32_t imgae; // 所属的纹理id
    uint32_t code;  // 字符编码: UTF-32编码
    float advance;  // 字符宽度:以字体大小为单位
    struct
    {
        float l, r, t, b;
    } image,   // 纹理坐标
        plane; // 平面坐标: 以字体大小为单位，基于原点偏移,用于提示如何绘制该字符
};
// 字体表
struct GlyphTable
{
    uint32_t count;
    Glyph table[];
};
// 纹理表
struct ImageTable
{
    uint32_t count;
    struct
    {
        uint32_t width, height;
    } table[];
};
// 字体元数据
struct FontMeta
{
    char magic[4]; // 魔数: BFNT
    uint32_t hash; // 字体哈希值

    float size;               // 字体大小
    float lineHeight;         // 行高: 以字体大小为单位
    float distanceRange;      // 距离场
    float underlineY;         // 下划线位置: 以字体大小为单位，基于原点偏移,
    float underlineThickness; // 下划线厚度: 以字体大小为单位

    uint32_t glyphOffset;
    uint32_t imageOffset;
};

#endif // __BFNT_H__