#pragma once

#ifndef STATIC_LINKAGE
#define BOOST_LOG_DYN_LINK 1 // necessary when linking the boost_log library dynamically
#endif

#include "pch.h"
#include "settings.h"

namespace logger
{

using LoggerType = boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>;
using LineType = std::uint32_t;

static constexpr std::string_view file_key = "File";
static constexpr std::string_view line_key = "Line";
static constexpr std::string_view function_key = "Function";

static constexpr std::string_view timestamp_key = "TimeStamp";
static constexpr std::string_view thread_id_key = "ThreadID";

}

#define LOG_SEV(lg, lvl) \
    BOOST_LOG_SEV((lg), (lvl)) \
        << boost::log::add_value(logger::file_key.data(), logger::Settings::get_path_file_name(__FILE__)) \
        << boost::log::add_value(logger::line_key.data(), static_cast<logger::LineType>(__LINE__)) \
        << boost::log::add_value(logger::function_key.data(), __func__)

BOOST_LOG_GLOBAL_LOGGER(Logger, logger::LoggerType)
