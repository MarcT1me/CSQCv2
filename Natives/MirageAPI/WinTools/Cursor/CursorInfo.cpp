#include "pch.h"
#include "CursorInfo.h"

namespace MirageAPI
{
    CursorInfo::CursorInfo(
        CursorType cursorType
    ) : m_hCursor(
        LoadCursor(
            nullptr,
            MAKEINTRESOURCE(cursorType)
        )
    )
    {
    }

    CursorInfo::CursorInfo(
        String^ path
    ) : m_hCursor(static_cast<HCURSOR>(LoadImageW(
            nullptr,
            CStringToWChar(path),
            IMAGE_CURSOR,
            0, 0,
            LR_LOADFROMFILE | LR_DEFAULTSIZE
        ))
    )
    {
    }

    CursorInfo::CursorInfo(
        array<unsigned char>^ pixelData,
        int width, int height,
        int hotX, int hotY
    )
    {
        pin_ptr<Byte> pinData = &pixelData[0];
        HBITMAP hColor = CreateBitmap(
            width, height,
            1, 32,
            pinData
        );
        HBITMAP hMask = CreateBitmap(
            width, height,
            1, 1,
            nullptr
        );

        ICONINFO iconInfo;
        iconInfo.fIcon = false;
        iconInfo.xHotspot = hotX;
        iconInfo.yHotspot = hotY;
        iconInfo.hbmColor = hColor;
        iconInfo.hbmMask = hMask;

        m_hCursor = CreateIconIndirect(&iconInfo);

        DeleteObject(hColor);
        DeleteObject(hMask);
    }

    CursorInfo::CursorInfo(
        array<unsigned char>^ data
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
    }

    void CursorInfo::!CursorInfo()
    {
        SafeDestroy(DestroyCursor, m_hCursor);
    }
}
