#include "String.h"
#include "Memory.h"
#include <string.h>
#include <iostream>
#include <cassert>
#include "Codec.h"

namespace Alastor 
{
	char* String::data() const
	{
		return m_buffer.data();
	}

	const size_t String::size() const
	{
		return strlen(this->data());
	}

	String::String()
	{
		m_buffer = ByteArray().append('\0');
	}
	String::String(const char value)
	{
		m_buffer = ByteArray().append(value).append('\0');
	}
	String::String(const char* value)
	{
		m_buffer = ByteArray(value).append('\0');
	}

	String::String(const char* value, size_t size)
	{
		m_buffer = ByteArray(value, size).append('\0');
	}

	String::String(const std::string& value)
	{
		m_buffer = ByteArray(value.c_str()).append('\0');
	}

	String::String(const String& value)
	{
		m_buffer = ByteArray(value.data()).append('\0');
	}

	String::String(const ByteArray& value)
	{
		size_t index = 0;
		if (value.indexOf('\0', index))
		{
			m_buffer = value.mid(0, index, true);
		}
		else
		{
			m_buffer = value;
			m_buffer.append('\0');
		}
	}

	String::~String()
	{

	}

	String::operator std::string() const
	{
		return std::string(data());
	}




	String& String::operator=(const char* value)
	{
		m_buffer = ByteArray(value).append('\0');
		return *this;
	}

	String& String::operator=(const String& value)
	{
		m_buffer = ByteArray(value).append('\0');
		return *this;
	}

	String& String::operator=(const std::string& value)
	{
		m_buffer = ByteArray(value.c_str()).append('\0');
		return *this;
	}

	String String::operator+(const char* value) const
	{
		String buffer = ByteArray(m_buffer.data(), m_buffer.size() - sizeof('\0')) + ByteArray(value);
		return buffer;
	}

	String String::operator+(const std::string& value) const
	{
		// 转ByteArray追加字节后构造String类自动补'\0'
		String buffer = ByteArray(m_buffer.data(), m_buffer.size() - sizeof('\0')) + ByteArray(value.c_str(),value.size()).append('\0');
		return buffer;
	}

	String String::operator+(const String& value) const
	{
		String buffer = ByteArray(m_buffer.data(), m_buffer.size() - sizeof('\0')) + ByteArray(value.data(),value.size()).append('\0');
		return buffer;
	}

	String& String::operator+=(const char* value)
	{
		m_buffer = ByteArray(m_buffer.data(), m_buffer.size() - sizeof('\0')) + ByteArray(value).append('\0');// +ByteArray(1, '\0');
		return *this;
	}

	String& String::operator+=(const std::string& value)
	{
		m_buffer = ByteArray(m_buffer.data(), m_buffer.size() - sizeof('\0')) + ByteArray(value.c_str(),value.size()).append('\0');
		return *this;
	}

	String& String::operator+=(const String& value)
	{
		m_buffer = ByteArray(m_buffer.data(), m_buffer.size() - sizeof('\0')) + ByteArray(value.data(),value.size()).append('\0');
		return *this;
	}

	bool String::operator==(const String& value) const
	{
		// 无视结尾'\0'
		if (this->size() != value.size())
		{
			return false;
		}
		
		ByteArray src = ByteArray(this->m_buffer.data(),this->size());
		ByteArray dest = ByteArray(value.data(), value.size());
		if (src != dest)
		{
			return false;
		}
		return true;
	}

	bool String::operator!=(const String& value) const
	{
		// 无视结尾'\0'
		if (this->size() != value.size())
		{
			return true;
		}
		ByteArray src = ByteArray(this->m_buffer.data(), this->size());
		ByteArray dest = ByteArray(value.data(), value.size());
		if (src != dest)
		{
			return true;
		}
		return false;
	}

	char& String::operator[](const size_t i) const
	{
		assert(i <= size());
		return m_buffer[i];
	}

	String String::append(const String& value) const
	{
		return *this + value;
	}

	bool String::indexOf(const String& value, size_t& index,size_t start) const
	{
		ByteArray src = ByteArray(value.data(),value.size());
		return m_buffer.indexOf(src, index, start);
	}

	bool String::lastIndexOf(const String& value, size_t& index, size_t start) const
	{
		ByteArray src = ByteArray(value.data(), value.size());
		return m_buffer.lastIndexOf(src, index, start);
	}

	String String::left(const size_t& index, bool include) const
	{
		return m_buffer.left(index,include);
	}

	String String::left(const String& data,bool include) const
	{
		return m_buffer.left(data,include);
	}

	String String::right(const size_t& index,bool include) const
	{
		return m_buffer.right(index,include);
	}

	String String::right(const String& data, bool include) const
	{
		return m_buffer.right(data, include);
	}

	String String::mid(const size_t& startIndex, const size_t& endIndex, bool include) const
	{
		return m_buffer.mid(startIndex, endIndex, include);
		// return m_buffer.mid(startIndex, endIndex, include).append('\0');
	}

	String String::mid(const String& start, const String& end, bool include) const
	{
		// '\0'
		ByteArray srcByte = ByteArray(start.data(), start.size());
		ByteArray destByte = ByteArray(end.data(), end.size());
		return m_buffer.mid(srcByte, destByte, include);
		//return m_buffer.mid(srcByte, destByte, include).append('\0');
	}



	String String::replace(const String& src, const String& dest, const bool& once) const
	{
		// '\0'
		ByteArray srcByte = ByteArray(src.data(),src.size());
		ByteArray destByte = ByteArray(dest.data(), dest.size());
		return m_buffer.repalce(srcByte, destByte, once);
	}

	String String::replace(size_t start, size_t len, const String& value) const
	{
		return String();
	}

	std::vector<String> String::split(const String& separator) const
	{
		std::vector<String> list;
		size_t index = 0;
		size_t startIndex = 0;
		while (this->indexOf(separator, index, startIndex))
		{
			// a   b   c
			// 0 1 2 3 4 5 6
			list.push_back(this->mid(startIndex, index - 1, true));
			startIndex = index + separator.size() ;
		}
		if (startIndex != size())
		{
			list.push_back(this->right(startIndex - 1));
		}
		return list;
	}

	/// <summary>
	/// 到本地分隔符
	/// </summary>
	/// <returns>Windows和Linux分隔符不同</returns>
	String String::toNativeSeparators() const
	{
#if defined(_MSC_VER)
		return this->replace("/", "\\");
#elif defined(__unix__)
		return this->replace("\\", "/");
#else
#endif
	}


	String String::toUpper() const
	{
		for (unsigned int i = 0; i < this->size(); i++)
		{
			operator[](i) = Codec::toUpper(operator[](i));
		}
		return String(*this);
	}

	String String::toLower() const
	{
		for (unsigned int i = 0; i < this->size(); i++)
		{
			operator[](i) = Codec::toLower(operator[](i));
		}
		return String(*this);
	}


	//String String::toHex() const
	//{
	//	String result;
	//	const char HEX_NUMBERS[16] = {
	//	  '0', '1', '2', '3',
	//	  '4', '5', '6', '7',
	//	  '8', '9', 'a', 'b',
	//	  'c', 'd', 'e', 'f' };

	//	for (unsigned int i = 0; i < this->size(); i++)
	//	{
	//		int t = this->data()[i];
	//		int high = t / 16;
	//		int low = t % 16;
	//		result += HEX_NUMBERS[high];
	//		result += HEX_NUMBERS[low];
	//	}
	//	return result;
	//}





	std::ostream& operator<<(std::ostream& out, const String& value)
	{
		return out << reinterpret_cast<const char*>(value.data());
	}


	// 有问题待修改
	std::istream& operator>>(std::istream& in, String& value)
	{
		return in >> reinterpret_cast<char*>(ByteArray(value).data());
	}

	String String::number(int Val)
	{
		return std::to_string(Val).c_str();
	}

	String String::number(unsigned int Val)
	{
		return std::to_string(Val).c_str();
	}

	String String::number(long Val)
	{
		return std::to_string(Val).c_str();
	}

	String String::number(unsigned long Val)
	{
		return std::to_string(Val).c_str();
	}

	String String::number(long long Val)
	{
		return std::to_string(Val).c_str();
	}

	String String::number(unsigned long long Val)
	{
		return std::to_string(Val).c_str();
	}

	String String::number(double Val)
	{
		return std::to_string(Val).c_str();
	}

	String String::number(float Val)
	{
		return std::to_string(Val).c_str();
	}

	String String::number(long double Val)
	{
		return std::to_string(Val).c_str();
	}


}
