#include "pch.h"
#include "IconInfo.h"

#include "../Window/WindowClass.h"

namespace MirageAPI
{
    IconInfo::IconInfo(
        NativeInstance^ instance,
        int resource
    )
    {
        if (resource)
        {
            m_hIcon = LoadIconW(
                instance->hInstance, MAKEINTRESOURCEW(resource)
            );

            CheckNull(m_hIcon)
            {
                QLog(
                    Warning, CSFormat(
                        "Error loading ICON with resourceId: {0} from {1}. Error: {2}",
                        resource, instance->instanceName, GetLastError()
                    )
                );
            }
        }
    }

    IconInfo::IconInfo(
        int resource
    ) : IconInfo(NativeInstance::MirageAPI, resource)
    {
    }

    IconInfo::IconInfo(
        IconType cursorType
    ) : IconInfo(NativeInstance::System, static_cast<int>(cursorType))
    {
    }

    IconInfo::IconInfo(
        String^ path
    ) : m_hIcon(static_cast<HICON>(LoadImageW(
            nullptr,
            CStringToWChar(path),
            IMAGE_ICON,
            0, 0,
            LR_LOADFROMFILE | LR_DEFAULTSIZE
        ))
    )
    {
        CheckNull(m_hIcon)
            QLog(Warning, CSFormat("Error loading icon from path: {0}. Error: {1}", path, GetLastError()));
    }

    IconInfo::IconInfo(
        array<Byte>^ data
    )
    {
        pin_ptr<Byte> pinData = &data[0];
        m_hIcon = CreateIconFromResourceEx(
            static_cast<PBYTE>(pinData),
            static_cast<DWORD>(data->Length),
            FALSE,
            0x00030000,
            0, 0,
            LR_DEFAULTCOLOR
        );

        CheckNull(m_hIcon)
            QLog(Warning, "Error loading icon from data. Error: " + GetLastError());
    }

    void IconInfo::!IconInfo()
    {
        SafeDestroy(DestroyIcon, m_hIcon);
    }
}
