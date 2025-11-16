#pragma once

#include "XRSessionConfig.h"

namespace MirageAPI::Native::OpenXR
{
    class XRSession
    {
        XRInstance* _xrInstance = XRInstance::GetXRInstance();

        XrSession _session = XR_NULL_HANDLE;
        XRPrimaryViewConfigurationType _primaryViewConfigurationType;
        XREnvironmentBlendMode _environmentBlendMode;
        bool _running = false;
        
        void _waitFrame() const;

    public:
        // native field
        XrSession GetNativeSession() const { return _session; }
        // other
        XRInstance* GetXRInstance() const { return _xrInstance; }
        bool IsRunning() const { return _running; }
        
        XRSession(const XRSessionConfig& config);
        ~XRSession();
        
        void BeginSession();
        void EndSession();
        
        void BeginFrame() const;
        void EndFrame() const;
    };
}
