#ifndef __ERROR_H
#define __ERROR_H 

#include "common.h"

typedef enum
{
    ERR_OK,
    ERR_ANY,
    ERR_OUT_OF_MEMORY, 
    ERR_ALLOCATORS_LINEAR_OUT_OF_MEMORY,
    ERR_ALLOCATORS_MEMORY_ALIGNMENT,
    ERR_PLATFORM_WEB_MEM_INIT,
    ERR_UNKNOWN,
    ERR_MATH_OVERFLOW,
    ERR_NOT_IMPLEMENTED,

    ERR_PLATFORM_WIN32_OPENGL32_DLL
} ErrorCode;

#define error_raise(f) \
    int TOKENCOMBINE(__error,__LINE__) = (f); if (TOKENCOMBINE(__error,__LINE__) != ERR_OK) return TOKENCOMBINE(__error,__LINE__);

#endif /* __ERROR_H */

