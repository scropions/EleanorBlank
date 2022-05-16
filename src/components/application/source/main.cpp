#include <Ultralight/Ultralight.h>
#include <AppCore/Platform.h>
#include <AppCore/JSHelpers.h>
#include "components/ui/source/Window.h"
#define _WIN32_WINNT 0x0603
#include <Windows.h>

int main(int argc, char** argv)
{

    UIEngine::UIWindow engine(1000, 1000, false, false);
    PROCESS_MITIGATION_DYNAMIC_CODE_POLICY dcp = {};
    dcp.ProhibitDynamicCode = 1;
    SetProcessMitigationPolicy(ProcessDynamicCodePolicy, &dcp, sizeof(dcp));
    engine.run();
    return 0;
}
