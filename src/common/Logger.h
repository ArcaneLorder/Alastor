
//https://www.cnblogs.com/Clark-Zhang/p/15699274.html
#pragma once
#pragma warning(disable:4996)

#include "Common.h"
#include "DateTime.h"

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#else
#include <stdarg.h>
#include <unistd.h>
#endif

#include <time.h>
#include <iostream>
#include <fstream>
#include <mutex>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <exception>



#ifndef _WIN32
#define RESET "\033[0m"
#define BLACK "\033[30m" /* Black */
#define RED "\033[31m" /* Red */
#define GREEN "\033[32m" /* Green */
#define YELLOW "\033[33m" /* Yellow */
#define BLUE "\033[34m" /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m" /* Cyan */
#define WHITE "\033[37m" /* White */
#define BOLDBLACK "\033[1m\033[30m" /* Bold Black */
#define BOLDRED "\033[1m\033[31m" /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m" /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m" /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m" /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m" /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m" /* Bold White */
#endif


#define LOGGER_DEBUG(...) Alastor::Logger::instance()->debug(__VA_ARGS__)
#define LOGGER_WARNING(...) Alastor::Logger::instance()->warning(__VA_ARGS__)
#define LOGGER_ERROR(...) Alastor::Logger::instance()->error(__VA_ARGS__)

namespace Alastor
{

    static const std::string enumstring[3] = { "Debug","Warning", "Error" };
    class ALASTOR_API Logger
    {
        /// <summary>
        /// 日志等级枚举
        /// </summary>
        typedef enum
        {
            Debug,
            Warning,
            Error
        }LogLevel;

    private:
        Logger();
    public:
        static Logger* instance();
    public:
        void setLevel(LogLevel level);
        void setPattern(std::string pattern);
        void clear();
    private:
        bool writeToFile(std::string filename, std::string content);
        std::vector<std::string> split(std::string str, std::string pattern);
    private:
        static std::ostream& red(std::ostream& s);
        static std::ostream& yellow(std::ostream& s);
        static std::ostream& blue(std::ostream& s);
        static std::ostream& green(std::ostream& s);
        static std::ostream& white(std::ostream& s);
        static std::ostream& reset(std::ostream& s);
    public:

        /// <summary>
        /// 读取参数
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="content"></param>
        /// <param name="args"></param>
        template<typename T>
        void readParameter(std::vector<std::string>& content, T&& args)
        {
            std::stringstream ss;
            ss << args;
            std::string s = ss.str();
            content.push_back(s);
        }

        void Log(std::vector<std::string>& content)
        {
        }

        template<typename T, typename... Args>
        void Log(std::vector<std::string>& content, T&& first, Args&&... args)
        {
            readParameter(content, std::forward<T>(first));
            Log(content, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void level_to_out(std::string content, LogLevel Level, Args... args)
        {
            std::string nowTime = DateTime::nowTime("[%Y-%m-%d %H:%M:%S]");
            try
            {
                std::lock_guard<std::mutex> guard(m_ioMutex);

                // 日志等级错误
                if (m_level > Level)
                {
                    return;
                }

                std::vector<std::string> cs = split(content, m_pattern);

                int paramcount = 0;
                paramcount = sizeof...(args);

                int amount = cs.size();

                
                if (Level == LogLevel::Debug)
                {
                    std::cout << nowTime << " [" << green << enumstring[(int)Level] << reset << "] ";
                }
                else if (Level == LogLevel::Warning)
                {
                    std::cout << nowTime << " [" << yellow << enumstring[(int)Level] << reset << "] ";
                }
                else if (Level == LogLevel::Error)
                {
                    std::cout << nowTime << " [" << red << enumstring[(int)Level] << reset << "] ";
                }
                else
                {
                    std::cout << nowTime << " [" << green << enumstring[(int)Level] << reset << "] ";
                }

                if (paramcount == 0 || amount == 1)
                {
                    std::cout << content << std::endl;
                }
                else
                {
                    std::vector<std::string> paramlist;

                    Log(paramlist, args...);

                    if (amount - 1 < paramcount)
                    {
                        for (int i = 0; i < amount - 1; ++i)
                        {
                            std::cout << cs[i] << paramlist[i];
                        }
                    }
                    else
                    {
                        for (int i = 0; i < paramcount; ++i)
                        {
                            std::cout << cs[i] << paramlist[i];
                        }

                        for (int i = paramcount; i < amount - 1; ++i)
                        {
                            std::cout << cs[i] << m_pattern;
                        }
                    }

                    std::cout << cs[amount - 1] << std::endl;
                }
            }
            catch (std::exception& ex)
            {
                std::cout << nowTime << " [" << red << "Error" << reset << "] " << ex.what() << std::endl;
            }
        }

        template<typename... Args>
        void level_to_file(std::string filename, std::string content, LogLevel loglevel, Args... args)
        {
            std::string nowTime = DateTime::nowTime("[%Y-%m-%d %H:%M:%S]");
            try
            {
                std::lock_guard<std::mutex> guard(m_fileMutex[filename]);

                if (m_level > loglevel) return;

                
                std::vector<std::string> cs = split(content, m_pattern);

                std::string filedata = nowTime + " [" + enumstring[(int)loglevel] + "] ";
                int paramcount = 0;
                paramcount = sizeof...(args);

                int amount = cs.size();

                if (paramcount == 0 || amount == 1)
                {
                    filedata += content;
                }
                else
                {
                    std::vector<std::string> paramlist;

                    Log(paramlist, args...);

                    if (amount - 1 < paramcount)
                    {
                        for (int i = 0; i < amount - 1; ++i)
                        {
                            filedata += cs[i] + paramlist[i];
                        }
                    }
                    else
                    {
                        for (int i = 0; i < paramcount; ++i)
                        {
                            filedata += cs[i] + paramlist[i];
                        }

                        for (int i = paramcount; i < amount - 1; ++i)
                        {
                            filedata += cs[i] + m_pattern;
                        }
                    }

                    filedata += cs[amount - 1];
                }

                if (!writeToFile(filename, filedata))
                {
                    std::cout << nowTime << " [" << red << "Error" << reset << "] " << "Write log file failed!" << std::endl;
                }
            }
            catch (std::exception& ex)
            {
                std::cout << nowTime << " [" << red << "Error" << reset << "] " << ex.what() << std::endl;
            }
        }

        template<typename... Args>
        void debug(std::string content, Args... args)
        {
            level_to_out(content, LogLevel::Debug, args...);
            if (m_outFile)
            {
                level_to_file(DateTime::nowTime("%Y-%m-%d.txt"), content, LogLevel::Debug, args...);
            }
        }

        template<typename... Args>
        void warning(std::string content, Args... args)
        {
            level_to_out(content, LogLevel::Warning, args...);
            if (m_outFile)
            {
                level_to_file(DateTime::nowTime("%Y-%m-%d.txt"), content, LogLevel::Warning, args...);
            }
        }

        template<typename... Args>
        void error(std::string content, Args... args)
        {
            level_to_out(content, LogLevel::Error, args...);
            if (m_outFile)
            {
                level_to_file(DateTime::nowTime("%Y-%m-%d.txt"), content, LogLevel::Error, args...);
            }
        }
    public:
        static Logger* m_instance;
        // 互斥体
        std::mutex m_ioMutex;
        // 文件互斥体映射
        std::map<std::string, std::mutex> m_fileMutex;
        // 日志等级
        LogLevel m_level;
        // 是否输出文件
        bool m_outFile;
        // 通配符
        std::string m_pattern;
    };


}
