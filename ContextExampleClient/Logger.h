#pragma once
#include<fstream>
#include <iostream>
#include<string>

class Logger
{
public:
    inline static Logger& getLoogerObject()
    {
        
        static Logger log;
        
        if (!m_logFile.is_open())
        {
            m_logFile.open(m_FileName, std::ios::out | std::ios::app);
        }

        return log;
    }

    Logger& operator<<(const std::string& message);
    Logger& operator<<(const int errorcode);
    
private:
    static std::ofstream m_logFile;
    static const std::string m_FileName;
    Logger() = default;
    ~Logger();
    Logger(const Logger& ref) = delete;
    Logger& operator= (const Logger& ref) = delete;

};


//Logger& operator<<(const std::string& message);