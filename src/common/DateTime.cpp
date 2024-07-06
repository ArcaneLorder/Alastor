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
    /// ��ȡϵͳʱ��
    /// </summary>
    /// <returns>ϵͳʱ���ʽ���ַ���</returns>
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
        struct tm* now;����������struct timeval time; ������������gettimeofday(&time, NULL);		������������now = localtime(&time.tv_sec);				������������char rst[27] = { 0 };		������������if (NULL != now) { sprintf(rst, "[%04d-%02d-%02d %02d:%02d:%02d.%03d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, time.tv_usec / 1000); }return rst;
#endif
    }

    time_t DateTime::textToTimeStamp(const std::string time)
    {
        std::istringstream iss(time);
        std::tm tm;

        // �ָ��� ����ʵ����Ҫ����
        char delimiter = ' ';

        // ʹ��strptime�����ı�ʱ�䵽tm�ṹ����
        iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        // �������Ƿ�ɹ�
        if (iss.fail())
        {
            std::cerr << "ʱ�����ʧ��" << std::endl;
            return 0;
        }

        // ʹ��mktime��tm�ṹ��ת��Ϊtime_tʱ���
        time_t timeSinceEpoch = std::mktime(&tm);

        // ���ת���Ƿ�ɹ�
        if (timeSinceEpoch == -1)
        {
            std::cerr << "ʱ��ת��ʧ��" << std::endl;
            return 0;
        }
        return static_cast<long long>(timeSinceEpoch) * 1000;

    }

    std::string DateTime::format(const std::string time, const std::string format)
    {
        std::istringstream iss(time);
        std::tm tm;

        // �ָ��� ����ʵ����Ҫ����
        char delimiter = ' ';

        // ʹ��strptime�����ı�ʱ�䵽tm�ṹ����
        iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        // �������Ƿ�ɹ�
        if (iss.fail())
        {
            std::cerr << "ʱ�����ʧ��" << std::endl;
            return "";
        }

        char szTime[30] = { '\0' };
        strftime(szTime, 30, format.c_str(), &tm);
        std::string strTime = szTime;
        return strTime;
    }

}
