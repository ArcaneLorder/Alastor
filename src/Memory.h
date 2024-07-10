#pragma once
#include "Common.h"


namespace Alastor
{
    namespace Memory
    {
        ALASTOR_API void* malloc(size_t size);
        ALASTOR_API void  free(void* src);
        ALASTOR_API void* realloc(void* src, size_t length);
        ALASTOR_API void* memcpy(void* dst, const void* src, size_t size);
        ALASTOR_API int memcmp(const void* src, const void* dst, size_t size);
        ALASTOR_API void* memset(void* dst, int value, size_t size);
    }
}