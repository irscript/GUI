#ifndef __AIR_PCH__H__
#define __AIR_PCH__H__

// c 头文件
#include <ctype.h>
#include <cstdint>
#include <cstdlib>
#include <cstdarg>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <locale.h>

// c++ 头文件
#include <new>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <initializer_list>
#include <fstream>
#include<filesystem>

#include <airkit/AIR_COMPILE_OPTION.h>

namespace airkit
{
    template <typename T>
    using Scope = std::unique_ptr<T>;
    template <typename T>
    using Holder = std::shared_ptr<T>;
    template <typename T>
    using Watcher = std::weak_ptr<T>;
}
#endif //!__AIR_PCH__H__