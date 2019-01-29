
#include <iostream>
#include <fstream>

#include "Logger.h"
using namespace std;


    std::ofstream Logger::m_logFile;
    const std::string Logger::m_FileName = ".\\MyAppLogs.log";

    Logger& Logger::operator<<(const std::string& message)
    {
        m_logFile << endl << __TIMESTAMP__ << " :\t" << message;
        return *this;
    }

    Logger& Logger::operator<<(const int errorcode)
    {
        m_logFile <<" " << errorcode;
        return *this;
    }


    Logger::~Logger()
    {
        m_logFile.close();

    }
