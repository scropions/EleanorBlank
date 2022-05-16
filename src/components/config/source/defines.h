#pragma once

namespace config
{

namespace consts
{
using LockTimeoutType = std::integral_constant<std::uint32_t, 1>; // 1 second
using writer_indent_symbol_count = std::integral_constant<std::size_t, 4>;

constexpr static char xpath_separator = '/';
constexpr static char writer_indent = ' ';

constexpr std::string_view default_xpath_preffix_xml = "config";
constexpr std::string_view default_config_preffix_xml = R"***(<config>)***";
constexpr std::string_view default_config_suffix_xml = R"***(</config>)***";

} // namespace consts

} // namespace config