#include <tsl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include <list>
#include <set>
#include <vector>
#include <filesystem>

const char *useMsg = "命令格式: i18n.exe -i infile infile2 -o outfile [-v 123456] [-t] [-l c/c++] [-h] \n"
                     "选项含义：\n"
                     "\t-i :指示后面是输入文件;\n"
                     "\t-o :指示后面是输出文件\n"
                     "\t-v: 指示文件的版本号,默认为 1;\n"
                     "\t-t: 指示强制分配枚举值,从 0 开始分配;\n"
                     "\t-l: 指示编译输出编程语言代码;\n"
                     "\t\tc  : 指示编译输出 C 编程语言代码;\n"
                     "\t\tc++: 指示编译输出 C++ 编程语言代码;\n\n"
                     "\t注解: 如果没有指定 -l 标志，则只输出二进制文件\n"
                     "\t      否则，输出指定的语言代码，以及二进制文件\n"
                     "\t文件格式：\n"
                     "\t枚举名称,枚举值,翻译字符串\n"
                     "\t\tmenu_edit, 1 , \"编辑\"\n"
                     "\t\tmenu_copy, 2 , \"复制\"\n"
                     "\t\t...\n"
                     "\t其中'#' 字符开始的一行为注释，会被跳过。\n";
using namespace std;
// 输入文件集合
list<string> input;
// 输出文件
string output;
// 输出语言
enum class Language
{
    unknown = 0,
    C,
    Cpp
};
// 输出C语言代码标志
Language lang = Language::unknown;
// 强制重新分配枚举
bool force_flg = false;

// 翻译文件版本号
uint32_t version = 1;

// 枚举项
struct Item
{
    string mName;   // 枚举名称
    uint32_t mEnum; // 枚举值
    string mTsl;    // 映射的转义信息
};
// 枚举项映射
map<string, Item> tslMap;
// 枚举集合
map<uint32_t, const Item &> tslEnum;
// 解析文件的行号
uint32_t tlsLine = 0;

void ParserCmd(int argc, const char *argv[])
{
    do
    {
        // 校验参数个数
        if (argc < 5)
            break;
        // 开始解析参数
        int i = 1;
        while (i < argc)
        {
            // 输入文件
            if (strcmp(argv[i], "-i") == 0)
            {
                ++i;
                if (i >= argc)
                {
                    i = -1;
                    break;
                }
                for (; i < argc; ++i)
                {
                    if (argv[i][0] != '-')
                        input.push_back(argv[i]);
                    else
                        break;
                }
            }
            // 输出文件
            else if (strcmp(argv[i], "-o") == 0)
            {
                ++i;
                if (i >= argc)
                {
                    i = -1;
                    break;
                }
                if (argv[i][0] != '-')
                {
                    output = argv[i];
                    ++i;
                }
                else
                {
                    i = -1;
                    break;
                }
            } // 强制分配枚举值
            else if (strcmp(argv[i], "-t") == 0)
            {
                force_flg = true;
                ++i;
            } // 输出C代码
            else if (strcmp(argv[i], "-v") == 0)
            {
                ++i;
                if (i >= argc)
                {
                    i = -1;
                    break;
                }
                if (argv[i][0] != '-')
                {
                    // 检查版本号必须是数字
                    auto vstr = argv[i];
                    while (*vstr)
                    {
                        if (isdigit(*vstr))
                        {
                            ++vstr;
                        }
                        else
                        {
                            i = -1;
                            break;
                        }
                    }
                    version = stoul(argv[i]);
                    ++i;
                }
                else
                {
                    i = -1;
                    break;
                }
            }
            else if (strcmp(argv[i], "-l") == 0)
            {
                ++i;
                if (i >= argc)
                {
                    i = -1;
                    break;
                }
                if (strcmp(argv[i], "c") == 0)
                {
                    lang = Language::C;
                    ++i;
                }
                else if (strcmp(argv[i], "c++") == 0)
                {
                    lang = Language::Cpp;
                    ++i;
                }
                else
                {
                    i = -1;
                    break;
                }
            }
            // 存在错误
            else
            {
                i = -1;
                break;
            }

        } // 存在错误
        if (i == -1 ||
            input.empty() == true ||
            output.empty() == true)
            break;
        return;
    } while (false);
    printf("命令存在错误！或者输入输出文件为空！\n\n使用帮助：\n");
    printf(useMsg);
    exit(0);
}

// 跳过空白字符
inline void SkipBlank(uint32_t &index, uint32_t size, const char data[])
{
    while (true)
    {
        // 注释
        if (data[index] == '#')
        {
            ++index;
            while (index < size && data[index++] != '\n')
                ;
            ++tlsLine;
            break;
        }
        // 空白字符
        else if (data[index] == ',' ||
                 data[index] == ' ' ||
                 data[index] == '\t' ||
                 data[index] == '\r')
            ++index;
        else if (data[index] == '\n')
        {
            ++tlsLine;
            ++index;
        }
        else
            break;
    }
}
// 解析枚举名称
bool ParserName(uint32_t &index, uint32_t size, const char data[], string &name)
{
    name.clear();
    const char ch = data[index];
    // C语言标识符规则
    if (isalpha(data[index]) == 0 && data[index] != '_')
        return false;
    while (index < size)
    {
        const char ch = data[index];
        if (isalpha(data[index]) != 0 ||
            isdigit(data[index]) != 0 ||
            data[index] == '_')
        {
            name.push_back(data[index++]);
        }
        else
            return true;
    }
    return false;
}
// 解析枚举值
bool ParserEnum(uint32_t &index, uint32_t size, const char data[], uint32_t &nEnum)
{
    string szNum;
    bool hexFlg = false;
    const char ch = data[index];
    // 10/16进制
    while (index < size)
    {
        if ((data[index] == 'x' || data[index] == 'X') &&
            hexFlg == false &&
            szNum.size() == 1 &&
            szNum[0] == '0')
        {
            hexFlg = true;
            szNum.push_back(data[index++]);
        }
        else if (hexFlg == false &&
                 isdigit(data[index]) != 0)
        {
            szNum.push_back(data[index++]);
        }
        else if (hexFlg == true &&
                 (isdigit(data[index]) != 0 ||
                  ((data[index] >= 'a' && data[index] <= 'f') ||
                   (data[index] >= 'A' && data[index] <= 'F'))))
        {
            szNum.push_back(data[index++]);
        }
        else
            break;
    }
    // 十进制
    if (hexFlg == false)
        nEnum = std::atoi(szNum.c_str());
    else // 十六进制
    {
        nEnum = 0;

        for (auto i = 0; i < szNum.size(); ++i)
        {
            if (szNum[i] >= '0' && szNum[i] <= '9')
                nEnum = nEnum * 16 + szNum[i] - '0'; // 0到9的字符串
            else if (szNum[i] >= 'A' && szNum[i] <= 'F')
                nEnum = nEnum * 16 + szNum[i] - 'A' + 10; // 大写十六进制的ABCDEF的字符串的情况
            else if (szNum[i] >= 'a' && szNum[i] <= 'f')
                nEnum = nEnum * 16 + szNum[i] - 'a' + 10;
        }
    }
    return true;
}
// 解析映射信息
bool ParserTsl(uint32_t &index, uint32_t size, const char data[], string &tsl)
{
    tsl.clear();
    // C语言字符串规则
    if (data[index] != '"')
        return false;
    ++index;
    while (index < size)
    {
        if (data[index] == '\\' && index + 1 < size)
        {
            if (data[index + 1] == '\\')
                tsl.push_back('\\');
            else if (data[index + 1] == 't')
                tsl.push_back('\t');
            else if (data[index + 1] == 'r')
                tsl.push_back('\r');
            else if (data[index + 1] == 'n')
                tsl.push_back('\n');
            else if (data[index + 1] == '"')
                tsl.push_back('"');
            else
            {
                ::printf("未知转义字符‘ \\%c’ !\n", data[index + 1]);
                return false;
            }
            index += 2;
        }
        else if (data[index] == '"')
        {
            ++index;
            break;
        }
        else
            tsl.push_back(data[index++]);
    }

    if (tsl.size() > 0xFFFF)
    {
        ::printf("%4d :字符串数据太长(len >65535)！\n", tlsLine);
        return false;
    }

    return true;
}
// 解析文本
bool ParserTxt(const string &path, const string &txt)
{
    uint32_t index = 0;
    const char *data = txt.data();
    Item item;
    uint32_t size = txt.length();
    // 跳过第一行
    SkipBlank(index, size, data);
    bool retFlg = true;
    while (index < size)
    {
        SkipBlank(index, size, data);
        if (index >= size || data[index] == 0)
            break;
        // 解析枚举名称
        if (ParserName(index, size, data, item.mName) == false)
        {
            retFlg = false;
            break;
        }
        SkipBlank(index, size, data);
        if (index >= size)
            break;
        // 解析枚举值
        if (ParserEnum(index, size, data, item.mEnum) == false)
        {
            retFlg = false;
            break;
        }
        SkipBlank(index, size, data);
        if (index >= size)
            break;
        // 解析映射信息
        if (ParserTsl(index, size, data, item.mTsl) == false)
        {
            retFlg = false;
            break;
        }
        SkipBlank(index, size, data);
        /*if (index >= size)
            break;*/
        // 强制分配枚举值
        if (force_flg == true)
            item.mEnum = 0;
        // 校验枚举值是否重复,重复设置为0
        auto eit = tslEnum.find(item.mEnum);
        if (eit != tslEnum.end())
        {
            printf("%4d :警告,枚举名称为 ‘%s’ 与 ‘%s’ 的枚举值存在重复  !\n",
                   tlsLine, item.mName.c_str(), eit->second.mName.c_str());
            item.mEnum = 0;
        }

        // 校验枚举名称是否重复
        if (tslMap.find(item.mName) != tslMap.end())
        {
            printf("%4d :错误， 枚举名称重复 ‘%s’ !\n",
                   item.mName.c_str(), tlsLine);
            return false;
        }
        auto it = tslMap.insert(pair<string, Item>(item.mName, item));
        if (item.mEnum != 0)
            tslEnum.insert(pair<uint32_t, const Item &>(item.mEnum, it.first->second));
    }
    if (retFlg == false)
        printf("%4d :解析文件发生错误!\n", tlsLine);
    return retFlg;
}
void ParserFile()
{
    int32_t errCnt = 0; // 错误计数
    string txt;
    for (auto iter = input.begin(); iter != input.end(); ++iter)
    {
        // 打开文件
        FILE *fp = fopen(iter->c_str(), "r");
        if (fp == nullptr)
        {
            ::printf("[ %s ]：打开文件失败!\n", iter->c_str());
            ::exit(0);
        }
        ::printf("[ %s ]：解析文件... \n", iter->c_str());
        // 获取大小
        fseek(fp, 0, SEEK_END);
        auto size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        // 分配内存
        txt.resize(size);
        // 读取文件
        fread(txt.data(), sizeof(char), size, fp);
        // 解析具体文件
        tlsLine = 1;
        if (ParserTxt(*iter, txt) == false)
            ++errCnt;
        // 关闭文件
        fclose(fp);
    }
    // 如果解析过程中存在错误
    if (errCnt != 0)
    {
        ::printf("解析文件错误！错误文件数: %d \n", errCnt);
        ::exit(0);
    }
}

void CheckEnum()
{
    int32_t nEnum = 0;
    for (auto i = tslMap.begin(); i != tslMap.end(); ++i)
    {
        // 调整枚举值
        if (i->second.mEnum == 0x0)
        {
            while (true)
            {
                if (tslEnum.find(nEnum) != tslEnum.end())
                {
                    ++nEnum;
                    continue;
                }
                else
                    break;
            }
            tslEnum.insert(pair<uint32_t, const Item &>(nEnum, i->second));
            i->second.mEnum = nEnum++;
        }
    }
}
string toHex(uint32_t num, uint32_t min)
{
    static char hexes[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    string hex;
    // min = min | 2;
    uint32_t nibble;
    do
    {
        nibble = num & 0x0f;
        num = num >> 4;
        hex = hexes[nibble] + hex;
    } while (num);
    if (hex.size() < min)
    {
        auto i = min - hex.size();
        while (i-- > 0)
            hex = '0' + hex;
    }
    return "0x" + hex;
}
void DumpFile()
{
    switch (lang)
    {
    case Language::C:
        DumpFileC();
        break;
    case Language::Cpp:
        DumpFileCpp();
        break;
    }
    DumpBinFile();
}

#include <../crc32.inl>

void DumpBinFile()
{
    printf("生成二进制文件中...\n");

    BinBuffer vbin, voffset, vtsl;

    //  1.魔数：4 byte 为 i18n 字母的值
    vbin.push((uint8_t)'i');
    vbin.push((uint8_t)'1');
    vbin.push((uint8_t)'8');
    vbin.push((uint8_t)'n');
    //  2.校验：4 byte crc32 校验
    vbin.push(0u);
    //  3.1 文件格式
    vbin.push(uint32_t(sizeof(uintptr_t) * 8));
    //  3.2 翻译版号：4 byte
    vbin.push(version);
    //  4.翻译单元数量
    uint32_t count = tslMap.size();
    ::printf("翻译单元数量:%u\n", count);
    vbin.push(count);
    // 5.翻译单元数据生成
    for (auto &kv : tslMap)
    {
        auto &key = kv.first;
        auto &value = kv.second;
        uintptr_t offset = vtsl.getSize();
        voffset.push(offset); // 记录偏移

        vtsl.push(value.mEnum); // 记录翻译枚举值
        vtsl.push(value.mTsl);  // 记录翻译语言
    }
    // 6.
    uint32_t serial = 0; // 编号是否连续
    if (force_flg == true)
        serial = -1;
    vbin.push(serial);
    // 7.翻译数据大小
    uint32_t tslsize = vtsl.getSize();
    vbin.push(tslsize);

    // 8.合并数据
    vbin.push(voffset);
    vbin.push(vtsl);

    // 9.计算哈希值
    vbin.hash();

    // 10.保存到文件中
    string tslBin = output + ".tsl";
    FILE *fout = fopen(tslBin.c_str(), "wb");
    if (fout == nullptr)
        ::printf("[ %s ]：创建并打开文件失败!\n", tslBin.c_str());
    else
    {
        ::printf("[ %s ]：写入文件...\n", tslBin.c_str());
        fwrite(vbin.getData().data(), sizeof(uint8_t), vbin.getSize(), fout);
        fclose(fout);
    }
}

void DumpFileC()
{
    printf("生成 C 语言代码中...\n");

    string enumdef; // 枚举定义
    string tslimp;  // 翻译数据定义

    enumdef = "#ifndef _I18N_Tsl_\n#define _I18N_Tsl_\n\n"
              "/*----------------------\n"
              "   多语言翻译枚举ID定义\n"
              "    自动生成，请勿修改\n"
              "----------------------*/\n"
              "enum Tsl_i18n{\n";

    tslimp = "#ifdef _Need_I18N_Tsl_String_\n"
             "/*----------------------\n"
             "   多语言翻译字符串定义\n"
             "    自动生成，请勿修改\n"
             "----------------------*/\n"
             "struct{\n"
             "\tunsigned int mID;// 翻译枚举ID\n"
             "\tconst char* mTsl;// 翻译字符串\n"
             "}Tsl_i18n_Items[]={\n";

    for (auto &kv : tslMap)
    {
        auto &key = kv.first;
        auto &value = kv.second;

        enumdef += "\t" + key + "=";
        enumdef += std::to_string(value.mEnum) + ",\n";

        tslimp += "\t{";
        tslimp += key + ",\"";
        tslimp += value.mTsl;
        tslimp += "\"},\n";
    }

    enumdef += "};\n\n#endif\n";
    tslimp += "};\n\n#endif\n";

    // 保存到文件中
    string path = output + ".tsl.h";
    FILE *fout = fopen(path.c_str(), "wb");
    if (fout == nullptr)
        ::printf("[ %s ]：创建并打开文件失败!\n", path.c_str());
    else
    {
        ::printf("[ %s ]：写入文件...\n", path.c_str());
        fwrite(enumdef.data(), sizeof(uint8_t), enumdef.size(), fout);
        fclose(fout);
    }

    path = output + ".tsl.c";
    fout = fopen(path.c_str(), "wb");
    if (fout == nullptr)
        ::printf("[ %s ]：创建并打开文件失败!\n", path.c_str());
    else
    {
        ::printf("[ %s ]：写入文件...\n", path.c_str());
        fwrite(tslimp.data(), sizeof(uint8_t), tslimp.size(), fout);
        fclose(fout);
    }
}

void DumpFileCpp()
{
    printf("生成 C++ 语言代码中...\n");

    string enumdef; // 枚举定义
    string tslimp;  // 翻译数据定义

    enumdef = "#ifndef _I18N_Tsl_\n#define _I18N_Tsl_\n\n"
              "/*----------------------\n"
              "   多语言翻译枚举ID定义\n"
              "    自动生成，请勿修改\n"
              "----------------------*/\n"
              "struct Tsl_i18n{\n"
              "\tenum :unsigned int{\n";

    tslimp = "#ifdef _Need_I18N_Tsl_String_\n"
             "/*----------------------\n"
             "   多语言翻译字符串定义\n"
             "    自动生成，请勿修改\n"
             "----------------------*/\n"
             "struct{\n"
             "\tunsigned int mID;// 翻译枚举ID\n"
             "\tconst char* mTsl;// 翻译字符串\n"
             "}Tsl_i18n_Items[]={\n";

    for (auto &kv : tslMap)
    {
        auto &key = kv.first;
        auto &value = kv.second;

        enumdef += "\t\t" + key + "=";
        enumdef += std::to_string(value.mEnum) + ",\n";

        tslimp += "\t{";
        tslimp += "Tsl_i18n::" + key + ",\"";
        tslimp += value.mTsl;
        tslimp += "\"},\n";
    }

    enumdef += "\t};\n};\n\n#endif\n";
    tslimp += "};\n\n#endif\n";

    // 保存到文件中
    string path = output + ".tsl.hpp";
    FILE *fout = fopen(path.c_str(), "wb");
    if (fout == nullptr)
        ::printf("[ %s ]：创建并打开文件失败!\n", path.c_str());
    else
    {
        ::printf("[ %s ]：写入文件...\n", path.c_str());
        fwrite(enumdef.data(), sizeof(uint8_t), enumdef.size(), fout);
        fclose(fout);
    }

    path = output + ".tsl.cpp";
    fout = fopen(path.c_str(), "wb");
    if (fout == nullptr)
        ::printf("[ %s ]：创建并打开文件失败!\n", path.c_str());
    else
    {
        ::printf("[ %s ]：写入文件...\n", path.c_str());
        fwrite(tslimp.data(), sizeof(uint8_t), tslimp.size(), fout);
        fclose(fout);
    }
}
