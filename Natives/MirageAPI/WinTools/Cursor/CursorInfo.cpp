#include "pch.h"
#include "CursorInfo.h"

#include "../../NativeInstance.h"

namespace MirageAPI
{
    CursorInfo::CursorInfo(
        NativeInstance^ instance,
        int resource
    )
    {
        if (resource)
        {
            m_hCursor = LoadCursorW(
                instance->hInstance, MAKEINTRESOURCEW(resource)
            );

            CheckNull(m_hCursor)
            {
                QLog(
                    Warning, CSFormat(
                        "Error loading CURSOR with resourceId: {0} from {1}. Error: {2}",
                        resource, instance->instanceName, GetLastError()
                    )
                );
            }
        }
    }

    CursorInfo::CursorInfo(
        int resource
    ) : CursorInfo(NativeInstance::MirageAPI, resource)
    {
    }

    CursorInfo::CursorInfo(
        CursorType cursorType
    ) : CursorInfo(NativeInstance::System, static_cast<int>(cursorType))
    {
    }

    CursorInfo::CursorInfo(
        String^ path
    ) : m_hCursor(LoadCursorFromFileW(CStringToWChar(path)))
    {
        CheckNull(m_hCursor)
            QLog(Warning, CSFormat("Error loading cursor from path: {0}. Error: {1}", path, GetLastError()));
    }

    CursorInfo::CursorInfo(
        array<Byte>^ data
    )
    {
        pin_ptr<Byte> pinData = &data[0];
        m_hCursor = CreateIconFromResourceEx(
            static_cast<PBYTE>(pinData),
            static_cast<DWORD>(data->Length),
            FALSE,
            0x00030000,
            0, 0,
            LR_DEFAULTCOLOR
        );

        CheckNull(m_hCursor)
            QLog(Warning, "Error loading cursor from data. Error:" + GetLastError());
    }

    void CursorInfo::!CursorInfo()
    {
        SafeDestroy(DestroyCursor, m_hCursor);
    }
}
