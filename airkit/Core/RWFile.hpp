#ifndef __RWFILE_H__
#define __RWFILE_H__

#include <airkit/AIR_PCH.hpp>

namespace airkit
{
    bool readFile(const std::string &path, std::vector<uint8_t> &data);
    bool writeFile(const std::string &path, const std::vector<uint8_t> &data);
}

#endif // __RWFILE_H__