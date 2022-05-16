#pragma once
namespace UIEngine
{
class Logger: public ultralight::Logger
{
public:
    Logger();
    ~Logger();

    virtual void LogMessage(ultralight::LogLevel log_level, const ultralight::String& message) override;
private:


};
}