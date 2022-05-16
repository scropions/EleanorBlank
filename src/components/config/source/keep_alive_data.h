#pragma once

#include <cstdint>

namespace config
{

class KeepAliveData
{
public:
    KeepAliveData() = default;
    KeepAliveData(const KeepAliveData& src) = default;
    KeepAliveData(KeepAliveData&& src) = default;
    KeepAliveData(bool enabled, std::uint32_t timeout, std::uint32_t cnt, std::uint32_t intverval);
    KeepAliveData& operator=(KeepAliveData&& src) = default;
    bool get_enabled() const noexcept;
    std::uint32_t get_timeout() const noexcept;
    std::uint32_t get_cnt() const noexcept;
    std::uint32_t get_intverval() const noexcept;
    void clear() noexcept;

private:
    bool _enabled = false;
    std::uint32_t _timeout = 0;
    std::uint32_t _cnt = 0;
    std::uint32_t _intverval = 0;
};

} // namespace config
