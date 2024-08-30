#ifndef __I18N_H__
#define __I18N_H__

#include <airkit/AIR_PCH.hpp>
#define FMT_HEADER_ONLY
#include <airkit/3Part/fmt/core.h>
namespace airkit
{
    // 国际化翻译类
    struct I18n
    {
        I18n() : mData(nullptr) {}
        ~I18n();
        // 加载文件,检查版本是否一致
        bool load(const std::string &filename, const uint32_t version);

        // 通过枚举值翻译到字符串
        const char *translate(uint32_t id);

        // 格式化翻译
        template <typename... Args>
        const std::string translate(uint32_t id, Args &&...args)
        {
            const char *str = translate(id);
            return fmt::format(str, std::forward<Args>(args)...);
        }

    private:
        // 翻译单元
        struct TslItem
        {
            uint32_t mEnum; // 枚举ID
            uint16_t mLen;  // 字符串长度
            char mStr[];    // 翻译字符串
        };

        // 文件头
        struct FileHeader
        {
            uint8_t mMagic[4]; // 魔数标识
            uint32_t mHash;    // 文件hash
            uint32_t mFormat; // 文件格式：32位还是64位格式
            uint32_t mVersion; // 文件版本
            uint32_t mTotal;   // 翻译单元数量
            uint32_t mSerial;  // 标识是否连续
            
            uint32_t mSize;    // 翻译单元数据大小
            union
            {
                uintptr_t mOffset; // 翻译单元偏移
                TslItem *mTsl;     // 翻译单元
            } mTsls[];             // 翻译单元数组
        };
        std::vector<uint8_t> mBuffer;
        FileHeader *mData; // 文件数据

        // 检查数据是否有效
        bool checkData(const uint32_t version, const size_t total) const;
    };
}
#endif // __I18N_H__