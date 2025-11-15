#pragma once

#include <string>
#include <vector>

namespace MirageAPI::OpenXR
{
    class XRInstanceConfig;
    class XRSystem;

    class XRInstance
    {
        static XRInstance* _xrInstance;
        static bool _isDebug;

        XrInstance _instance;
        XRSystem* _system;
        
        std::vector<const char*> _enabledExtensions;
        std::vector<const char*> _enabledLayers;

    public:
        // native field
        XrInstance GetNativeInstance() const { return _instance; }
        // other
        XRSystem* GetSystem() const { return _system; }

        // static
        static XRInstance* GetXRInstance() { return _xrInstance; }
        static bool IsDebug() { return _isDebug; }
        static bool IsInitialized() { return _xrInstance != nullptr; }

        XRInstance(const XRInstanceConfig& config);
        ~XRInstance();

        static std::vector<std::string> GetAvailableExtensions();
        static std::vector<std::string> GetAvailableLayers();
    };
}
