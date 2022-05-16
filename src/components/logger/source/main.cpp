#include "pch.h"
#include "settings.h"
#include <components/filesystem/source/file_system.h>

using namespace logger;

static const std::string_view default_config_xml =
    R"***(
<logger>
    <severity_level type = "int">2</severity_level>
    <target type = "string">logs</target>
    <rotation_size type = "int">10485760</rotation_size>
    <file_name_time_fmt type = "string">%Y%m%d_%H%M%S</file_name_time_fmt>
    <file_name_separator type = "string">_</file_name_separator>
    <file_name_suffix type = "string">%3N.log</file_name_suffix>
    <auto_flush type = "bool">0</auto_flush>
    <add_console_log type = "bool">0</add_console_log>
    <file_collector>
        <max_files type = "int">2</max_files>
        <clear_history type = "bool">1</clear_history>
    </file_collector>
    <message>
        <source_short_path_separator type = "string">Application</source_short_path_separator>
        <time_stamp_format type = "string">%Y-%m-%d %H:%M:%S.%f</time_stamp_format>
    </message>
</logger>
)***";


static std::string get_file_name()
{
    std::string data_folder_path;
    CHECK_SC(os::FileSystemSingleton::instance().get_data_folder_path(data_folder_path));

    return boost::filesystem::path{ data_folder_path }
            .append(common::consts::product_name.data())
            .append(get_settings().get_file_target())
            .append(get_settings().get_file_name()).string();
}

static std::string get_file_collector_target()
{
    std::string data_folder_path;
    CHECK_SC(os::FileSystemSingleton::instance().get_data_folder_path(data_folder_path));
    return boost::filesystem::path{ data_folder_path }
            .append(common::consts::product_name.data()).append(get_settings().get_file_target()).string();
}

static auto get_file_formatter()
{
    using namespace boost::log;
    return expressions::stream
            << "[" << expressions::format_date_time<boost::posix_time::ptime>(
            timestamp_key.data(),
            get_settings().get_message_time_fmt()) << "]"
            << "[" << expressions::attr<attributes::current_thread_id::value_type>
                    (thread_id_key.data()) << "]"
            << "[" << trivial::severity << "] "
            << expressions::smessage
            << " (" << expressions::attr<std::string>(file_key.data()) << ":"
            << expressions::attr<logger::LineType>(line_key.data()) << ":"
            << expressions::attr<std::string>(function_key.data())
            << ")";
}

static auto get_console_formatter()
{
    using namespace boost::log;
    return expressions::stream
            << "[" << trivial::severity << "] "
            << "[" << expressions::attr<std::string>(file_key.data()) << ":"
            << expressions::attr<std::string>(function_key.data()) << " - "
            << expressions::attr<logger::LineType>(line_key.data())
            << "] "
            << expressions::smessage;
}

static void add_console_logger()
{
    auto console_sink = boost::log::add_console_log
            (
                    std::clog,
                    boost::log::keywords::format = get_console_formatter()
            );

    if (console_sink && console_sink->locked_backend())
    {
        if (get_settings().get_file_auto_flush())
        {
            console_sink->locked_backend()->auto_flush(true);
        }
    }
}

static void add_file_logger()
{
    using namespace boost::log;
    auto sink = add_file_log
            (
                    keywords::file_name = get_file_name(),
                    keywords::target_file_name = get_settings().get_target_file_name(),
                    keywords::rotation_size = get_settings().get_file_rotation_size(),
                    keywords::format = get_file_formatter()
            );

    if (sink && sink->locked_backend())
    {
        sink->locked_backend()->set_file_collector(
                sinks::file::make_collector(
                        keywords::target = get_file_collector_target(),
                        keywords::max_files = get_settings().get_file_collector_max_files()));

        sink->locked_backend()->scan_for_files(sinks::file::scan_all);

        if (get_settings().get_file_auto_flush())
        {
            sink->locked_backend()->auto_flush(true);
        }
    }
}

void logger::set_severity_level(boost::log::trivial::severity_level lvl) noexcept
{
    get_settings().set_severity_level(lvl);
}

boost::log::trivial::severity_level logger::get_severity_level() noexcept
{
    return get_settings().get_severity_level();
}

StatusCode logger::init(config::IConfig& config) noexcept
{
    try
    {
        get_settings().try_load_from_config(config);

        boost::log::core::get()->set_exception_handler(boost::log::make_exception_suppressor());

        boost::log::add_common_attributes();

        auto test = get_file_name();
        auto target_file_name = get_settings().get_target_file_name();
        auto target = get_file_collector_target();

        add_console_logger();
        add_file_logger();

        boost::log::core::get()->set_filter
                (
                        [](boost::log::attribute_value_set const& values)
                        {
                            return values[boost::log::trivial::severity] >= get_settings().get_severity_level();
                        }
                );
        return StatusCode::Good;
    }
    catch (...)
    {
        LOG_ERROR << "Logger failed to init: ";
        return StatusCode::Unexpected;
    }
}

StatusCode logger::default_setup() noexcept
{
    try
    {
        boost::log::add_common_attributes();
        auto console_sink = boost::log::add_console_log
                (
                        std::cout,
                        boost::log::keywords::format = get_console_formatter()
                );

        if (console_sink && console_sink->locked_backend())
        {
            if (get_settings().get_file_auto_flush())
            {
                console_sink->locked_backend()->auto_flush(true);
            }
        }

        return StatusCode::Good;
    }
    catch (...)
    {
        LOG_ERROR << "Logger failed to init: ";
        return StatusCode::Unexpected;
    }
}
