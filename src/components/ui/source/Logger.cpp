#include "pch.h"
#include "Logger.h"

namespace UIEngine
{

Logger::Logger()
{

}

Logger::~Logger()
{

}

void Logger::LogMessage(ultralight::LogLevel log_level,
                        const ultralight::String& message)
{
    ultralight::String msg(message);
    std::cout << msg.utf8().data() << std::endl;
}

}