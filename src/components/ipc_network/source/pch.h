#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <common/status_code_macro.h>
#include <interfaces/logger/ilogger.h>
#include <utils/exception_catch.h>
