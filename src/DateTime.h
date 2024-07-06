#pragma once
#include "Common.h"
#include <string>
namespace Alastor
{
	class ALASTOR_API DateTime
	{
	public:
		explicit DateTime();
	public:
		/*
			��ǰʱ���
			����:1714787791611
		*/
		static long long nowTimeStamp();

		/*
			��ǰʱ��
		*/
		static std::string nowTime(const std::string format = "%Y-%m-%d %H:%M:%S");

		/*
			�ı�ʱ��תʱ���
		*/
		static long long textToTimeStamp(const std::string time);

		/*
			�ı�ʱ��תָ����ʽ
		*/
		static std::string format(const std::string time, const std::string format= "%Y-%m-%d %H:%M:%S");
	};
}

