#include "pch.h"
#include "file_system.h"

#include <shlobj.h>
#include <winerror.h>
#include <comutil.h>
#include <atlcomcli.h>
#pragma comment(lib, "comsuppw")

StatusCode get_folder_path(REFKNOWNFOLDERID rfid, std::string& path) noexcept
{
    path.clear();

    PWSTR szPath = NULL;

    if (FAILED(::SHGetKnownFolderPath(rfid, 0, NULL, &szPath)))
    {
        return StatusCode::Unexpected;
    }

    try
    {
        path = _bstr_t(szPath);
        ::CoTaskMemFree(szPath);
        return StatusCode::Good;
    }
    catch (const std::bad_alloc&)
    {
        ::CoTaskMemFree(szPath);
        return StatusCode::OutOfMemory;
    }
}

os::FileSystem& os::FileSystem::instance()
{
    static FileSystem inst;
    return inst;
}

StatusCode os::FileSystem::get_app_folder_path(std::string& path) noexcept
{
    return get_folder_path(FOLDERID_LocalAppData, path);
}

StatusCode os::FileSystem::get_data_folder_path(std::string& path) noexcept
{
    return get_folder_path(FOLDERID_ProgramData, path);
}