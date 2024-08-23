#ifndef __BITOP_H__
#define __BITOP_H__

#include <airkit/AIR_PCH.hpp>

namespace airkit
{

    namespace Bitop
    {

        /// @brief 通过位移操作创建一个特定位置为1的掩码。
        /// @param bit 指定需要设置为1的位的位置。
        /// @return 返回一个T类型的值，其中指定的bit位被设置为1，其他位为0。
        template <typename T>
        static constexpr T bit(uint32_t bit) { return (T(1) << bit); }

        /// @brief 将指定的位设置为1。
        /// @param value 要修改的原始值。
        /// @param bit  要设置为1的位的位置，从0开始计数，最低位为第0位。
        /// @return 修改后的值，其中指定的位被设置为1。
        template <typename T>
        static constexpr T set(T value, uint32_t bit) { return value | (T(1) << bit); }

        /// @brief 将指定的位设置为0。
        /// @param value 要修改的原始值。
        /// @param bit  要设置为0的位的位置，从0开始计数，最低位为第0位。
        /// @return 修改后的值，其中指定的位被设置为0。
        template <typename T>
        static constexpr T reset(T value, uint32_t bit) { return value & ~(T(1) << bit); }

        /// @brief 检查给定值的特定位是否被设置为1
        /// @param value 待检查的值。
        /// @param bit 要检查的位的位置，从0开始计数，最低位为第0位。
        /// @return 如果给定值的特定位被设置为1，则返回true，否则返回false。
        template <typename T>
        static constexpr bool check(T value, uint32_t bit) { return (value & (T(1) << bit)) != 0; }
        /// @brief 翻转给定值的特定位
        /// @param value 待翻转的值。
        /// @param bit 要翻转的位的位置，从0开始计数，最低位为第0位。
        /// @return 修改后的值，其中指定的位被翻转。
        template <typename T>
        static constexpr T flip(T value, uint32_t bit) { return value ^ (T(1) << bit); }

        /// @brief 循环左移
        /// @param value 要进行循环左移的值。
        /// @param shift 左移的位数，必须是非负整数。
        /// @return 循环左移后的结果值。
        template <typename T>
        static constexpr T rol(T value, uint32_t shift)
        {
            auto width = sizeof(T) * 8;
            shift &= width - 1;
            return (value << shift) | (value >> (width - shift));
        }
        /// @brief 循环右移
        /// @param value 要进行循环右移的值。
        /// @param shift 右移的位数，必须是非负整数。
        /// @return 循环右移后的结果值。
        template <typename T>
        static constexpr T ror(T value, uint32_t shift)
        {
            auto width = sizeof(T) * 8;
            shift &= width - 1;
            return (value >> shift) | (value << (width - shift));
        }
    };

    struct Bit32
    {
        Bit32(uint32_t value = 0) : mValue(value) {}

        // 置位
        void set(uint32_t bit) { mValue = Bitop::set<uint32_t>(mValue, bit); }
        void setMask(uint32_t mask) { mValue |= mask; }
        // 复位
        void reset(uint32_t bit) { mValue = Bitop::reset<uint32_t>(mValue, bit); }
        void resetMask(uint32_t mask) { mValue &= ~mask; }
        // 检查位
        bool check(uint32_t bit) const { return Bitop::check<uint32_t>(mValue, bit); }
        uint32_t checkMask(uint32_t mask) const { return (mValue & mask); }
        // 位翻转
        void flip(uint32_t bit) { mValue = Bitop::flip<uint32_t>(mValue, bit); }
        void flipMask(uint32_t mask) { mValue ^= mask; }
        // 获取值
        uint32_t getValue() const { return mValue; }
        // 设置值
        void setValue(uint32_t value) { mValue = value; }

    protected:
        uint32_t mValue;
    };

    struct Bit64
    {
        Bit64(uint32_t value = 0) : mValue(value) {}

        // 置位
        void set(uint32_t bit) { mValue = Bitop::set(mValue, bit); }
        // 复位
        void reset(uint32_t bit) { mValue = Bitop::reset(mValue, bit); }
        // 检查位
        bool check(uint32_t bit) const { return Bitop::check(mValue, bit); }
        // 位翻转
        void flip(uint32_t bit) { mValue = Bitop::flip(mValue, bit); }
        // 获取值
        uint64_t getValue() const { return mValue; }
        // 设置值
        void setValue(uint64_t value) { mValue = value; }

    protected:
        uint64_t mValue;
    };
}
#endif // __BITOP_H__