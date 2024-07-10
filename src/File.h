#pragma once

#include "String.h"
#include <map>
#include <list>



namespace Alastor
{
	enum OPEN_TYPE {
		ReadText,
		ReadAndWriteText,
		ReadBytes,
		ReadAndWriteBytes,
		CreateClearReadText,
		CreateClearReadAndWriteText,
		CreateClearReadBytes,
		CreateClearReadAndWriteBytes,
		CreateWriteTextTail,
		CreateWriteBytesTail,
		CreateReadAndWriteBytesTail
	};
	class ALASTOR_API File
	{
	public:
		File();
		~File();
		bool open(const String& filepath, OPEN_TYPE type = CreateReadAndWriteBytesTail);
		bool read(size_t index, size_t len, ByteArray& buffer) const;
		bool write(const char* buffer, size_t size);
		bool write(const String& value);
		bool write(const ByteArray& value);
		// 重定位文件指针
		bool seek(size_t index) const;
		bool close();
		bool flush();
		bool clear();
		const String& filepath() const;
		char operator[](size_t index) const;
		bool isOpen() { return m_file != NULL; }
	private:
		String m_path;
		FILE* m_file;
	public:
		static bool size(const String& filepath, size_t& size);
		static bool exist(const String& filepath);
		static bool remove(const String& filepath);
		static bool rename(const String& src,const String& dst);
		static void list(const std::string& path, std::vector<std::string>& list,bool recursion = false);
		static bool write(const String& filepath, const char* data, size_t len);
		static bool write(const String& filepath, const ByteArray& data);
		static bool write(const String& filepath, const String& data);
		static bool append(const String& filepath, const ByteArray& data);
		static bool read(const String& filepath, ByteArray& data, OPEN_TYPE type = ReadBytes);
		static bool copy(const String& src, const String& dst, bool cover = true);
		static String directory(const String& path);
		static String filename(const String& path);

		/*
			取文件后缀
		*/
		static std::string suffix(const std::string& path);
		static ByteArray icon(const std::string& exePath, int iconIndex = 0);

		/*
			取临时文件名
		*/
		static std::string tempPath();

		static ByteArray resources(char* type, char* index);

		static std::string description(const std::string& path);//utf8
		static std::string company(const std::string& path);//utf8

		static bool move(const std::string& src, const std::string& dst);

	};
}

