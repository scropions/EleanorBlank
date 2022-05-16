#pragma once

namespace config
{

class Document
{
    using PropertyName = std::string;

    using BadTimeType = std::integral_constant<std::time_t, -1>;
    using PathDelimiterType = std::integral_constant<std::string_view::value_type, '.'>;

    const std::string_view new_line = "\n";

public:
    StatusCode init(const std::string_view file_name, const std::string_view template_xml) noexcept;
    StatusCode save() const noexcept;
    StatusCode load() noexcept;
    StatusCode refresh() noexcept;

    template<typename Type>
    Type get_value(const std::string& path) const
    {
        auto full_path = std::string{ consts::default_xpath_preffix_xml } + consts::xpath_separator + path;
        return _doc.get<Type>(boost::property_tree::ptree::path_type{ full_path, consts::xpath_separator });
    }

    template<typename Type>
    void set_value(const std::string& path, Type value)
    {
        auto full_path = std::string{ consts::default_xpath_preffix_xml } + consts::xpath_separator + path;
        _doc.put<Type>(boost::property_tree::ptree::path_type { full_path, consts::xpath_separator }, value);
    }

private:
    StatusCode create_directories(const std::string& fn) const noexcept;
    StatusCode save_to(const std::string& fn, const boost::property_tree::ptree& doc) const noexcept;
    StatusCode load_from(const std::string& fn, boost::property_tree::ptree& doc) const noexcept;
    StatusCode read_modification_time(const std::string& fn, std::time_t& tm) const noexcept;
    StatusCode validate_file() const noexcept;
    StatusCode validate_document() noexcept;
    StatusCode validate_document(const boost::property_tree::ptree& template_document) noexcept;
    StatusCode rename_file(const std::string& fn) const noexcept;
    StatusCode remove_file(const std::string& fn) const noexcept;
    std::string get_file_path(const std::string_view file_name) const;
    std::string create_xml() const;
    StatusCode import_child_nodes(
        const boost::property_tree::ptree& template_parent,
        boost::property_tree::ptree& doc_parent,
        bool& dirty) const;
    StatusCode create_template_document(boost::property_tree::ptree& doc) const noexcept;

private:
    mutable std::mutex _mutex;
    std::string _file_path;
    std::string _template_xml;
    mutable std::time_t _last_write_time = BadTimeType::value;
    boost::property_tree::ptree _doc;
};

} // namespace config