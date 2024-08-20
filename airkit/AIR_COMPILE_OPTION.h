#ifndef __AIR_COMPILE_OPTION__H__
#define __AIR_COMPILE_OPTION__H__

// 编译选项在此定义
#define thisfile() __FILE__
#define thisfunc() __FUNCTION__
#define thisline() __LINE__
#define thisfuncarg() __PRETTY_FUNCTION__

// 开启 OpenGL 调试
#define GL_DEBUG

#endif //!__AIR_COMPILE_OPTION__H__