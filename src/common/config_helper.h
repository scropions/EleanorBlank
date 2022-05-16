#pragma once

#include <interfaces/config/iconfig.h>
#include <components/config/source/keep_alive_data.h>

namespace config
{

IConfig& get_app();

StatusCode load_keep_alive_data(std::string_view root, KeepAliveData& keep_alive_data) noexcept;

} 