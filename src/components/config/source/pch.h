#pragma once
#include <string>
#include <mutex>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4702)
#endif
#include <boost/property_tree/ptree.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/stream_translator.hpp>
#include <boost/property_tree/string_path.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <common/status_code_macro.h>
#include <common/consts.h>
#include <utils/exception_catch.h>

#include "defines.h"
