#pragma once
#include "Alastor.h"
#include "String.h"

namespace Alastor
{
	class ALASTOR_API Directory 
	{
	public:
		Directory();
	public:
		/*
			枚举驱动器(以'\'结尾)
		*/
		static std::vector<std::string> drives();

		/*
			枚举子目录(仅目录名称)
		*/
		static std::vector<std::string> sub(const std::string& path, bool recursion = false);

		/*
			取运行目录(以'\'结尾)
		*/
		static std::string current();
		/*
			取父目录
			例:
				输入:C:\\System32\\a.exe
				输出:C:\\System32
		*/
		static String parent(const String& path);

		/*
			创建目录
		*/
		static bool create(const String& filepath, bool recursion = true);

		/*
			格式化目录路径(默认以'\'结尾)
		*/
		static String format(const String& path, bool tailSlash = true);

		/*
			删除目录(递归删除目录下文件)
		*/
		static bool remove(const std::string& path);

		/*
			取临时文件夹路径(以'\'结尾)
			返回:C:\\Users\\Administrator\\AppData\\Local\\Temp\\
		*/
		static std::string temp();

		/*
			功能:取桌面路径(以'\'结尾)
			返回:C:\\Users\\Administrator\\Desktop\\
		*/
		static std::string desktop();
	};

}
