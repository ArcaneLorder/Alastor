#include "String.h"
#include "Memory.h"
#include <string.h>
#include <cassert>
#include "Codec.h"

namespace Alastor
{
	wchar_t* WString::data() const
	{
		return (wchar_t*)(m_buffer.data());
	}

	const size_t WString::size() const
	{
		if (m_buffer.size() == 0)
		{
			return 0;
		}
		return wcslen(this->data());
	}

	WString::WString()
	{
		m_buffer = ByteArray().append(L'\0');
	}
	WString::WString(const wchar_t value)
	{
		m_buffer = ByteArray().append(value).append(L'\0');
	}
	WString::WString(const wchar_t* value)
	{
		m_buffer = ByteArray(value).append(L'\0');
	}

	WString::WString(const char* value, size_t size)
	{
		m_buffer = ByteArray(value, size).append(L'\0');
	}

	WString::WString(const std::wstring& value)
	{
		m_buffer = ByteArray(value.c_str()).append(L'\0');
	}

	WString::WString(const WString& value)
	{
		m_buffer = ByteArray(value.data()).append(L'\0');
	}

	WString::WString(const ByteArray& value)
	{
		size_t index = 0;
		
		if (value.indexOf(ByteArray().append(L'\0'), index))
		{
			// L'\0' == 0000
			m_buffer = value.mid(0, index + 1, true);
		}
		else
		{
			m_buffer = value;
			m_buffer.append(L'\0');
		}
	}

	WString::~WString()
	{

	}

	WString::operator std::wstring() const
	{
		return std::wstring(data());
	}


	WString& WString::operator=(const wchar_t* value)
	{
		m_buffer = ByteArray(value).append(L'\0');
		return *this;
	}

	WString& WString::operator=(const WString& value)
	{
		m_buffer = ByteArray(value).append(L'\0');
		return *this;
	}

	WString& WString::operator=(const std::wstring& value)
	{
		m_buffer = ByteArray(value.c_str()).append(L'\0');
		return *this;
	}

	WString WString::operator+(const wchar_t* value) const
	{
		WString buffer = ByteArray(m_buffer.data(), m_buffer.size() - sizeof(L'\0')) + ByteArray(value);
		return buffer;
	}

	WString WString::operator+(const std::wstring& value) const
	{
		// 转ByteArray追加字节后构造String类自动补'\0'
		WString buffer = ByteArray(m_buffer.data(), m_buffer.size() - sizeof(L'\0')) + ByteArray(value.c_str()).append(L'\0');
		return buffer;
	}

	WString WString::operator+(const WString& value) const
	{
		WString buffer = ByteArray(m_buffer.data(), m_buffer.size() - sizeof(L'\0')) + ByteArray(value.data()).append(L'\0');
		return buffer;
	}

	WString& WString::operator+=(const wchar_t* value)
	{
		m_buffer = ByteArray(m_buffer.data(), m_buffer.size() - sizeof(L'\0')) + ByteArray(value).append(L'\0');// +ByteArray(1, '\0');
		return *this;
	}

	WString& WString::operator+=(const std::wstring& value)
	{
		m_buffer = ByteArray(m_buffer.data(), m_buffer.size() - sizeof(L'\0')) + ByteArray(value.c_str()).append(L'\0');
		return *this;
	}

	WString& WString::operator+=(const WString& value)
	{
		m_buffer = ByteArray(m_buffer.data(), m_buffer.size() - sizeof(L'\0')) + ByteArray(value.data()).append(L'\0');
		return *this;
	}

	bool WString::operator==(const WString& value) const
	{
		// 无视结尾'\0'
		if (this->size() != value.size())
		{
			return false;
		}

		ByteArray src = ByteArray(this->data());
		ByteArray dest = ByteArray(value.data());
		if (src != dest)
		{
			return false;
		}
		return true;
	}

	bool WString::operator!=(const WString& value) const
	{
		// 无视结尾'\0'
		if (this->size() != value.size())
		{
			return true;
		}
		ByteArray src = ByteArray(this->data());
		ByteArray dest = ByteArray(value.data());
		if (src != dest)
		{
			return true;
		}
		return false;
	}

	wchar_t& WString::operator[](const size_t i) const
	{
		assert(i <= size());
		return reinterpret_cast<wchar_t*>(m_buffer.data())[i];
	}

	WString WString::append(const WString& value) const
	{
		return *this + value;
	}

	bool WString::indexOf(const WString& value, size_t& index, size_t start) const
	{
		ByteArray src = ByteArray(value.data());
		bool result = m_buffer.indexOf(src, index, start*2);
		index /= 2;
		return result;
	}

	bool WString::lastIndexOf(const WString& value, size_t& index, size_t start) const
	{
		ByteArray src = ByteArray(value.data());
		bool result = m_buffer.lastIndexOf(src, index, start*2);
		index /= 2;
		return result;
	}

	WString WString::left(const size_t& index, bool include) const
	{
		if (include)
		{
			// 0 1 2 3 4 5 6 7 8 9
			return m_buffer.left(index * 2 + 1,true);
		}
		return m_buffer.left(index * 2, include);
	}

	WString WString::left(const WString& data, bool include) const
	{
		return m_buffer.left(data, include);
	}

	WString WString::right(const size_t& index, bool include) const
	{
		if (include)
		{
			return m_buffer.right(index * 2,true);
		}

		return m_buffer.right(index * 2 + 1, include);
	}

	WString WString::right(const WString& data, bool include) const
	{
		return m_buffer.right(data, include);
	}

	WString WString::mid(const size_t& startIndex, const size_t& endIndex, bool include) const
	{
		// 空字符问题
		if (include)
		{   // 01 23 45 67 89 
			// W  S  t  r  i n g
			// 0  1  2  3  4 5 6 7 8 9
			// 
			return m_buffer.mid(startIndex * 2, endIndex * 2 + 1, true);// .append(L'\0');
		}
		return m_buffer.mid(startIndex * 2 + 1, endIndex * 2, include);// .append(L'\0');
	}

	WString WString::mid(const WString& start, const WString& end, bool include) const
	{
		// '\0'
		ByteArray srcByte = ByteArray(start.data());
		ByteArray destByte = ByteArray(end.data());
		return m_buffer.mid(srcByte, destByte, include);//.append(L'\0');
	}



	WString WString::replace(const WString& src, const WString& dest, const bool& once) const
	{
		// '\0'
		ByteArray srcByte = ByteArray(src.data());
		ByteArray destByte = ByteArray(dest.data());
		return m_buffer.repalce(srcByte, destByte, once);
	}

	WString WString::replace(size_t start, size_t len, const WString& value) const
	{
		return WString();
	}

	std::vector<WString> WString::split(const WString& separator) const
	{
		std::vector<WString> list;
		size_t index = 0;
		size_t startIndex = 0;
		while (this->indexOf(separator, index, startIndex))
		{
			std::wcout << this->mid(startIndex, index - 1, true) << std::endl;
			list.push_back(this->mid(startIndex, index - 1, true));
			startIndex = index + separator.size();
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
	WString WString::toNativeSeparators() const
	{
#if defined(_MSC_VER)
		return this->replace(L"/", L"\\");
#elif defined(__unix__)
		return this->replace(L"\\", L"/");
#else
#endif
	}

	//WString String::toWString() const
	//{
	//	return Codec::gbkToUnicode(*this);
	//}

	//String String::toGbk() const
	//{
	//	return Codec::utf8ToGbk(*this);
	//}

	WString WString::toUpper() const
	{
		for (unsigned int i = 0; i < this->size(); i++)
		{
			operator[](i) = Codec::toWUpper(operator[](i));
		}
		return WString(*this);
	}

	WString WString::toLower() const
	{
		for (unsigned int i = 0; i < this->size(); i++)
		{
			operator[](i) = Codec::toWLower(operator[](i));
		}
		return WString(*this);
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





	std::wostream& operator<<(std::wostream& out, const WString& value)
	{
		return out << reinterpret_cast<const wchar_t*>(value.data());
	}


	std::wistream& operator>>(std::wistream& in, WString& value)
	{
		return in >> reinterpret_cast<wchar_t*>(value.data());
	}

}
