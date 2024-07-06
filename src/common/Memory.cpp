#include "Memory.h"
#include <memory>

namespace Alastor
{
    namespace Memory
    {
        void* malloc(size_t size)
        {
            void* result = ::malloc(size);
            return result;
        }

        void free(void* src)
        {
            ::free(src);
        }

        void* realloc(void* src, size_t length)
        {
            void* result = ::realloc(src, length);;
            return result;
        }
        void* memcpy(void* dst, const void* src, size_t size)
        {
            void* result = ::memcpy(dst,src,size);
            return result;
        }
        int memcmp(const void* src, const void* dst, size_t size)
        {
            int result = ::memcmp(src, dst, size);
            return result;
        }
        void* memset(void* dst, int value, size_t size)
        {
            void* result = ::memset(dst, value, size);
            return result;
        }
    }
}

