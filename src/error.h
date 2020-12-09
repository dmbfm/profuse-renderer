#ifndef __ERROR_H
#define __ERROR_H 

typedef enum
{
    ERR_OK,
    ERR_OUT_OF_MEMORY, 
    ERR_ALLOCATORS_LINEAR_OUT_OF_MEMORY,
    ERR_ALLOCATORS_MEMORY_ALIGNMENT,
    ERR_UNKNOWN
} ErrorCode;

#endif /* __ERROR_H */

