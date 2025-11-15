#pragma once

#include "../platform.h"

namespace MirageAPI::OpenXR
{
    enum class XRPrimaryViewConfigurationType
    {
        Mono = 1,
        Stereo = 2,
        Observer = 1000054000,
    };
    
    enum class XREnvironmentBlendMode
    {
        Opaque = 1,
        Additive = 2,
        AlphaBlend = 3
    };
    
    struct XRSessionConfig
    {
        uint64_t flags;
        Platform::SessionRequirements* requirements;
        XRPrimaryViewConfigurationType primaryViewConfigurationType;
        XREnvironmentBlendMode environmentBlendMode;
    };
}
