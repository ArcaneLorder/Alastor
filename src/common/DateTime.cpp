#include "DateTime.h"
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

//#include <string>

namespace Alastor
{
    DateTime::DateTime()
	{
	}

	long long DateTime::nowTimeStamp()
	{
		std::time_t now = std::time(nullptr);
		return static_cast<long long>(now) * 1000;
	}

    /// <summary>
    /// 获取系统时间
    /// </summary>
    /// <returns>系统时间格式化字符串</returns>
    std::string DateTime::nowTime(const std::string format)
    {
#ifdef _WIN32
        time_t tNowTime;
        time(&tNowTime);
        //struct tm t;
        tm tLocalTime;
        localtime_s(&tLocalTime,&tNowTime);
        char szTime[30] = { '\0' };
        strftime(szTime, 30, format.c_str(), &tLocalTime);
        std::string strTime = szTime;
        return strTime;
#else
        //std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
        //auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
        //std::time_t timestamp = tmp.count();

        //uint64_t milli = timestamp;
        //milli += (uint64_t)8 * 60 * 60 * 1000;
        //auto mTime = std::chrono::milliseconds(milli);
        //tp = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>(mTime);
        //auto tt = std::chrono::system_clock::to_time_t(tp);
        //std::tm* now = std::gmtime(&tt);

        //char rst[27] = { 0 };
        //sprintf(rst, "[%04d-%02d-%02d %02d:%02d:%02d.%03d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, std::atoi(std::to_string(timestamp % 1000000).substr(3, 3).c_str()));
        struct tm* now;　　　　　struct timeval time; 　　　　　　gettimeofday(&time, NULL);		　　　　　　now = localtime(&time.tv_sec);				　　　　　　char rst[27] = { 0 };		　　　　　　if (NULL != now) { sprintf(rst, "[%04d-%02d-%02d %02d:%02d:%02d.%03d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, time.tv_usec / 1000); }return rst;
#endif
    }

    time_t DateTime::textToTimeStamp(const std::string time)
    {
        std::istringstream iss(time);
        std::tm tm;

        // 分隔符 根据实际需要调整
        char delimiter = ' ';

        // 使用strptime解析文本时间到tm结构体中
        iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        // 检查解析是否成功
        if (iss.fail())
        {
            std::cerr << "时间解析失败" << std::endl;
            return 0;
        }

        // 使用mktime将tm结构体转换为time_t时间戳
        time_t timeSinceEpoch = std::mktime(&tm);

        // 检查转换是否成功
        if (timeSinceEpoch == -1)
        {
            std::cerr << "时间转换失败" << std::endl;
            return 0;
        }
        return static_cast<long long>(timeSinceEpoch) * 1000;

    }

    std::string DateTime::format(const std::string time, const std::string format)
    {
        std::istringstream iss(time);
        std::tm tm;

        // 分隔符 根据实际需要调整
        char delimiter = ' ';

        // 使用strptime解析文本时间到tm结构体中
        iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        // 检查解析是否成功
        if (iss.fail())
        {
            std::cerr << "时间解析失败" << std::endl;
            return "";
        }

        char szTime[30] = { '\0' };
        strftime(szTime, 30, format.c_str(), &tm);
        std::string strTime = szTime;
        return strTime;
    }

}
