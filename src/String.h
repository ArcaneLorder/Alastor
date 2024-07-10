#pragma once
#include "Common.h"
#include "ByteArray.h"
#include <string>
#include <iostream>
#include "WString.h"

#include<sstream>


namespace Alastor 
{
	class ALASTOR_API String
	{
	public:
		String();
		String(const char value);
		String(const char* value);
		String(const char* value, size_t size);
		String(const std::string& value);
		String(const String& value);
		String(const ByteArray& data);
		~String();
		operator std::string() const;


	public:
		String& operator=(const char* value);
		String& operator=(const std::string& value);
		String& operator=(const String& value);
		

		String operator+(const char* value) const;
		String operator+(const std::string& value) const;
		String operator+(const String& value) const;

		String& operator+=(const char* value);
		String& operator+=(const std::string& value);
		String& operator+=(const String& value);

		bool operator==(const String& value) const;
		bool operator!=(const String& value) const;

		char& operator[](const size_t i) const;
	public:
		friend std::ostream& operator<<(std::ostream& out, const String& value);
		friend std::istream& operator>>(std::istream& in, String& value);

	public:
		String append(const String& value) const;

		bool indexOf(const String& value, size_t& index, size_t start = 0) const;
		bool lastIndexOf(const String& value, size_t& index, size_t start = 0) const;
		String left(const size_t& index, bool include = false) const;
		String left(const String& ,bool include = false) const;
		String right(const size_t& index, bool include = false) const;
		String right(const String& data, bool include = false) const;
		String mid(const size_t& startIndex, const size_t& endIndex, bool include = false) const;
		String mid(const String& start, const String& end, bool include = false) const;
		String replace(const String& src, const String& dest, const bool& once = false) const;
		String replace(size_t start, size_t len, const String& value) const;
		std::vector<String> split(const String& separator) const;
		String toNativeSeparators() const;

		// 转换为大写字母
		String toUpper() const;
		// 转换为小写字母
		String toLower() const;
		//String toHex() const;
	public:
		static String number(int Val);
		static String number(unsigned int Val);
		static String number(long Val);
		static String number(unsigned long Val);
		static String number(long long Val);
		static String number(unsigned long long Val);
		static String number(double Val);
		static String number(float Val);
		static String number(long double Val);

	public:

		template<typename T>
		static String fromNumber(T num)
		{
			std::string result;
			std::stringstream ss;
			ss << num;
			ss >> result;
			return result;
		};

		template<typename T>
		T toNumber() 
		{
			T result;
			std::stringstream ss;
			ss << data();
			ss >> result;
			return result;
		};

	public:
		char* data() const;
		const size_t size() const;
	private:
		Alastor::ByteArray m_buffer;
	};
}

