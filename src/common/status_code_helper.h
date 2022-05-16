#pragma once

#include <common/status_codes.h>

namespace status_code_helper
{
    inline bool is_bad(StatusCode sc)
    {
        return sc >= StatusCode::Bad;
    }

    inline bool is_good(StatusCode sc)
    {
        return sc < StatusCode::Bad;
    }
}
