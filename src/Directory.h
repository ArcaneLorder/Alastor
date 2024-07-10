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
			ö��������(��'\'��β)
		*/
		static std::vector<std::string> drives();

		/*
			ö����Ŀ¼(��Ŀ¼����)
		*/
		static std::vector<std::string> sub(const std::string& path, bool recursion = false);

		/*
			ȡ����Ŀ¼(��'\'��β)
		*/
		static std::string current();
		/*
			ȡ��Ŀ¼
			��:
				����:C:\\System32\\a.exe
				���:C:\\System32
		*/
		static String parent(const String& path);

		/*
			����Ŀ¼
		*/
		static bool create(const String& filepath, bool recursion = true);

		/*
			��ʽ��Ŀ¼·��(Ĭ����'\'��β)
		*/
		static String format(const String& path, bool tailSlash = true);

		/*
			ɾ��Ŀ¼(�ݹ�ɾ��Ŀ¼���ļ�)
		*/
		static bool remove(const std::string& path);

		/*
			ȡ��ʱ�ļ���·��(��'\'��β)
			����:C:\\Users\\Administrator\\AppData\\Local\\Temp\\
		*/
		static std::string temp();

		/*
			����:ȡ����·��(��'\'��β)
			����:C:\\Users\\Administrator\\Desktop\\
		*/
		static std::string desktop();
	};

}
