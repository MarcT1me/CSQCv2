#include "pch.h"
#include "XRSession.h"

#include "../Instance/XRSystem.h"

namespace MirageAPI::Native::OpenXR
{
    XRSession::XRSession(const XRSessionConfig& config)
        : _primaryViewConfigurationType(config.primaryViewConfigurationType), _environmentBlendMode(config.environmentBlendMode)
    {
        XrSessionCreateInfo createInfo{
            XR_TYPE_SESSION_CREATE_INFO,
            config.requirements->GetBindings(),
            config.flags,
            _xrInstance->GetSystem()->GetNativeSystemId()
        };
        XRResultChecker::Check(
            xrCreateSession(_xrInstance->GetNativeInstance(), &createInfo, &_session),
            "Create XRSession"
        );
    }

    XRSession::~XRSession()
    {
        if (_running)
        {
            EndSession();
        }

        if (_session != XR_NULL_HANDLE)
        {
            XRResultChecker::Check(
                xrDestroySession(_session),
                "Destroy XRSession"
            );
            _session = XR_NULL_HANDLE;
        }
    }

    void XRSession::BeginSession()
    {
        if (_running) return;

        XrSessionBeginInfo beginInfo{
            XR_TYPE_SESSION_BEGIN_INFO,
            nullptr,
            XrViewConfigurationType(_primaryViewConfigurationType)
        };
        XRResultChecker::Check(
            xrBeginSession(_session, &beginInfo),
            "begin XRSession"
        );

        _running = true;
    }

    void XRSession::EndSession()
    {
        if (!_running) return;

        XRResultChecker::Check(
            xrEndSession(_session),
            "end XRSession"
        );

        _running = false;
    }

    void XRSession::_waitFrame() const
    {
        XrFrameWaitInfo waitInfo{XR_TYPE_FRAME_WAIT_INFO};
        XrFrameState frameState{XR_TYPE_FRAME_STATE};
        XRResultChecker::Check(
            xrWaitFrame(_session, &waitInfo, &frameState),
            "XRSession wait frame"
        );
    }

    void XRSession::BeginFrame() const
    {
        if (!_running) return;

        _waitFrame();

        XrFrameBeginInfo frameBeginInfo{XR_TYPE_FRAME_BEGIN_INFO};
        XRResultChecker::Check(
            xrBeginFrame(_session, &frameBeginInfo),
            "XRSession begin frame"
        );
    }

    void XRSession::EndFrame() const
    {
        if (!_running) return;

        XrFrameEndInfo endInfo{
            XR_TYPE_FRAME_END_INFO,
            nullptr,
            0,
            XrEnvironmentBlendMode(_environmentBlendMode),
            0,
            nullptr
        };
        XRResultChecker::Check(
            xrEndFrame(_session, &endInfo),
            "XRSession end frame"
        );
    }
}
