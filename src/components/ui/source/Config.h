#pragma once

namespace UIEngine
{

class Config
{
public:
    Config() = default;

    void SetMSAAEnabled(bool is_enabled) noexcept;
    bool GetMSAAEnabled() const;
private:
    bool _msaa_enabled = false;
};
}