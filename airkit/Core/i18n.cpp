#include <airkit/Core/i18n.hpp>
#include <airkit/Core/Crc32.hpp>
#include <airkit/Core/RWFile.hpp>
namespace airkit
{
    I18n::~I18n()
    {
        mBuffer.clear();
        mData = nullptr;
    }
    bool I18n::load(const std::string &filename, const uint32_t version)
    {
        if (mData != nullptr)
        {
            mBuffer.clear();
            mData = nullptr;
        }
        if (readFile(filename, mBuffer) == false)
            return false;

        mData = (FileHeader *)mBuffer.data();
        ;
        // 检查数据是否有效
        if (false == checkData(version, mBuffer.size()))
        {
            mBuffer.clear();
            mData = nullptr;
            return false;
        }

        return true;
    }
    const char *I18n::translate(uint32_t id)
    {
        const char *ret = "$$-Unknown-ID-$$";
        if (mData == nullptr)
            return ret;
        // 从0连续的值
        if (mData->mSerial == -1)
        {
            if (id < mData->mTotal)
            {
                auto item = mData->mTsls[id].mTsl;
                ret = item->mStr;
            }
            return ret;
        }
        // 不连续，使用二分法查找
        size_t start = 0;
        size_t end = mData->mTotal - 1;
        auto item = mData->mTsls[end].mTsl;
        if (item->mEnum < id)
            return ret;

        // 开始二分查找
        while (start <= end)
        {

            auto mid = (start + end) / 2;
            auto item = mData->mTsls[mid].mTsl;
            if (item->mEnum == id)
            {
                ret = item->mStr;
                break;
            }
            else if (item->mEnum > id)
            {
                end = mid - 1;
            }
            else
            {
                start = mid + 1;
            }
        }

        return ret;
    }
    bool I18n::checkData(const uint32_t version, const size_t total) const
    {
        // 检查魔数标识
        if (mData->mMagic[0] != 'i' ||
            mData->mMagic[1] != '1' ||
            mData->mMagic[2] != '8' ||
            mData->mMagic[3] != 'n')
            return false;

        // 计算文件hash
        auto data = (const uint8_t *)mData;
        uint32_t hash = Crc32::calc(data + 8, total - 8, 0);
        // 比对文件hash
        if (hash != mData->mHash)
            return false;
        // 检查文件格式
        if (mData->mFormat != sizeof(uintptr_t) * 8)
            return false;
        // 检查版本
        if (mData->mVersion != version)
            return false;

        // 开始重定位翻译单元项
        uintptr_t base = (uintptr_t)mData + sizeof(FileHeader) + mData->mTotal * sizeof(uintptr_t);
        for (size_t i = 0; i < mData->mTotal; i++)
        {
            auto offset = mData->mTsls[i].mOffset;
            mData->mTsls[i].mTsl = (TslItem *)(base + offset);
        }

        return true;
    }
}
