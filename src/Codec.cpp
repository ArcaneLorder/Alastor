#include "Codec.h"
#include "MD5.h"
#include "Crypt/WjCryptLib_Md5.h"
#include "Crypt/WjCryptLib_Aes.h"
#include <iostream>
#include <stdio.h>

#if defined(_MSC_VER)
#include <Windows.h>
#elif defined(__unix__)
#else
#endif

#include "Zip/Zip.h"

namespace Alastor
{

	Codec::Codec()
	{
	}

	char Codec::toLower(char c)
	{
		if (c >= 'A' && c <= 'Z') 
		{
			return c - 'A' + 'a';
		}
		return c;
	}

	char Codec::toUpper(char c)
	{
		if (c >= 'a' && c <= 'z') 
		{
			return c - 'a' + 'A';
		}
		return c;
	}

	wchar_t Codec::toWLower(wchar_t wc)
	{
		if (wc >= L'A' && wc <= L'Z')
		{
			return wc - L'A' + L'a';
		}
		return wc;
	}

	wchar_t Codec::toWUpper(wchar_t wc)
	{
		if (wc >= L'a' && wc <= L'z')
		{
			return wc - L'a' + L'A';
		}
		return wc;
	}

	std::string Codec::toHex(ULONG_PTR number, bool pre)
	{
		std::stringstream ss;
		if (pre)
		{
			ss << "0x";
		}
		ss << std::hex << std::uppercase;
		ss << number;
		return ss.str();
	}

	bool Codec::zipUncompress(std::string path, std::string directory)
	{
		return zip_extract(path.c_str(), directory.c_str(),NULL,NULL) == 0;
	}

	String Codec::md5(ByteArray data)
	{
		MD5_HASH hash;
		Md5Calculate(data.data(),(uint32_t)data.size(),&hash);
		return ByteArray((char*)hash.bytes, sizeof(hash)).toHex();
	}

	ByteArray Codec::aes(ByteArray data)
	{
		return ByteArray();
	}

	ByteArray Codec::base64Decode(String data)
	{
		size_t outSize = BASE64_DECODE_OUT_SIZE(data.size());
		std::cout << "预计输出长度:" << outSize << std::endl;
		if (outSize == 0)
		{
			return ByteArray();
		}
		ByteArray result;
		result.resize(outSize);
		size_t size = base64_decode(data.data(), (unsigned int)data.size(), (unsigned char*)(result.data()));
		std::cout << "实际输出长度:" << size << std::endl;
		//if (size > outSize)
		//{
		//	return ByteArray();
		//}
		return result;
	}

	String Codec::base64Encode(ByteArray data)
	{
		if (data.size() == 0)
		{
			return String();
		}

		size_t outSize = BASE64_ENCODE_OUT_SIZE(data.size());
		if (outSize == 0)
		{
			return String();
		}
		ByteArray result(outSize);
		size_t size = base64_encode((unsigned char*)(data.data()), (unsigned int)data.size(), result.data());
		//if (size > outSize)
		//{
		//	return String();
		//}
		return result;
	}



	int Codec::hexToDec(const std::string& hex)
	{
		// 跳过 '#' 或 '0x' 前缀
		size_t startIndex = 0;
		if ((hex.size() >= 2 && (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X'))) ||
			(hex.size() >= 1 && hex[0] == '#')) {
			startIndex = (hex[0] == '#' ? 1 : 2); 
		}

		int dec = 0;
		int base = 1;
		for (size_t i = hex.size() - 1; i >= startIndex; --i) {
			char c = hex[i];
			if (!::isxdigit(c)) {
				// 非法字符，可以抛出异常或返回错误代码  
				throw std::invalid_argument("Invalid hexadecimal string");
			}
			int digitValue = ::tolower(c) - (c <= '9' ? '0' : ('a' - 10));
			dec += digitValue * base;
			base *= 16;
		}
		return dec;
	}

	std::string Codec::decToHex(const int dec)
	{
		std::vector<char> hexChars;
		char hexDigits[] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','\0' };
		// char hexDigits[] = "0123456789abcdef";

		unsigned int uDec = dec; // 避免负数情况  
		while (uDec > 0) {
			hexChars.push_back(hexDigits[uDec % 16]);
			uDec /= 16;
		}

		// 如果结果为 0，直接返回 "0"  
		if (hexChars.empty()) {
			hexChars.push_back('0');
		}

		// 逆序拼接字符  
		std::reverse(hexChars.begin(), hexChars.end());

		// 转换为字符串并返回  
		return std::string(hexChars.begin(), hexChars.end());
	}


	String Codec::ansiToUtf8(String src)
	{
		WString unicodeStr = ansiToUnicode(src);
		return unicodeToUtf8(unicodeStr);
	}

	WString Codec::ansiToUnicode(String src)
	{
		// 获取转换为宽字节后需要的缓冲区大小，创建宽字节缓冲区，936为简体中文GB2312代码页  
		int length = MultiByteToWideChar(CP_ACP, NULL, src.data(), -1, NULL, 0);
		if (length == 0)
		{
			return WString();
		}
		wchar_t* wideBuffer = new wchar_t[length];
		int conversionResult = MultiByteToWideChar(CP_ACP, NULL, src.data(), -1, wideBuffer, length);
		if (conversionResult == 0)
		{
			delete[] wideBuffer;
			return WString();
		}
		WString result = wideBuffer;

		delete[] wideBuffer;
		return result;
	}

    WString Codec::utf8ToUnicode(String src)
    {
#ifdef _WIN32
		int length = MultiByteToWideChar(CP_UTF8, 0, src.data(), -1, NULL, 0); // 获取需要转换的宽字节长度
		wchar_t* wideBuffer = new wchar_t[length]; // 创建宽字节缓冲区
		MultiByteToWideChar(CP_UTF8, 0, src.data(), -1, wideBuffer, length); // 将UTF-8转换为宽字节
		WString result = wideBuffer;
		delete[] wideBuffer;
		return result;
#else
#endif
    }

	String Codec::utf8ToAnsi(String src)
	{
		int length = MultiByteToWideChar(CP_UTF8, 0, src.data(), -1, NULL, 0); // 获取需要转换的宽字节长度
		wchar_t* wideBuffer = new wchar_t[length]; // 创建宽字节缓冲区
		MultiByteToWideChar(CP_UTF8, 0, src.data(), -1, wideBuffer, length); // 将UTF-8转换为宽字节


		String result = unicodeToAnsi(wideBuffer);
		delete[] wideBuffer; // 释放内存
		return result;
	}

	String Codec::unicodeToAnsi(WString src)
	{
		int bufferSize = WideCharToMultiByte(CP_ACP, 0, src.data(), -1, NULL, 0, NULL, NULL); // 获取所需的ANSI字节大小
		char* ansiStr = new char[bufferSize]; // 创建存放ANSI字符串的缓冲区
		WideCharToMultiByte(CP_ACP, 0, src.data(), -1, ansiStr, bufferSize, NULL, NULL); // 转换为ANSI字符串
		String result = ansiStr;
		delete[] ansiStr; // 释放内存
		return result;
	}

	String Codec::unicodeToUtf8(WString src)
	{
		// 获取转为UTF8多字节后需要的缓冲区大小，创建多字节缓冲区  
		int length = WideCharToMultiByte(CP_UTF8, NULL, src.data(), -1, NULL, NULL, NULL, NULL);
		char* szBuffer = new char[length];
		WideCharToMultiByte(CP_UTF8, NULL, src.data(), -1, szBuffer, length, NULL, NULL);
		String result = szBuffer;
		delete[] szBuffer;
		return result;
	}

	std::string Codec::urlEncode(const std::string& szToEncode)
	{
		std::string src = szToEncode;
		char hex[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','\0' };// "0123456789ABCDEF"
		string dst;


		for (size_t i = 0; i < src.size(); ++i)
		{
			unsigned char cc = src[i];
			if (cc >= 'A' && cc <= 'Z'
				|| cc >= 'a' && cc <= 'z'
				|| cc >= '0' && cc <= '9'
				|| cc == '.'
				|| cc == '_'
				|| cc == '-'
				|| cc == '*')
			{
				if (cc == ' ')
				{
					dst += "+";
				}
				else
					dst += cc;
			}
			else
			{
				unsigned char c = static_cast<unsigned char>(src[i]);
				dst += '%';
				dst += hex[c / 16];
				dst += hex[c % 16];
			}
		}
		return dst;
	}

	std::string Codec::urlDecode(const std::string& szToDecode)
	{
		std::string result;
		int hex = 0;
		for (size_t i = 0; i < szToDecode.length(); ++i)
		{
			switch (szToDecode[i])
			{
			case '+':
				result += ' ';
				break;
			case '%':
				if (isxdigit(szToDecode[i + 1]) && isxdigit(szToDecode[i + 2]))
				{
					std::string hexStr = szToDecode.substr(i + 1, 2);
					hex = strtol(hexStr.c_str(), 0, 16);
					//字母和数字[0-9a-zA-Z]、一些特殊符号[$-_.+!*'(),] 、以及某些保留字[$&+,/:;=?@]
					//可以不经过编码直接用于URL
					if (!((hex >= 48 && hex <= 57) ||	//0-9
						(hex >= 97 && hex <= 122) ||	//a-z
						(hex >= 65 && hex <= 90) ||	//A-Z
						//一些特殊符号及保留字[$-_.+!*'(),]  [$&+,/:;=?@]
						hex == 0x21 || hex == 0x24 || hex == 0x26 || hex == 0x27 || hex == 0x28 || hex == 0x29
						|| hex == 0x2a || hex == 0x2b || hex == 0x2c || hex == 0x2d || hex == 0x2e || hex == 0x2f
						|| hex == 0x3A || hex == 0x3B || hex == 0x3D || hex == 0x3f || hex == 0x40 || hex == 0x5f
						))
					{
						result += char(hex);
						i += 2;
					}
					else result += '%';
				}
				else {
					result += '%';
				}
				break;
			default:
				result += szToDecode[i];
				break;
			}
		}
		return result;
	}

}

