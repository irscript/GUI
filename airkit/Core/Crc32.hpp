#ifndef __CRC32_H__
#define __CRC32_H__

#include <airkit/AIR_PCH.hpp>

namespace airkit
{
    struct Crc32
    {

        /// @brief 计算 CRC32
        /// @param data 待计算的数据
        /// @param size 数据的大小
        /// @param hash 初始哈希值
        /// @return
        static uint32_t calc(const void *data, size_t size, uint32_t hash = 0);

        /// @brief 校验数据的 CRC32 是否一致
        /// @param data 待计算的数据
        /// @param size 数据的大小
        /// @param hash 校验 CRC32 值
        /// @return
        static bool check(const void *data, size_t size, uint32_t hash)
        {
            uint32_t calval = calc(data, size);
            return calval == hash;
        }

    private:
        static const uint32_t crc32_table[256];
    };
}
#endif // __CRC32_H__