// main.cpp

#include "pch.h"
#include "config.h"
#include "keep_alive_data.h"

namespace config
{

IConfig& get_app()
{
    return Config::instance();
}

StatusCode load_keep_alive_data(std::string_view root, KeepAliveData& keep_alive_data) noexcept
{
    keep_alive_data.clear();

    try
    {
        bool enabled = false;
        CHECK_SC_R(get_app().get_bool(std::string{ root } +"/keep_alive_enabled", enabled));
        std::int32_t timeout = 0;
        CHECK_SC_R(get_app().get_int(std::string{ root } +"/keep_alive_timeout", timeout));
        std::int32_t cnt = 0;
        CHECK_SC_R(get_app().get_int(std::string{ root } +"/keep_alive_count", cnt));
        std::int32_t intverval = 0;
        CHECK_SC_R(get_app().get_int(std::string{ root } +"/keep_alive_intverval", intverval));
        keep_alive_data = std::move(KeepAliveData
        {
            enabled,
            static_cast<std::uint32_t>(timeout),
            static_cast<std::uint32_t>(cnt),
            static_cast<std::uint32_t>(intverval) 
        });
    }
    catch (...)
    {
        LOG_ERROR << "Config failed to load keep alive data: " << utils::what_s() << ".";
        return StatusCode::Bad;
    }

    return StatusCode::Good;
}

} // namespace config
