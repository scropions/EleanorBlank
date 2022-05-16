#pragma once

#include "interfaces/config/iconfig.h"
#include "components/logger/source/logger.h" // exclude it 

namespace logger
{
    StatusCode init(config::IConfig& config) noexcept;
    void set_severity_level(boost::log::trivial::severity_level) noexcept;
    boost::log::trivial::severity_level get_severity_level() noexcept;
    std::string_view get_default_config_xml() noexcept;
    StatusCode default_setup() noexcept;

} // namespace logger

#define LOG_TRACE LOG_SEV(Logger::get(), boost::log::trivial::trace)
#define LOG_DEBUG LOG_SEV(Logger::get(), boost::log::trivial::debug)
#define LOG_INFO  LOG_SEV(Logger::get(), boost::log::trivial::info)
#define LOG_WARN  LOG_SEV(Logger::get(), boost::log::trivial::warning)
#define LOG_ERROR LOG_SEV(Logger::get(), boost::log::trivial::error)
#define LOG_FATAL LOG_SEV(Logger::get(), boost::log::trivial::fatal)

#define LOG_VAR(var) LOG_INFO << #var << " : " << var
