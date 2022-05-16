#pragma once

namespace logger
{
class Settings
{
    // keys
    static constexpr std::string_view file_target_key = "logger/target";
    static constexpr std::string_view file_rotation_size_key = "logger/rotation_size";

    static constexpr std::string_view file_name_time_fmt_key = "logger/file_name_time_fmt";
    static constexpr std::string_view file_name_sep_key = "logger/file_name_separator";
    static constexpr std::string_view file_name_suff_key = "logger/file_name_suffix";
    static constexpr std::string_view file_auto_flush_key = "logger/auto_flush";
    static constexpr std::string_view add_console_log_key = "logger/add_console_log";

    static constexpr std::string_view file_collector_max_files_key = "logger/file_collector/max_files";
    static constexpr std::string_view file_collector_clear_history_key = "logger/file_collector/clear_history";

    static constexpr std::string_view message_source_short_path_separator_key =
        "logger/message/source_short_path_separator";
    static constexpr std::string_view message_time_stamp_fmt_key = "logger/message/time_stamp_format";

    static constexpr std::string_view severity_level_key = "logger/severity_level";

    // defaults
    static constexpr std::string_view file_target_default = "logs";
    static constexpr std::int32_t file_rotation_size_default = 10 * 1024 * 1024;

    static constexpr std::string_view file_name_time_fmt_default = "%Y%m%d_%H%M%S";
    static constexpr std::string_view file_name_sep_default = "_";
    static constexpr std::string_view file_name_suff_default = "%3N";
    static constexpr std::string_view file_name_ext_default = ".log";
    static constexpr bool file_auto_flush_default = false;
    static constexpr bool add_console_log_default = false;

    static constexpr std::int32_t file_collector_max_files_default = 2;
    static constexpr bool file_collector_clear_history_default = true;

    static constexpr std::string_view message_source_short_path_separator_default = "source";
    static constexpr std::string_view message_time_fmt_default = "%Y-%m-%d %H:%M:%S.%f";

    static constexpr boost::log::trivial::severity_level severity_level_default = boost::log::trivial::severity_level::info;

public:
    static Settings& instance();

private:
    Settings() = default;
    ~Settings() = default;
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

public:
    static std::string get_path_short_name(std::string_view) noexcept;
    static std::string get_path_file_name(std::string_view) noexcept;

public:
    void try_load_from_config(config::IConfig& config);

    void set_severity_level(boost::log::trivial::severity_level) noexcept;
    boost::log::trivial::severity_level get_severity_level() const noexcept;

    const std::string& get_file_target() const noexcept;
    std::int32_t get_file_rotation_size() const noexcept;

    const std::string& get_file_name_time_fmt() const noexcept;
    const std::string& get_file_name_sep() const noexcept;
    const std::string& get_file_name_suff() const noexcept;
    bool get_file_auto_flush() const noexcept;
    bool get_add_console_log() const noexcept;

    std::int32_t get_file_collector_max_files() const noexcept;
    bool get_file_collector_clear_history() const noexcept;

    const std::string& get_message_source_short_path_separator() const noexcept;
    const std::string& get_message_time_fmt() const noexcept;

    std::string get_file_name() const noexcept;
    std::string get_target_file_name() const noexcept;

private:
    void try_load_severity_level(config::IConfig& config) noexcept;
    void try_load_file_target(config::IConfig& config) noexcept;
    void try_load_file_rotation_size(config::IConfig& config) noexcept;
    void try_load_file_name_time_fmt(config::IConfig& config) noexcept;
    void try_load_file_name_sep(config::IConfig& config) noexcept;
    void try_load_file_name_suff(config::IConfig& config) noexcept;
    void try_load_add_console_log(config::IConfig& config) noexcept;
    void try_load_file_auto_flush(config::IConfig& config) noexcept;
    void try_load_file_collector_max_files(config::IConfig& config) noexcept;
    void try_load_file_collector_clear_history(config::IConfig& config) noexcept;
    void try_load_message_source_short_path_separator(config::IConfig& config)  noexcept;
    void try_load_message_time_fmt(config::IConfig& config) noexcept;

private:
    std::string get_app_name() const noexcept;
    std::string format_time(const boost::posix_time::ptime& t) const noexcept;

private:
    std::atomic<boost::log::trivial::severity_level> _severity_level = severity_level_default;
    std::string _file_target{ file_target_default };
    std::int32_t _file_rotation_size{ file_rotation_size_default };

    std::string _file_name_time_fmt{ file_name_time_fmt_default };
    std::string _file_name_sep{ file_name_sep_default };
    std::string _file_name_suff{ file_name_suff_default };
    std::string _file_name_ext{ file_name_ext_default };
    bool _file_auto_flush{ file_auto_flush_default };
    bool _add_console_log{ add_console_log_default };

    std::int32_t _file_collector_max_files{ file_collector_max_files_default };
    bool _file_collector_clear_history{ file_collector_clear_history_default };

    std::string _message_source_short_path_separator{ message_source_short_path_separator_default };
    std::string _message_time_fmt{ message_time_fmt_default };
};

inline Settings& get_settings() noexcept
{
    return Settings::instance();
}

} // namespace logger
