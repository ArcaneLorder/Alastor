#pragma once
#include "Common.h"
#include <vector>

#define INDEXTOSIZE(value) value + 1
#define SIZETOINDEX(value) value - 1

namespace Alastor
{
    class ALASTOR_API ByteArray
    {
    public:
        // 默认构造
        ByteArray();
        // 拷贝构造
        ByteArray(const ByteArray& data);
        // 字节构造
        ByteArray(const char* data, size_t size);
        // Ansi字符串构造
        ByteArray(const char* data);
        // Unicode字符串构造
        ByteArray(const wchar_t* data);
        // 根据长度构造
        ByteArray(size_t size, char c = 0);
        // 根据String构造
        ByteArray(const class String& data);
        // 根据WString构造
        ByteArray(const class WString& data);
        // 析构函数
        ~ByteArray();

    public:
        ByteArray& operator=(const ByteArray& value);
        ByteArray& operator+=(const ByteArray& value);
        ByteArray operator+(const ByteArray& value) const; 
        char& operator[](const size_t i) const;
        const bool operator==(const ByteArray& value) const;
        const bool operator!=(const ByteArray& value) const;
    public:
        ByteArray& append(const ByteArray& value); 
        ByteArray& append(const char* data, size_t size);
        ByteArray& append(const char* data);
        ByteArray& append(const wchar_t* data);
        ByteArray& append(const bool data);
        ByteArray& append(const char data);
        ByteArray& append(const wchar_t data);
        ByteArray& append(const short data);
        ByteArray& append(const __int32 data);
        ByteArray& append(const __int64 data);
        ByteArray& append(const unsigned char data);
        ByteArray& append(const unsigned short data);
        ByteArray& append(const unsigned __int32 data);
        ByteArray& append(const unsigned __int64 data);

        ByteArray& prepend(const ByteArray& value);
        ByteArray& prepend(const char* data, size_t size);
        ByteArray& prepend(const char* data);
        ByteArray& prepend(const wchar_t* data);
        ByteArray& prepend(const bool data);
        ByteArray& prepend(const char data);
        ByteArray& prepend(const wchar_t data);
        ByteArray& prepend(const short data);
        ByteArray& prepend(const __int32 data);
        ByteArray& prepend(const __int64 data);
        ByteArray& prepend(const unsigned char data);
        ByteArray& prepend(const unsigned short data);
        ByteArray& prepend(const unsigned __int32 data);
        ByteArray& prepend(const unsigned __int64 data);

        ByteArray left(const size_t& index, bool include = false) const;
        ByteArray left(const ByteArray& data, bool include = false) const;
        ByteArray right(const size_t index,bool include = false) const;
        ByteArray right(const ByteArray& data, bool include = false) const;
        ByteArray mid(const size_t& startIndex,const size_t& endIndex,bool include = false) const;
        ByteArray mid(const ByteArray& start, const ByteArray& end, bool include = false) const;

        bool indexOf(const char c, size_t& index, size_t startIndex = 0) const;
        bool indexOf(const ByteArray& data, size_t& index, size_t startIndex = 0) const;
        bool lastIndexOf(const char c, size_t& index, size_t startIndex = 0) const;
        bool lastIndexOf(const ByteArray& data, size_t& index, size_t startIndex = 0) const;

        ByteArray repalce(const ByteArray& src, const ByteArray& dest, const bool& once = false) const;
        std::vector<ByteArray> split(const ByteArray& separator) const;
       
        class String toHex(char separator = NULL) const;
    public:
        static ByteArray fromHex(class String hex);
    public:
        void resize(size_t size);
        void resize(char padding, size_t size);
        const size_t size() const;
        const size_t length() const;
        char* data() const;
        void clear();
    private:
        char* m_buffer;
        size_t m_size;
    };

}


