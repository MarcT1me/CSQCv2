#include "pch.h"
#include "XRInstanceConfig.h"

namespace MirageAPI::OpenXR
{
    XRInstanceConfig::XRInstanceConfig(const std::string& appName, int appVersion, OpenXR::FormFactor formFactor)
    {
        ApplicationName = appName;
        ApplicationVersion = appVersion;

        EngineName = "QuantumCore MirageAPI.OXR";
        EngineVersion = 2;

        FormFactor = formFactor;
        Flags = 0;
    }
}
