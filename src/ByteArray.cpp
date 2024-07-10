#include "Common.h"

#include <vector>
#include <string>

#include <assert.h>
#include "Memory.h"
#include "String.h"
namespace Alastor
{
	/// <summary>
	/// 默认构造函数
	/// </summary>
	ByteArray::ByteArray()
	{
		m_buffer = nullptr;
		m_size = 0;
	}

	ByteArray::ByteArray(const ByteArray& data)
	{
		size_t size = data.size();
		m_buffer = (char*)Alastor::Memory::malloc(size);
		if (m_buffer)
		{
			Alastor::Memory::memcpy(m_buffer, data.data(), size);
			m_size = size;
		}
		else
		{
			m_size = 0;
		}
	}

	ByteArray::ByteArray(const char* data, size_t size)
	{
		if (!data || !size)
		{
			m_buffer = nullptr;
			m_size = 0;
			return;
		}
		m_buffer = (char*)Alastor::Memory::malloc(size);
		if (m_buffer)
		{
			Alastor::Memory::memcpy(m_buffer, data, size);
			m_size = size;
		}
		else
		{
			m_size = 0;
		}
	}

	ByteArray::ByteArray(const char* data)
	{
		if (!data)
		{
			m_buffer = nullptr;
			m_size = 0;
			return;
		}
		// '\0'
		size_t size = strlen(data);// + sizeof(char);// +sizeof('\0');
		m_buffer = (char*)Alastor::Memory::malloc(size);
		if (m_buffer)
		{
			Alastor::Memory::memcpy(m_buffer, data, size);
			m_size = size;
		}
		else
		{
			m_size = 0;
		}
	}

	ByteArray::ByteArray(const wchar_t* data)
	{
		if (!data)
		{
			m_buffer = nullptr;
			m_size = 0;
			return;
		}
		// '\0'
		size_t size = wcslen(data) * 2;//+ sizeof(wchar_t);// +sizeof('\0');
		m_buffer = (char*)Alastor::Memory::malloc(size);
		if (m_buffer)
		{
			Alastor::Memory::memcpy(m_buffer, data, size);
			m_size = size;
		}
		else
		{
			m_size = 0;
		}
	}

	ByteArray::ByteArray(size_t size, char c)
	{
		if (!size)
		{
			return;
		}
		m_buffer = (char*)Alastor::Memory::malloc(size);
		if (m_buffer)
		{
			Alastor::Memory::memset(m_buffer, c, size);
			m_size = size;
		}
		else
		{
			m_size = 0;
		}
	}

	ByteArray::ByteArray(const String& data)
	{
		if (!data.data())
		{
			m_buffer = nullptr;
			m_size = 0;
			return;
		}
		// '\0'
		size_t size = strlen(data.data());// + sizeof(char);// +sizeof('\0');
		m_buffer = (char*)Alastor::Memory::malloc(size);
		if (m_buffer)
		{
			Alastor::Memory::memcpy(m_buffer, data.data(), size);
			m_size = size;
		}
		else
		{
			m_size = 0;
		}
	}

	ByteArray::ByteArray(const WString& data)
	{
		if (!data.data())
		{
			m_buffer = nullptr;
			m_size = 0;
			return;
		}
		// '\0'
		size_t size = wcslen(data.data());// + sizeof(char);// +sizeof('\0');
		m_buffer = (char*)Alastor::Memory::malloc(size);
		if (m_buffer)
		{
			Alastor::Memory::memcpy(m_buffer, data.data(), size);
			m_size = size;
		}
		else
		{
			m_size = 0;
		}
	}

	ByteArray::~ByteArray()
	{
		if (m_buffer)
		{
			Alastor::Memory::free(m_buffer);
		}
		m_size = 0;
	}

	/*
		重载函数
	*/
	ByteArray& ByteArray::operator=(const ByteArray& value)
	{
		if (m_buffer)
		{
			Alastor::Memory::free(m_buffer);
			m_size = 0;
		}
		m_buffer = (char*)Alastor::Memory::malloc(value.size());
		if (m_buffer)
		{
			Alastor::Memory::memcpy(m_buffer, value.data(), value.size());
			m_size = value.size();
		}
		return *this;
	}

	ByteArray ByteArray::operator+(const ByteArray& value) const
	{
		ByteArray buffer;
		// 调整缓冲区大小
		buffer.resize(m_size + value.size());

		// 追加复制数据
		Alastor::Memory::memcpy(buffer.data(), this->data(), this->size());
		Alastor::Memory::memcpy(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer.data()) + this->size()), value.data(), value.size());

		// 多余
		// m_size += value.size();
		return buffer;
	}


	ByteArray& ByteArray::operator+=(const ByteArray& value)
	{
		// 调整缓冲区大小
		resize(m_size + value.size());

		// 追加复制数据
		Alastor::Memory::memcpy(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(m_buffer) + m_size - value.size()), value.data(), value.size());

		// 多余
		// m_size += value.size();
		return *this;
	}

	char& ByteArray::operator[](const size_t i) const
	{
		assert(i < m_size && m_buffer);
		return reinterpret_cast<char*>(m_buffer)[i];
	}


	const bool ByteArray::operator==(const ByteArray& value) const
	{
		if (this->m_size != value.m_size)
		{
			return false;
		}

		for (size_t i = 0; i < m_size; i++)
		{
			if (operator[](i) != value[i])
			{
				return false;
			}
		}

		return true;
	}


	const bool ByteArray::operator!=(const ByteArray& value) const
	{
		if (this->m_size != value.m_size)
		{
			return true;
		}

		for (size_t i = 0; i < m_size; i++)
		{
			if (operator[](i) != value[i])
			{
				return true;
			}
		}

		return false;
	}

	ByteArray& ByteArray::append(const ByteArray& value)
	{
		if (value.size() == 0)
		{
			return *this;
		}
		return (*this += value);
	}

	ByteArray& ByteArray::append(const char* data, size_t size)
	{
		if (!data || !size)
		{
			return *this;
		}
		return *this += ByteArray(data, size);
	}

	ByteArray& ByteArray::append(const char* data)
	{
		if (!data)
		{
			return *this;
		}
		return *this += ByteArray(data);
	}

	ByteArray& ByteArray::append(const wchar_t* data)
	{
		if (!data)
		{
			return *this;
		}
		return *this += ByteArray(data);
	}

	ByteArray& ByteArray::append(const bool data)
	{
		this->append((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::append(const char data)
	{
		this->append((char*) & data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::append(const wchar_t data)
	{
		this->append((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::append(const short data)
	{
		this->append((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::append(const __int32 data)
	{
		this->append((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::append(const __int64 data)
	{
		this->append((char*)&data, sizeof(data));
		return *this;
	}


	ByteArray& ByteArray::append(const unsigned char data)
	{
		this->append((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::append(const unsigned short data)
	{
		this->append((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::append(const unsigned __int32 data)
	{
		this->append((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::append(const unsigned __int64 data)
	{
		this->append((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::prepend(const ByteArray& value)
	{
		if (value.size() == 0)
		{
			return *this;
		}
		*this = value + *this;
		return *this;
	}

	ByteArray& ByteArray::prepend(const char* data, size_t size)
	{
		if (!data || !size)
		{
			return *this;
		}
		*this = ByteArray(data, size) + *this;
		return *this;
	}

	ByteArray& ByteArray::prepend(const char* data)
	{
		if (!data)
		{
			return *this;
		}
		*this = ByteArray(data) + *this;
		return *this;
	}

	ByteArray& ByteArray::prepend(const wchar_t* data)
	{
		if (!data)
		{
			return *this;
		}
		*this = ByteArray(data) + *this;
		return *this;
	}

	ByteArray& ByteArray::prepend(const bool data)
	{
		this->prepend((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::prepend(const char data)
	{
		this->prepend((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::prepend(const wchar_t data)
	{
		this->prepend((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::prepend(const short data)
	{
		this->prepend((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::prepend(const __int32 data)
	{
		this->prepend((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::prepend(const __int64 data)
	{
		this->prepend((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::prepend(const unsigned char data)
	{
		this->prepend((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::prepend(const unsigned short data)
	{
		this->prepend((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::prepend(const unsigned __int32 data)
	{
		this->prepend((char*)&data, sizeof(data));
		return *this;
	}

	ByteArray& ByteArray::prepend(const unsigned __int64 data)
	{
		this->prepend((char*)&data, sizeof(data));
		return *this;
	}



	ByteArray ByteArray::left(const size_t& index, bool include) const
	{
		if (!include)
		{
			return ByteArray(m_buffer, INDEXTOSIZE(index) - 1);
		}
		else
		{
			return ByteArray(m_buffer, INDEXTOSIZE(index));
		}
		
	}

	ByteArray ByteArray::left(const ByteArray& data, bool include) const
	{
		size_t index = 0;
		if (!this->indexOf(data, index))
		{
			return ByteArray();
		}
		if(!include)
		{
			return this->left(index);
		}
		else
		{
			return this->left(index + SIZETOINDEX(data.size()),true);
		}
	}

	ByteArray ByteArray::right(const size_t index,bool include) const
	{
		if (!include)
		{
			return ByteArray(reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(m_buffer) + index + 1), m_size - INDEXTOSIZE(index) - 1); // 起始索引变化
		}
		else
		{
			return ByteArray(reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(m_buffer) + index), m_size - INDEXTOSIZE(index));
		}
	}

	ByteArray ByteArray::right(const ByteArray& data, bool include) const
	{
		size_t index;
		if (!this->indexOf(data, index))
		{
			return ByteArray();
		}
		if (!include)
		{
			return this->right(index + SIZETOINDEX(data.size()));
		}
		else
		{
			return this->right(index,true);
		}
	}

	ByteArray ByteArray::mid(const size_t& startIndex, const size_t& endIndex, bool include) const
	{
		if (startIndex > m_size || endIndex > m_size)
		{
			return ByteArray();
		}
		if (!include)
		{
			return  ByteArray(reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(m_buffer) + startIndex + 1), INDEXTOSIZE(endIndex - startIndex - 1 - 1));// 起始位置改变
		}
		else
		{
			return  ByteArray(reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(m_buffer) + startIndex), INDEXTOSIZE(endIndex - startIndex));
		}

	}

	ByteArray ByteArray::mid(const ByteArray& start, const ByteArray& end, bool include) const
	{
		size_t startIndex = 0;
		size_t endIndex = 0;
		if (!this->indexOf(start, startIndex) || !this->indexOf(end, endIndex, startIndex))
		{
			return ByteArray();
		}

		if (include)
		{
			return this->mid(startIndex, endIndex + SIZETOINDEX(end.size()), true);
		}
		else
		{
			return this->mid(startIndex + SIZETOINDEX(start.size()), endIndex, false);
		}
	}

	bool ByteArray::indexOf(const char c, size_t& index, size_t startIndex) const
	{
		if (m_size <= startIndex)
		{
			return false;
		}
		for (size_t i = startIndex; i < m_size; i++)
		{
			if (operator[](i) == c)
			{
				index = i;
				return true;
			}
		}
		return false;
	}

	bool ByteArray::indexOf(const ByteArray& data, size_t& index, size_t startIndex) const
	{
		if (m_size <= startIndex + SIZETOINDEX(data.size()))
		{
			return false;
		}
		for (size_t i = startIndex; i <= m_size - data.size(); i++)
		{
			bool found = true;
			for (size_t j = 0; j < data.size(); j++)
			{
				if (operator[](i + j) != data[j])
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				index = i;
				return true;
			}

		}
		return false;
	}

	bool ByteArray::lastIndexOf(const char c, size_t& index, size_t startIndex) const
	{
		if (m_size <= startIndex)
		{
			return false;
		}
		for (size_t i = startIndex ? startIndex : SIZETOINDEX(m_size); i > 0;i--)
		{
			if (operator[](i) == c)
			{
				index = i;
				return true;
			}
		}
		if (operator[](0) == c)
		{
			index = 0;
			return true;
		}
		return false;
	}

	bool ByteArray::lastIndexOf(const ByteArray& data, size_t& index, size_t startIndex) const
	{
		if (m_size <= startIndex + SIZETOINDEX(data.size()))
		{
			return false;
		}
		for (size_t i = startIndex ? startIndex : m_size - data.size(); i > 0; i--)
		{
			bool found = true;
			for (size_t j = 0; j < data.size(); j++)
			{
				if (operator[](i + j) != data[j])
				{
					found = false;
					break;
				}

			}
			if (found)
			{
				index = i;
				return true;
			}
		}

		for (size_t i = 0; i < data.size(); i++)
		{
			if (operator[](i) != data[i])
			{
				return false;
			}
		}
		index = 0;
		return true;
	}

	ByteArray ByteArray::repalce(const ByteArray& src, const ByteArray& dest, const bool& once) const
	{
		ByteArray buffer = *this;
		size_t index = 0;
		size_t startIndex = 0;

		while (buffer.indexOf(src, index, startIndex))
		{
			buffer = buffer.left(index) + dest + buffer.right(index + src.size() - 1);
			startIndex += dest.size();
			if (once)
			{
				break;
			}
		}

		return buffer;
	}

	std::vector<ByteArray> ByteArray::split(const ByteArray& separator) const
	{
		std::vector<ByteArray> list;
		size_t index = 0;
		size_t startIndex = 0;
		while (this->indexOf(separator, index, startIndex))
		{
			list.push_back(this->mid(startIndex, index));
			startIndex = index + separator.size();
		}
		return list;
	}


	String ByteArray::toHex(char separator) const
	{
		if (!m_size)
		{
			return ByteArray();
		}

		const char HEX_NUMBERS[16] = {
		  '0', '1', '2', '3',
		  '4', '5', '6', '7',
		  '8', '9', 'a', 'b',
		  'c', 'd', 'e', 'f' };

		// 一个字节会形成两个字符,最终长度是原始数据的2倍
		const size_t length = separator ? (m_size * 3 - 1) : (m_size * 2);

		ByteArray hex;
		hex.resize(length);
		unsigned char* data = (unsigned char*)hex.data();
		for (unsigned int i = 0, o = 0; i < m_size; ++i)
		{
			unsigned char t = reinterpret_cast<unsigned char*>(m_buffer)[i];
			unsigned char high = t / 16;
			unsigned char low = t % 16;
			data[o++] = HEX_NUMBERS[high];
			data[o++] = HEX_NUMBERS[low];
			if (separator && (o < length))
			{
				data[o++] = separator;
			}
		}
		return hex;
	}

	ByteArray ByteArray::fromHex(String hex)
	{
		return ByteArray();
	}

	void ByteArray::resize(size_t size)
	{
		char* buffer = (char*)Alastor::Memory::malloc(size);
		if (!buffer)
		{
			return;
		}
		Alastor::Memory::memset(buffer, 0, size);
		if (m_buffer)
		{
			Alastor::Memory::memcpy(buffer, m_buffer, MIN(size, m_size));
			Alastor::Memory::free(m_buffer);
		}
		m_buffer = buffer;
		m_size = size;
	}

	void ByteArray::resize(char padding, size_t size)
	{
		char* buffer = (char*)Alastor::Memory::malloc(size);
		if (!buffer)
		{
			return;
		}
		Alastor::Memory::memset(buffer, padding, size);
		if (m_buffer)
		{
			Alastor::Memory::memcpy(buffer, m_buffer, MIN(size, m_size));
			Alastor::Memory::free(m_buffer);
		}
		m_buffer = buffer;
		m_size = size;
	}

	const size_t ByteArray::size() const
	{
		return m_size;
	}

	const size_t ByteArray::length() const
	{
		return m_size;
	}

	char* ByteArray::data() const
	{
		return m_buffer;
	}
	void ByteArray::clear()
	{
		if (m_buffer)
		{
			Alastor::Memory::free(m_buffer);
			m_size = 0;
			m_buffer = nullptr;
		}
	}
}
