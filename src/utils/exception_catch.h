#pragma once

#include <exception>
#include <string>
#include <string_view>

namespace utils
{

inline std::string what(const std::exception_ptr& ptr = std::current_exception())
{
    if (!ptr) 
    {
        return std::bad_exception().what();
    }

    try 
    {
        std::rethrow_exception(ptr);
    }
    catch (const std::exception& e)
    {
        return e.what();
    }
    catch (const std::string& e)
    {
        return e;
    }
    catch (const char* e)
    {
        return e;
    }
    catch (...)
    {
        return "Unknown exception.";
    }
}

inline std::string what_s(const std::exception_ptr& ptr = std::current_exception()) noexcept
{
    try
    {
        return what(ptr);
    }
    catch (...)
    {
        return std::string{};
    }
}

}
