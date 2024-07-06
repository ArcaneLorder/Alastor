#pragma once
#include "Common.h"
#include <string>
#include <iostream>
#include "String.h"



namespace Alastor
{
	class ALASTOR_API WString
	{
	public:
		WString();
		WString(const wchar_t value);
		WString(const wchar_t* value);
		WString(const char* value, size_t size);
		WString(const std::wstring& value);
		WString(const WString& value);
		WString(const ByteArray& data);
		~WString();
		operator std::wstring() const;

	public:
		WString& operator=(const wchar_t* value);
		WString& operator=(const std::wstring& value);
		WString& operator=(const WString& value);


		WString operator+(const wchar_t* value) const;
		WString operator+(const std::wstring& value) const;
		WString operator+(const WString& value) const;

		WString& operator+=(const wchar_t* value);
		WString& operator+=(const std::wstring& value);
		WString& operator+=(const WString& value);

		bool operator==(const WString& value) const;
		bool operator!=(const WString& value) const;

		wchar_t& operator[](const size_t i) const;
	public:
		friend std::wostream& operator<<(std::wostream& out, const WString& value);
		friend std::wistream& operator>>(std::wistream& in, WString& value);

	public:
		WString append(const WString& value) const;

		bool indexOf(const WString& value, size_t& index, size_t start = 0) const;
		bool lastIndexOf(const WString& value, size_t& index, size_t start = 0) const;
		WString left(const size_t& index, bool include = false) const;
		WString left(const WString&, bool include = false) const;
		WString right(const size_t& index, bool include = false) const;
		WString right(const WString& data, bool include = false) const;
		WString mid(const size_t& startIndex, const size_t& endIndex, bool include = false) const;
		WString mid(const WString& start, const WString& end, bool include = false) const;
		WString replace(const WString& src, const WString& dest, const bool& once = false) const;
		WString replace(size_t start, size_t len, const WString& value) const;
		std::vector<WString> split(const WString& separator) const;
		WString toNativeSeparators() const;
		//WString toWString() const;
		WString toGbk() const;

		// 转换为大写字母
		WString toUpper() const;
		// 转换为小写字母
		WString toLower() const;
		//String toHex() const;

	public:
		wchar_t* data() const;
		const size_t size() const;
	private:
		Alastor::ByteArray m_buffer;
	};
}

