#pragma once
#include <cstdint>

enum class [[nodiscard]] StatusCode : std::uint32_t
{
    Good = 0U,
    GoodNotInitialized,
    GoodAlreadyInitialized,
    GoodAlreadyExists,
    GoodNothingTodo,
    Bad = 0x80000000U,
    InvalidArgument,
    BadPointer,
    BadAlreadyExists,
    OutOfMemory,
    NotInitialized,
    BadCommand,
    NotImplemented,
    NotFound,
    NotValid,
    NotSupported,
    BadState,
    BadSize,
    IpcFail,
    SessionLimitExceeded,
    ParseFail,
    FileOpenFailed,
    FileFlushFailed,
    FileWriteFailed,
    AccessDenied,
    ServiceStartFailed,
    Unexpected 
};
