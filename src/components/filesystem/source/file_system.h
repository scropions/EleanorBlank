#pragma once
#include <interfaces/filesystem/file_system.h>

namespace os
{

class FileSystem final : public IFileSystem
{
public:
    static FileSystem& instance();

private:
    FileSystem() = default;
    FileSystem(const FileSystem&) = delete;
    FileSystem& operator=(const FileSystem&) = delete;

public:
    virtual StatusCode get_app_folder_path(std::string& path) noexcept override;
    virtual StatusCode get_data_folder_path(std::string& path) noexcept override;
};

using FileSystemSingleton = FileSystem;

} // namespace os
