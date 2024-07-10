#pragma once
#include "Alastor.h"
#include <stdio.h>
namespace Alastor 
{
    template<typename T>
    class ALASTOR_API Vector 
    {
    public:
        typedef T value_type;
        typedef T* iterator;
    private:
        // 内存缓冲区
        value_type* m_buffer;
        // 元素个数
        size_t m_size;
        // 用于存放Vector对象所申请的空间大小
        size_t m_capacity;

    public:
        Vector() :m_buffer(NULL), m_size(0), m_capacity(0) {}
        ~Vector() 
        {
            delete[] m_buffer;
            m_buffer = NULL;
            m_size = 0;
            m_capacity = 0;
        }
        Vector(const Vector& vec) {
            m_size = vec.m_size;
            m_capacity = vec.m_capacity;
            m_buffer = new value_type[m_capacity];
            for (size_t i = 0; i < m_size; ++i) {
                m_buffer[i] = vec.m_buffer[i];
            }
        }
        Vector& operator=(const Vector& vec) {
            if (this == &vec) return *this;
            value_type* temp = new value_type[vec.m_capacity];
            for (int i = 0; i < vec.m_size; ++i) {
                temp[i] = vec.m_buffer[i];
            }
            delete[] m_buffer;
            m_buffer = temp;
            m_size = vec.m_size;
            m_capacity = vec.m_capacity;
            return *this;
        }
        void push_back(value_type val) {
            if (0 == m_capacity) {
                m_capacity = 1;
                m_buffer = new value_type[1];
            }
            else if (m_size + 1 > m_capacity) {
                m_capacity *= 2;
                value_type* temp = new value_type[m_capacity];
                for (size_t i = 0; i < m_size; ++i) {
                    temp[i] = m_buffer[i];
                }
                delete[] m_buffer;
                m_buffer = temp;
            }
            m_buffer[m_size] = val;
            ++m_size;
        }
        void pop_back() { --m_size; }
        size_t size()const { return m_size; }
        size_t capacity()const { return m_capacity; }
        bool empty() { return m_size == 0; }
        value_type& operator[](size_t index) {
            return m_buffer[index];
        }
        bool operator==(const Vector& vec)const {
            if (m_size != vec.m_size) return false;
            for (int i = 0; i < m_size; ++i) {
                if (m_buffer[i] != vec.m_buffer[i]) return false;
            }
            return true;
        }
        value_type front()const { return m_buffer[0]; }
        value_type back() const { return m_buffer[m_size - 1]; }

        void insert(iterator it, value_type val) {
            int index = it - m_buffer;
            if (0 == m_capacity) {
                m_capacity = 1;
                m_buffer = new value_type[1];
                m_buffer[0] = val;
            }
            else if (m_size + 1 > m_capacity) {
                m_capacity *= 2;
                value_type* temp = new value_type[m_capacity];
                for (int i = 0; i < index; ++i) {
                    temp[i] = m_buffer[i];
                }
                temp[index] = val;
                for (int i = index; i < m_size; ++i) {
                    temp[i + 1] = m_buffer[i];
                }
                delete[] m_buffer;
                m_buffer = temp;
            }
            else {
                for (int i = m_size - 1; i >= index; --i) {
                    m_buffer[i + 1] = m_buffer[i];
                }
                m_buffer[index] = val;
            }
            ++m_size;

        }
        void erase(iterator it) {
            size_t index = it - m_buffer;
            for (int i = index; i < m_size - 1; ++i) {
                m_buffer[i] = m_buffer[i + 1];
            }
            --m_size;
        }

        iterator begin() { return m_buffer; }
        iterator end() { return m_buffer + m_size; }
    };

}

