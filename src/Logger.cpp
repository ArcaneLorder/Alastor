#include "Logger.h"

namespace Alastor
{
    Logger* Logger::m_instance = new Logger();

	Logger::Logger()
	{
        m_pattern = "{}";
        m_outFile = true;
        setLevel(LogLevel::Debug);

	}

    Logger* Logger::instance()
    {
        return m_instance;
    }

    void Logger::setLevel(LogLevel level)
    {
        std::lock_guard<std::mutex> guard(m_ioMutex);
        m_level = level;
    }

    void Logger::setPattern(std::string pattern)
    {
        std::lock_guard<std::mutex> guard(m_ioMutex);
        m_pattern = pattern;
    }

    void Logger::clear()
    {
        std::lock_guard<std::mutex> guard(m_ioMutex);
        m_fileMutex.clear();
    }

    bool Logger::writeToFile(std::string filename, std::string content)
    {
        std::ofstream file(filename, std::ios::binary | std::ios::app);

        // 检查文件流状态
        if (file.good())
        {
            // 输出结尾字符串
            file << content << "\n";
            // 关闭文件流
            file.close();
            return true;
        }
        else
        {
            return false;
        }
    }


    std::vector<std::string> Logger::split(std::string str, std::string pattern)
    {
        std::string::size_type pos;
        std::vector<std::string> result;
        str += pattern;//扩展字符串以方便操作
        size_t size = str.size();

        for (size_t i = 0; i < size; i++)
        {
            pos = str.find(pattern, i);
            if (pos < size)
            {
                std::string s = str.substr(i, pos - i);
                result.push_back(s);
                i = pos + pattern.size() - 1;
            }
        }
        return result;
    }



    std::ostream& Logger::red(std::ostream& s)
    {
#ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
#else
        s << BOLDRED;
#endif
        return s;
    }

    std::ostream& Logger::yellow(std::ostream& s)
    {
#ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
#else
        s << BOLDYELLOW;
#endif
        return s;
    }

    std::ostream& Logger::blue(std::ostream& s)
    {
#ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
#else
        s << BOLDBLUE;
#endif
        return s;
    }

    std::ostream& Logger::green(std::ostream& s)
    {
#ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
#else
        s << BOLDGREEN;
#endif
        return s;
    }

    std::ostream& Logger::white(std::ostream& s)
    {
#ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
#else
        s << BOLDWHITE;
#endif
        return s;
    }

    std::ostream& Logger::reset(std::ostream& s)
    {
#ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
#else
        s << RESET;
#endif
        return s;
    }


}


