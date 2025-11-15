#pragma once

#include <string>
#include <vector>

namespace MirageAPI::OpenXR
{
    enum FormFactor
    {
        HMD = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY,
        HANDHELD = XR_FORM_FACTOR_HANDHELD_DISPLAY,
    };

    class XRInstanceConfig
    {
    public:
        std::string ApplicationName;
        int ApplicationVersion;
        std::string EngineName;
        int EngineVersion;

        std::vector<std::string> EnabledExtensions;
        std::vector<std::string> EnabledLayers;
        FormFactor FormFactor;
        
        uint64_t Flags;

        XRInstanceConfig(const std::string& appName, int appVersion, OpenXR::FormFactor formFactor);
    };
}
