// keep_alive_data.cpp
#include "pch.h"
#include "keep_alive_data.h"

namespace config
{

KeepAliveData::KeepAliveData(
    bool enabled,
    std::uint32_t timeout,
    std::uint32_t cnt,
    std::uint32_t intverval) :
    _enabled{ enabled },
    _timeout{ timeout },
    _cnt{ cnt },
    _intverval{ intverval }
{
}

bool KeepAliveData::get_enabled() const noexcept
{
    return _enabled;
}

std::uint32_t KeepAliveData::get_timeout() const noexcept
{
    return _timeout;
}

std::uint32_t KeepAliveData::get_cnt() const noexcept
{
    return _cnt;
}

std::uint32_t KeepAliveData::get_intverval() const noexcept
{
    return _intverval;
}

void KeepAliveData::clear() noexcept
{
    _enabled = false;
    _timeout = 0;
    _cnt = 0;
    _intverval = 0;
}

} // namespace config
