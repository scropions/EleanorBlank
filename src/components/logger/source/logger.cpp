// logger.cpp
#include "pch.h"
#include "logger.h"
#include "settings.h"

using namespace logger;

BOOST_LOG_GLOBAL_LOGGER_INIT(Logger, LoggerType)
{
    return LoggerType{};
}
