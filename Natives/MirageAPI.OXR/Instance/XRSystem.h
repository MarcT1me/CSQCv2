#pragma once
#include "XRInstance.h"

#include "XRInstanceConfig.h"

namespace MirageAPI::OpenXR
{
    class XRSystem
    {
        XrSystemId _systemId;

    public:
        XRSystem(const XRInstance& instance, FormFactor formFactor);

        XrSystemId GetNativeSystemId() const { return _systemId; };
    };
}
