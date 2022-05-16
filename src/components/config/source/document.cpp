#include "pch.h"
#include "document.h"

#include <components/filesystem/source/file_system.h>

namespace config
{

StatusCode Document::init(const std::string_view file_name, const std::string_view template_xml) noexcept
{
    try
    {
        _file_path = get_file_path(file_name);
        _template_xml = template_xml;
        return StatusCode::Good;
    }
    catch (...)
    {
        LOG_ERROR << "Document failed to init: " << utils::what_s() << ".";
        return StatusCode::Unexpected;
    }
}

StatusCode Document::save() const noexcept
{
    CHECK_SC_R(save_to(_file_path, _doc));
    CHECK_SC_R(read_modification_time(_file_path, _last_write_time));
    return StatusCode::Good;
}

StatusCode Document::rename_file(const std::string& fn) const noexcept
{
    try
    {
        boost::filesystem::rename(fn, fn + ".bak");
        return StatusCode::Good;
    }
    catch (...)
    {
        LOG_ERROR << "Document failed to rename file: " << utils::what_s() << ".";
        return StatusCode::Unexpected;
    }
}

StatusCode Document::remove_file(const std::string& fn) const noexcept
{
    boost::system::error_code ec;
    boost::filesystem::remove(fn, ec);
    CHECK_R2(!ec, StatusCode::Unexpected);
    return StatusCode::Good;
}

StatusCode Document::load() noexcept
{
    if (StatusCode::ParseFail == load_from(_file_path, _doc))
    {
        if (status_code_helper::is_bad(rename_file(_file_path)))
        {
            CHECK_SC_R(remove_file(_file_path));
        }

        CHECK_SC_R(validate_file());
        CHECK_SC_R(load_from(_file_path, _doc));
    }
    else
    {
        CHECK_SC_R(validate_document());
    }

    CHECK_SC_R(read_modification_time(_file_path, _last_write_time));
    return StatusCode::Good;
}

StatusCode Document::refresh() noexcept
{
    CHECK_SC_R(validate_file());

    std::time_t last_write_time = BadTimeType::value;
    CHECK_SC_R(read_modification_time(_file_path, last_write_time));
    CHECK_R2(BadTimeType::value != last_write_time, StatusCode::Unexpected);

    if (BadTimeType::value == _last_write_time || last_write_time > _last_write_time)
    {
        CHECK_SC_R(load());
    }

    return StatusCode::Good;
}

StatusCode Document::create_directories(const std::string& fn) const noexcept
{
    CHECK_R2(!fn.empty(), StatusCode::InvalidArgument);

    try
    {
        boost::filesystem::path path{ fn };
        path.remove_filename();

        if (!boost::filesystem::exists(path))
        {
            boost::filesystem::create_directories(path);
        }

        return StatusCode::Good;
    }
    catch (...)
    {
        LOG_ERROR << "Document failed to create directories: " << utils::what_s() << ".";
        return StatusCode::Unexpected;
    }
}

StatusCode Document::save_to(const std::string& fn, const boost::property_tree::ptree& doc) const noexcept
{
    CHECK_R2(!fn.empty(), StatusCode::NotInitialized);

    try
    {
        std::lock_guard<std::mutex> guard(_mutex);
        boost::property_tree::xml_writer_settings<std::string> settings(
            consts::writer_indent, consts::writer_indent_symbol_count::value);
        boost::property_tree::write_xml(fn, doc, std::locale(), settings);
        return StatusCode::Good;
    }
    catch (...)
    {
        LOG_ERROR << "Document failed to save config: " << utils::what_s() << ".";
        return StatusCode::Unexpected;
    }
}

StatusCode Document::load_from(const std::string& fn, boost::property_tree::ptree& doc) const noexcept
{
    doc.clear();
    CHECK_R2(!fn.empty(), StatusCode::NotInitialized);

    try
    {
        std::lock_guard<std::mutex> guard(_mutex);
        boost::property_tree::read_xml(fn, doc, boost::property_tree::xml_parser::trim_whitespace);
        return StatusCode::Good;
    }
    catch (...)
    {
        LOG_ERROR << "Document failed to load config: " << utils::what_s() << ".";
        return StatusCode::ParseFail;
    }
}

StatusCode Document::read_modification_time(const std::string& fn, std::time_t& tm) const noexcept
{
    tm = BadTimeType::value;
    CHECK_R2(!fn.empty(), StatusCode::InvalidArgument);

    try
    {
        boost::filesystem::path path(fn);
        CHECK_R2(boost::filesystem::exists(path), StatusCode::NotFound);
        tm = boost::filesystem::last_write_time(path);
        return StatusCode::Good;
    }
    catch (...)
    {
        LOG_ERROR << "Document failed to read modification time: " << utils::what_s() << ".";
        return StatusCode::Unexpected;
    }
}

StatusCode Document::validate_file() const noexcept
{
    try
    {
        if (boost::filesystem::exists(boost::filesystem::path(_file_path)))
        {
            return StatusCode::Good;
        }

        CHECK_SC_R(create_directories(_file_path));
        boost::property_tree::ptree template_document;
        CHECK_SC_R(create_template_document(template_document));
        CHECK_SC_R(save_to(_file_path, template_document));
        return StatusCode::Good;
    }
    catch (...)
    {
        LOG_ERROR << "Document failed to validate file: " << utils::what_s() << ".";
        return StatusCode::Unexpected;
    }
}

std::string Document::create_xml() const
{
    std::stringstream xml;
    xml << consts::default_config_preffix_xml << std::endl
        << logger::get_default_config_xml() << std::endl
        << _template_xml << std::endl
        << consts::default_config_suffix_xml;
    return xml.str();
}

StatusCode Document::validate_document() noexcept
{
    try
    {
        boost::property_tree::ptree template_document;
        CHECK_SC_R(create_template_document(template_document));
        CHECK_SC_R(validate_document(template_document));
        return StatusCode::Good;
    }
    catch (...)
    {
        LOG_ERROR << "Document failed to validate document: " << utils::what_s() << ".";
        return StatusCode::Bad;
    }
}

StatusCode Document::import_child_nodes(
    const boost::property_tree::ptree& template_parent,
    boost::property_tree::ptree& doc_parent,
    bool& dirty) const
{
    std::for_each(template_parent.begin(), template_parent.end(), [this, &doc_parent, &dirty](const auto& template_node)
    {
        auto doc_child = doc_parent.get_child_optional(template_node.first);

        if (doc_child.has_value())
        {
            CHECK_SC_R0(import_child_nodes(template_node.second, doc_child.value(), dirty));
        }
        else
        {
            dirty = true;
            doc_parent.add_child(template_node.first, template_node.second);
        }
    });

    return StatusCode::Good;
}

StatusCode Document::create_template_document(boost::property_tree::ptree& doc) const noexcept
{
    try
    {
        std::istringstream stream(create_xml());
        CHECK_R2(stream.good(), StatusCode::NotFound);
        boost::property_tree::read_xml(stream, doc, boost::property_tree::xml_parser::trim_whitespace);
        return StatusCode::Good;
    }
    catch (...)
    {
        LOG_ERROR << "Document failed to create template document: " << utils::what_s() << ".";
        return StatusCode::Bad;
    }
}

StatusCode Document::validate_document(const boost::property_tree::ptree& template_document) noexcept
{
    try
    {
        bool dirty = false;
        CHECK_SC_R(Document::import_child_nodes(template_document, _doc, dirty));

        if (dirty)
        {
            CHECK_SC_R(save());
        }

        return StatusCode::Good;
    }
    catch (...)
    {
        LOG_ERROR << "Document validation failed: " << utils::what_s() << ".";
        return StatusCode::Bad;
    }
}

std::string Document::get_file_path(const std::string_view file_name) const
{
    std::string data_folder_path;
    CHECK_SC(os::FileSystemSingleton::instance().get_data_folder_path(data_folder_path));
    return boost::filesystem::path{ data_folder_path }
           .append(common::consts::product_name.data())
           .append(std::string(file_name)).string();
}

} // namespace config