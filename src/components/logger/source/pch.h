#pragma once
#include <string>
#include <filesystem>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/exception_handler.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/system/error_code.hpp>
#include <boost/filesystem/operations.hpp>

#include <boost/dll.hpp>
#include <common/consts.h>
#include <common/status_code_macro.h>
#include <interfaces/config/iconfig.h>