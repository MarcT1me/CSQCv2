#include "pch.h"
#include "IconInfo.h"

namespace MirageAPI
{
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
    }

    IconInfo::IconInfo(
        array<unsigned char>^ pixelData
    )
    {
        pin_ptr<Byte> pinData = &pixelData[0];
        m_hIcon = CreateIconFromResourceEx(
            static_cast<PBYTE>(pinData),
            static_cast<DWORD>(pixelData->Length),
            FALSE,
            0x00030000,
            0, 0,
            LR_DEFAULTCOLOR
        );
    }

    void IconInfo::!IconInfo()
    {
        SafeDestroy(DestroyCursor, m_hIcon);
    }
}
