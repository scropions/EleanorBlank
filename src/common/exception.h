#pragma once

#include <exception>

#include <common/status_codes.h>

class exception : public std::exception
{
public:
    exception(StatusCode status_code, const char* message = "exception") :
        _msg(message), _status_code(status_code)
    {
    }
    virtual ~exception() override 
    {
    }
    virtual const char* what() const noexcept override
    {
        return _msg;
    }
    StatusCode get_staus_code() const noexcept
    {
        return _status_code;
    }
protected:
    const char* _msg;
    StatusCode _status_code;
};
