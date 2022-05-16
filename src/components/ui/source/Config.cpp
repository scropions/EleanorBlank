#include "pch.h"
#include "Config.h"

namespace UIEngine
{

void Config::SetMSAAEnabled(bool is_enabled) noexcept
{
    _msaa_enabled = is_enabled;
}

bool Config::GetMSAAEnabled() const
{
    return _msaa_enabled;
}

}