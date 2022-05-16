#pragma once

#define VERSION                     1,0,0,0
#define VERSION_STR                 "1.0.0.0"

enum class Version: uint32_t
{
    Undefined = 0,
    _1_0,
    Count
};

const std::map<Version, std::string> VersionMap = 
{
    { Version::Undefined, std::string("Undefined") },
    { Version::_1_0,  std::string("1.0.0")  }
};

#define CURRENT_VERSION             Version::_1_0
