#pragma once

namespace os
{

class IFileSystem
{
public:
    virtual ~IFileSystem() = default;

    virtual StatusCode get_app_folder_path(std::string& path) noexcept = 0;
    virtual StatusCode get_data_folder_path(std::string& path) noexcept = 0;
};

} // namespace os
