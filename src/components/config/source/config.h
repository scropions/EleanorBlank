#pragma once

#include "document.h"

namespace config
{

class Config : public IConfig
{
public:
    static Config& instance();

private:
    Config() = default;
    virtual ~Config() override = default;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

public:
// IConfig
    virtual StatusCode init(const std::string_view file_name, const std::string_view template_xml) noexcept override;
    virtual StatusCode get_string(const std::string_view path, std::string& value) const noexcept override;
    virtual StatusCode set_string(const std::string_view path, const std::string& value) noexcept override;
    virtual StatusCode get_int(const std::string_view path, std::int32_t& value) const noexcept override;
    virtual StatusCode set_int(const std::string_view path, std::int32_t value) noexcept override;
    virtual StatusCode get_double(const std::string_view path, double& value) const noexcept override;
    virtual StatusCode set_double(const std::string_view path, const double& value) noexcept override;
    virtual StatusCode get_bool(const std::string_view path, bool& value) const noexcept override;
    virtual StatusCode set_bool(const std::string_view path, bool value) noexcept override;

private:
    template<typename T>
    StatusCode get_value(std::string_view path, T& value) const noexcept
    {
        try
        {
            std::lock_guard<std::mutex> guard(_mutex);
            CHECK_SC_R(_doc.refresh());
            value = _doc.get_value<T>(std::string{ path });
            return StatusCode::Good;
        }
        catch (...)
        {
            LOG_ERROR << "Config failed to get value: " << utils::what_s() << ".";
            return StatusCode::Unexpected;
        }
    }

    template<typename T>
    StatusCode set_value(std::string_view path, const T& value) noexcept
    {
        try
        {
            std::lock_guard<std::mutex> guard(_mutex);
            CHECK_SC_R(_doc.refresh());
            _doc.set_value<T>(std::string{ path }, value);
            CHECK_SC_R(_doc.save());
            return StatusCode::Good;
        }
        catch (...)
        {
            LOG_ERROR << "Config failed to set value: " << utils::what_s() << ".";
            return StatusCode::Unexpected;
        }
    }

private:
    mutable std::mutex _mutex;
    mutable Document _doc;
    std::atomic_bool _initialized = false;
};

} // namespace config