#pragma once

namespace UIEngine
{
class View;
class Loader: public ultralight::LoadListener
{
public:
    typedef std::function<void(ultralight::View*)> LoadedCallback;
    typedef LoadedCallback* LoadedCallbackPtr;

    Loader() = default;

    virtual void OnFailLoading(ultralight::View* caller,
                               uint64_t frame_id,
                               bool is_main_frame,
                               const ultralight::String& url,
                               const ultralight::String& description,
                               const ultralight::String& error_domain,
                               int error_code) override;

    virtual void OnFinishLoading(ultralight::View* caller,
                                 uint64_t frame_id,
                                 bool is_main_frame,
                                 const ultralight::String& url) override;

    virtual void OnDOMReady(ultralight::View* caller,
                            uint64_t frame_id,
                            bool is_main_frame,
                            const ultralight::String& url) override;

    void SubscribeLoader(const LoadedCallback on_loaded);

private:
    std::vector<LoadedCallback> _callbacks;

};
}