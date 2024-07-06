#pragma once
#include "Alastor.h"
#include "String.h"
#include "Base64.h"

namespace Alastor 
{
	class ALASTOR_API Codec 
	{
	public:
		explicit Codec();

	public:
		static char toLower(char c);
		static char toUpper(char c);
		static wchar_t toWLower(wchar_t wc);
		static wchar_t toWUpper(wchar_t wc);
		static std::string toHex(ULONG_PTR number,bool pre = false);

		// 十六进制字符串转文本
		// 文本转十六进制字符串
		static int hexToDec(const std::string& hex);
		static std::string decToHex(const int dec);

		static String ansiToUtf8(String src);
		static WString ansiToUnicode(String src);
		static WString utf8ToUnicode(String src);
		static String utf8ToAnsi(String src);
		static String unicodeToAnsi(WString src);
		static String unicodeToUtf8(WString src);
		static std::string urlEncode(const std::string& src);
		static std::string urlDecode(const std::string& src);

		//static bool zipCompress(std::string directory);
		static bool zipUncompress(std::string path, std::string directory);

	
	public:
		static String md5(ByteArray data);
		static ByteArray aes(ByteArray data);
		static ByteArray base64Decode(String data);
		static String base64Encode(ByteArray data);

	};
}