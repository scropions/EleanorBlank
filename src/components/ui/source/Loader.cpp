#include "pch.h"
#include "Loader.h"

namespace UIEngine
{

void Loader::OnFailLoading(ultralight::View* caller,
                           uint64_t frame_id,
                           bool is_main_frame,
                           const ultralight::String& url,
                           const ultralight::String& description,
                           const ultralight::String& error_domain,
                           int error_code)
{

}

void Loader::OnFinishLoading(ultralight::View* caller,
                             uint64_t frame_id,
                             bool is_main_frame,
                             const ultralight::String& url)
{

}

void Loader::OnDOMReady(ultralight::View* caller,
                        uint64_t frame_id,
                        bool is_main_frame,
                        const ultralight::String& url)
{
    ultralight::RefPtr<ultralight::JSContext> context = caller->LockJSContext();
    ultralight::SetJSContext(context->ctx());

    ultralight::JSObject global = ultralight::JSGlobalObject();
}

}