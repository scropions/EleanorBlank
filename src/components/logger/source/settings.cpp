#include "pch.h"
#include "settings.h"

namespace logger
{

Settings& Settings::instance()
{
    static Settings inst;
    return inst;
}

void Settings::try_load_from_config(config::IConfig& config)
{
    try_load_severity_level(config);
    try_load_file_target(config);
    try_load_file_rotation_size(config);
    try_load_file_name_time_fmt(config);
    try_load_file_name_sep(config);
    try_load_file_name_suff(config);
    try_load_add_console_log(config);
    try_load_file_auto_flush(config);
    try_load_file_collector_max_files(config);
    try_load_file_collector_clear_history(config);
    try_load_message_source_short_path_separator(config);
    try_load_message_time_fmt(config);
}

boost::log::trivial::severity_level Settings::get_severity_level() const noexcept
{
    return _severity_level;
}

void Settings::set_severity_level(boost::log::trivial::severity_level lvl) noexcept
{
    _severity_level = lvl;
}

const std::string& Settings::get_file_target() const noexcept
{
    return _file_target;
}

std::int32_t Settings::get_file_rotation_size() const noexcept
{
    return file_rotation_size_default;
}

const std::string& Settings::get_file_name_time_fmt() const noexcept
{
    return _file_name_time_fmt;
}

const std::string& Settings::get_file_name_sep() const noexcept
{
    return _file_name_sep;
}

const std::string& Settings::get_file_name_suff() const noexcept
{
    return _file_name_suff;
}

bool Settings::get_add_console_log() const noexcept
{
    return _add_console_log;
}

bool Settings::get_file_auto_flush() const noexcept
{
    return _file_auto_flush;
}

std::int32_t Settings::get_file_collector_max_files() const noexcept
{
    return _file_collector_max_files;
}

bool Settings::get_file_collector_clear_history() const noexcept
{
    return _file_collector_clear_history;
}

const std::string& Settings::get_message_source_short_path_separator() const noexcept
{
    return _message_source_short_path_separator;
}

const std::string& Settings::get_message_time_fmt() const noexcept
{
    return _message_time_fmt;
}

std::string Settings::get_path_short_name(std::string_view fn) noexcept
{
    try
    {
        const auto& sep = get_settings().get_message_source_short_path_separator();
        auto pos = fn.find(sep);

        if (std::string_view::npos == pos)
        {
            return std::string{ fn };
        }

        fn.remove_prefix(pos + sep.length());

        return std::string{ fn };
    }
    catch (...)
    {
        return std::string{};
    }
}

std::string Settings::get_path_file_name(std::string_view name) noexcept
{
    try
    {
        return std::filesystem::path(name).filename().u8string();
    }
    catch (...)
    {
        return { };
    }
}

std::string Settings::format_time(const boost::posix_time::ptime& t) const noexcept
{
    try
    {
        using namespace boost::posix_time;

        static std::locale loc(std::cout.getloc(), new time_facet(_file_name_time_fmt.c_str()));

        std::stringstream ss;
        auto old_loc = ss.imbue(loc);

        ss << t;

        ss.imbue(old_loc);

        return ss.str();
    }
    catch (...)
    {
        return std::string{};
    }
}

void Settings::try_load_severity_level(config::IConfig& config) noexcept
{
    try
    {
        std::int32_t v;

        if (status_code_helper::is_good(config.get_int(severity_level_key, v)))
        {
            if (boost::log::trivial::severity_level::trace <= v && v <= boost::log::trivial::severity_level::fatal)
            {
                _severity_level = static_cast<boost::log::trivial::severity_level>(v);
            }
        }
    }
    catch (...)
    {
    }
}

void Settings::try_load_file_target(config::IConfig& config) noexcept
{
    std::string v;

    if (status_code_helper::is_good(config.get_string(file_target_key, v)))
    {
        _file_target.swap(v);
    }
}

void Settings::try_load_file_rotation_size(config::IConfig& config) noexcept
{
    std::int32_t v;

    if (status_code_helper::is_good(config.get_int(file_rotation_size_key, v)))
    {
        if (0 < _file_rotation_size)
        {
            _file_rotation_size = v;
        }
    }
}

void Settings::try_load_file_name_time_fmt(config::IConfig& config) noexcept
{
    std::string v;

    if (status_code_helper::is_good(config.get_string(file_name_time_fmt_key, v)))
    {
        _file_name_time_fmt.swap(v);
    }
}

void Settings::try_load_file_name_sep(config::IConfig& config) noexcept
{
    std::string v;

    if (status_code_helper::is_good(config.get_string(file_name_sep_key, v)))
    {
        _file_name_sep.swap(v);
    }
}

void Settings::try_load_file_name_suff(config::IConfig& config) noexcept
{
    std::string v;

    if (status_code_helper::is_good(config.get_string(file_name_suff_key, v)))
    {
        _file_name_suff.swap(v);
    }
}

void Settings::try_load_add_console_log(config::IConfig& config) noexcept
{
    bool v;

    if (status_code_helper::is_good(config.get_bool(add_console_log_key, v)))
    {
        _add_console_log = v;
    }
}

void Settings::try_load_file_auto_flush(config::IConfig& config) noexcept
{
    bool v;

    if (status_code_helper::is_good(config.get_bool(file_auto_flush_key, v)))
    {
        _file_auto_flush = v;
    }
}

void Settings::try_load_file_collector_max_files(config::IConfig& config) noexcept
{
    std::int32_t v;

    if (status_code_helper::is_good(config.get_int(file_collector_max_files_key, v)))
    {
        _file_collector_max_files = v;
    }
}

void Settings::try_load_file_collector_clear_history(config::IConfig& config) noexcept
{
    bool v;

    if (status_code_helper::is_good(config.get_bool(file_collector_clear_history_key, v)))
    {
        _file_collector_clear_history = v;
    }
}

void Settings::try_load_message_source_short_path_separator(config::IConfig& config) noexcept
{
    std::string v;

    if (status_code_helper::is_good(config.get_string(message_source_short_path_separator_key, v)))
    {
        _message_source_short_path_separator.swap(v);
    }
}

void Settings::try_load_message_time_fmt(config::IConfig& config) noexcept
{
    std::string v;

    if (status_code_helper::is_good(config.get_string(message_time_stamp_fmt_key, v)))
    {
        _message_time_fmt.swap(v);
    }
}

std::string Settings::get_app_name() const noexcept
{
    try
    {
        return boost::dll::program_location().relative_path().stem().string();
    }
    catch (...)
    {
        return std::string{};
    }
}

std::string Settings::get_file_name() const noexcept
{
    try
    {
        std::stringstream ss;
        ss << get_app_name() << _file_name_ext;
        ss.flush();
        return ss.str();
    }
    catch (...)
    {
        return std::string{};
    }
}

std::string Settings::get_target_file_name() const noexcept
{
    try
    {
        std::stringstream ss;
        ss << get_app_name()
           << _file_name_sep
           << format_time(boost::posix_time::second_clock::local_time())
           << _file_name_sep
           << _file_name_suff;
        ss.flush();
        return ss.str();
    }
    catch (...)
    {
        return std::string{};
    }
}

} // namespace logger
