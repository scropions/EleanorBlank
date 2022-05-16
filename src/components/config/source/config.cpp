#include "pch.h"
#include "config.h"

namespace config
{

Config& Config::instance()
{
    static Config inst;
    return inst;
}

StatusCode Config::init(const std::string_view file_name, const std::string_view template_xml) noexcept
{
    std::lock_guard<std::mutex> guard(_mutex);

    if (_initialized)
    {
        return StatusCode::GoodAlreadyInitialized;
    }

    CHECK_SC_R(_doc.init(file_name, template_xml));

    _initialized = true;

    return StatusCode::Good;
}

StatusCode Config::get_string(std::string_view path, std::string& value) const noexcept
{
    CHECK_R2(_initialized, StatusCode::NotInitialized);

    return get_value<std::string>(path, value);
}

StatusCode Config::set_string(std::string_view path, const std::string& value) noexcept
{
    CHECK_R2(_initialized, StatusCode::NotInitialized);

    return set_value<std::string>(path, value);
}

StatusCode Config::get_int(std::string_view path, std::int32_t& value) const noexcept
{
    CHECK_R2(_initialized, StatusCode::NotInitialized);

    value = 0;

    return get_value<std::int32_t>(path, value);
}

StatusCode Config::set_int(std::string_view path, std::int32_t value) noexcept
{
    CHECK_R2(_initialized, StatusCode::NotInitialized);

    return set_value<std::int32_t>(path, value);
}

StatusCode Config::get_double(std::string_view path, double& value) const noexcept
{
    CHECK_R2(_initialized, StatusCode::NotInitialized);

    value = 0.0;

    return get_value<double>(path, value);
}

StatusCode Config::set_double(std::string_view path, const double& value) noexcept
{
    CHECK_R2(_initialized, StatusCode::NotInitialized);

    return set_value<double>(path, value);
}

StatusCode Config::get_bool(std::string_view path, bool& value) const noexcept
{
    CHECK_R2(_initialized, StatusCode::NotInitialized);

    value = false;

    return get_value<bool>(path, value);
}

StatusCode Config::set_bool(std::string_view path, bool value) noexcept
{
    CHECK_R2(_initialized, StatusCode::NotInitialized);

    return set_value<bool>(path, value);
}

} // namespace config