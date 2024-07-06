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
			当前时间戳
			返回:1714787791611
		*/
		static long long nowTimeStamp();

		/*
			当前时间
		*/
		static std::string nowTime(const std::string format = "%Y-%m-%d %H:%M:%S");

		/*
			文本时间转时间戳
		*/
		static long long textToTimeStamp(const std::string time);

		/*
			文本时间转指定格式
		*/
		static std::string format(const std::string time, const std::string format= "%Y-%m-%d %H:%M:%S");
	};
}

