#pragma once

enum class StatusCode : std::uint32_t;

namespace config
{

class IConfig
{
public:
    virtual ~IConfig() {}

    virtual StatusCode init(const std::string_view file_name, const std::string_view template_xml) noexcept = 0;
    virtual StatusCode get_string(const std::string_view path, std::string& value) const noexcept = 0;
    virtual StatusCode set_string(const std::string_view path, const std::string& value) noexcept = 0;
    virtual StatusCode get_int(const std::string_view path, std::int32_t& value) const noexcept = 0;
    virtual StatusCode set_int(const std::string_view path, std::int32_t value) noexcept = 0;
    virtual StatusCode get_double(const std::string_view path, double& value) const noexcept = 0;
    virtual StatusCode set_double(const std::string_view path, const double& value) noexcept = 0;
    virtual StatusCode get_bool(const std::string_view path, bool& value) const noexcept = 0;
    virtual StatusCode set_bool(const std::string_view path, bool value) noexcept = 0;
};

} // namespace config