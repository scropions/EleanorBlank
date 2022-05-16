#pragma once

#include <unordered_map>
#include <common/status_code_helper.h>
#include <common/exception.h>
#include <interfaces/logger/ilogger.h>

constexpr static std::string_view default_bad_sc_name = "UnknownDescription. Add status code description into status_code_macro.h.";

#define STATUS_CODE_NAMES_BEGIN() static std::unordered_map<StatusCode, std::string_view> statuscode_names = {
#define STATUS_CODE_NAMES_VALUE(sc) {StatusCode::sc, #sc},
#define STATUS_CODE_NAMES_END() }; inline std::string_view get_statuscode_name(StatusCode sc) { \
    const auto it = statuscode_names.find(sc); \
    return (it != statuscode_names.end()) ? it->second : default_bad_sc_name; }

STATUS_CODE_NAMES_BEGIN()
    STATUS_CODE_NAMES_VALUE(Good)
    STATUS_CODE_NAMES_VALUE(GoodNotInitialized)
    STATUS_CODE_NAMES_VALUE(GoodAlreadyInitialized)
    STATUS_CODE_NAMES_VALUE(GoodAlreadyExists)
    STATUS_CODE_NAMES_VALUE(GoodNothingTodo)
    STATUS_CODE_NAMES_VALUE(Bad)
    STATUS_CODE_NAMES_VALUE(InvalidArgument)
    STATUS_CODE_NAMES_VALUE(BadPointer)
    STATUS_CODE_NAMES_VALUE(BadAlreadyExists)
    STATUS_CODE_NAMES_VALUE(OutOfMemory)
    STATUS_CODE_NAMES_VALUE(NotInitialized)
    STATUS_CODE_NAMES_VALUE(BadCommand)
    STATUS_CODE_NAMES_VALUE(NotImplemented)
    STATUS_CODE_NAMES_VALUE(NotFound)
    STATUS_CODE_NAMES_VALUE(NotValid)
    STATUS_CODE_NAMES_VALUE(NotSupported)
    STATUS_CODE_NAMES_VALUE(BadState)
    STATUS_CODE_NAMES_VALUE(BadSize)
    STATUS_CODE_NAMES_VALUE(IpcFail)
    STATUS_CODE_NAMES_VALUE(SessionLimitExceeded)
    STATUS_CODE_NAMES_VALUE(ParseFail)
    STATUS_CODE_NAMES_VALUE(FileOpenFailed)
    STATUS_CODE_NAMES_VALUE(FileFlushFailed)
    STATUS_CODE_NAMES_VALUE(FileWriteFailed)
    STATUS_CODE_NAMES_VALUE(AccessDenied)
    STATUS_CODE_NAMES_VALUE(ServiceStartFailed)
    STATUS_CODE_NAMES_VALUE(Unexpected)
STATUS_CODE_NAMES_END()

#define CHECK_SC(exp) { StatusCode sc; if (status_code_helper::is_bad(sc = (exp))) { LOG_ERROR << get_statuscode_name(sc).data(); }}
#define CHECK_SC_R(exp) { StatusCode sc; if (status_code_helper::is_bad(sc = (exp))) { LOG_ERROR << get_statuscode_name(sc).data(); return sc; } }
#define CHECK_SC_R0(exp) { StatusCode sc; if (status_code_helper::is_bad(sc = (exp))) { LOG_ERROR << get_statuscode_name(sc).data(); return; }}
#define CHECK_SC_R2(exp, ret) { StatusCode sc; if (status_code_helper::is_bad(sc = (exp))) { LOG_ERROR << get_statuscode_name(sc).data(); return (ret); }}
#define CHECK_R2(exp, ret) { if (!(exp)) { LOG_ERROR << "!(exp): " << #exp; return (ret); }}
#define CHECK_R0(exp) { if (!(exp)) { LOG_ERROR << "!(exp): " << #exp; return; }}
#define CHECK_EXP(exp) { if (!(exp)) { LOG_ERROR << "!(exp): " << #exp; }}
#define CHECK_POINTER_R(exp) { if (nullptr == (exp)) { LOG_ERROR << "nullptr == " << #exp; return StatusCode::BadPointer; }}
#define CHECK_POINTER_R0(exp) { if (nullptr == (exp)) { LOG_ERROR << "nullptr == " << #exp; return; }}
#define CHECK_POINTER_R2(exp, ret) { if (nullptr == (exp)) { LOG_ERROR << "nullptr == " << #exp; return (ret); }}

#define CHECK_R_THROW(exp) { if (!(exp)) { LOG_ERROR << "!(exp): " << #exp; throw exception(StatusCode::Bad); }}
#define CHECK_R2_THROW(exp, ret) { if (!(exp)) { LOG_ERROR << "!(exp): " << #exp; throw exception((ret)); }}
#define CHECK_SC_THROW(exp) { StatusCode sc; if (status_code_helper::is_bad(sc = (exp))) { LOG_ERROR << get_statuscode_name(sc).data(); throw exception(sc); } }
#define CHECK_SC_THROW2(exp, text) { StatusCode sc; if (status_code_helper::is_bad(sc = (exp))) { \
    LOG_ERROR << get_statuscode_name(sc).data() << ": " << text; throw exception(sc, text); } }
#define CHECK_SC_THROW_SILENT(exp) { StatusCode sc; if (status_code_helper::is_bad(sc = (exp))) { throw exception(sc); } }
#define CHECK_POINTER_THROW(exp) { if (nullptr == (exp)) { LOG_ERROR << "nullptr == " << #exp; throw exception(StatusCode::BadPointer); }}
