#include "pch.h"
#include "XRInstance.h"

#include "XRInstanceConfig.h"
#include "XRSystem.h"


namespace MirageAPI::OpenXR
{
    XRInstance::XRInstance(const XRInstanceConfig& config)
    {
        if (IsInitialized()) return;

        // app info for creating instance
        XrApplicationInfo appInfo{};
        // names
        strncat_s(appInfo.applicationName, sizeof(appInfo.applicationName), config.ApplicationName.c_str(), XR_MAX_APPLICATION_NAME_SIZE - 1);
        strncat_s(appInfo.engineName, sizeof(appInfo.engineName), config.EngineName.c_str(),XR_MAX_ENGINE_NAME_SIZE - 1);
        // versions
        appInfo.applicationVersion = config.ApplicationVersion;
        appInfo.engineVersion = config.EngineVersion;
        appInfo.apiVersion = XR_CURRENT_API_VERSION;

        // layers
        for each (std::string layer in config.EnabledLayers)
        {
            _enabledLayers.push_back(layer.c_str());
        }
        // extensions
        for each (std::string extension in config.EnabledExtensions)
        {
            _enabledExtensions.push_back(extension.c_str());
            _isDebug = _isDebug || _enabledExtensions.back() == "XR_EXT_debug_utils";
        }

        // instance himself
        XrInstanceCreateInfo createInfo{
            XR_TYPE_INSTANCE_CREATE_INFO,
            nullptr,
            config.Flags,
            appInfo,
            static_cast<uint32_t>(_enabledLayers.size()),
            _enabledLayers.data(),
            static_cast<uint32_t>(_enabledExtensions.size()),
            _enabledExtensions.data()
        };
        XRExceptionChecker::Check(
            xrCreateInstance(&createInfo, &_instance),
            "Create XRInstance"
        );

        // saving to use in next times
        _xrInstance = this;
    }

    XRInstance::~XRInstance()
    {
        if (_xrInstance != XR_NULL_HANDLE)
        {
            XRExceptionChecker::Check(
                xrDestroyInstance(_instance),
                "Destroy XRInstance"
            );
            _instance = XR_NULL_HANDLE;
        }

        if (_system != nullptr)
        {
            delete _system;
            _system = nullptr;
        }

        _xrInstance = nullptr;
    }

    std::vector<std::string> XRInstance::GetAvailableExtensions()
    {
        // count
        uint32_t extensionCount = 0;
        XRExceptionChecker::Check(
            xrEnumerateInstanceExtensionProperties(nullptr, 0, &extensionCount, nullptr),
            "Enumerate Instance Extensions (for counting)"
        );

        // native data
        std::vector<XrExtensionProperties> extensions(extensionCount, {XR_TYPE_EXTENSION_PROPERTIES}); // NOLINT(clang-diagnostic-missing-field-initializers)

        XRExceptionChecker::Check(
            xrEnumerateInstanceExtensionProperties(nullptr, extensionCount, &extensionCount, extensions.data()),
            "Enumerate Instance Extensions (for properties)"
        );

        // converting to std::string
        std::vector<std::string> result = std::vector<std::string>(extensionCount);
        for (uint32_t i = 0; i < extensionCount; i++)
        {
            result[i] = std::string(extensions[i].extensionName);
        }
        return result;
    }

    std::vector<std::string> XRInstance::GetAvailableLayers()
    {
        // count
        uint32_t layerCount = 0;
        XRExceptionChecker::Check(
            xrEnumerateApiLayerProperties(0, &layerCount, nullptr),
            "Enumerate Instance Layers (for counting)"
        );

        // native data
        std::vector<XrApiLayerProperties> layers(layerCount, {XR_TYPE_API_LAYER_PROPERTIES}); // NOLINT(clang-diagnostic-missing-field-initializers)
        XRExceptionChecker::Check(
            xrEnumerateApiLayerProperties(layerCount, &layerCount, layers.data()),
            "Enumerate Instance Layers (for properties)"
        );

        // converting to std::string
        std::vector<std::string> result = std::vector<std::string>(layerCount);
        for (uint32_t i = 0; i < layerCount; i++)
        {
            result[i] = std::string(layers[i].layerName);
        }
        return result;
    }
}
