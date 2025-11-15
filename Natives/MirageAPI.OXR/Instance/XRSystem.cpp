#include "pch.h"
#include "XRSystem.h"

namespace MirageAPI::OpenXR
{
    XRSystem::XRSystem(const XRInstance& instance, FormFactor formFactor)
    {
        XrSystemGetInfo systemGetInfo = {
            XR_TYPE_SYSTEM_GET_INFO,
            nullptr,
            XrFormFactor(formFactor)
        };
        XrResult result = xrGetSystem(instance.GetNativeInstance(), &systemGetInfo, &_systemId);
        if (XR_FAILED(result))
        {
            throw std::exception("Failed to get XRSystemId", result);
        }
    }
}
