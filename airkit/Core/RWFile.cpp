#include <airkit/Core/RWFile.hpp>

namespace airkit
{
    bool readFile(const std::string &filename, std::vector<uint8_t> &data)
    {
        // 文件是否存在
        std::filesystem::path path(filename);
        if (!std::filesystem::exists(path))
            return false;
        // 获取文件大小
        auto size = std::filesystem::file_size(path);
        if (size == 0)
            return false;
        // 打开文件
        FILE *fp = fopen(filename.c_str(), "rb");
        if (fp == nullptr)
            return false;

        // 读取文件
        data.resize(size);
        uint8_t *dataptr = data.data();
        if (fread(dataptr, 1, size, fp) != size)
        {
            data.clear(); // 关闭文件
            fclose(fp);
            return false;
        }
        // 关闭文件
        fclose(fp);
        return true;
    }

    bool writeFile(const std::string &path, const std::vector<uint8_t> &data)
    {
        // 打开文件
        FILE *fp = fopen(path.c_str(), "wb");
        if (fp == nullptr)
            return false;

        // 读取文件
        const uint8_t *dataptr = data.data();
        fwrite(dataptr, 1, data.size(), fp);
        // 关闭文件
        fclose(fp);
        return true;
    }

}
