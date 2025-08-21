#include "pch.h"
#include "TrayMenu.h"

#include "shellapi.h"

#include "../Window/Window.h"
#include "../Icon/IconInfo.h"

namespace MirageAPI
{
    TrayMenu::TrayMenu(
        Window^ window, int id, Rect^ rect,
        String^ tooltipText, String^ infoTitleText, String^ infoText
    ) : Menu(window),
        m_id(id),
        m_rect(rect),
        m_tooltipText(tooltipText),
        m_infoTitleText(infoTitleText),
        m_infoText(infoText)
    {
        m_handle = CreatePopupMenu();
    }

    void TrayMenu::!TrayMenu()
    {
        RemoveFromTray();
        SafeDestroy(DestroyMenu, m_handle);
    }

    void TrayMenu::AddToTray()
    {
        NOTIFYICONDATA nid;
        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.hWnd = m_window->NativeWindow;
        nid.uID = m_id;
        nid.uFlags = NIF_MESSAGE;
        nid.uCallbackMessage = TrayMenuEventType;
        if (auto icon = m_window->Style->Icon)
        {
            nid.uFlags |= NIF_ICON;
            nid.hIcon = icon->NativeIcon;
        }
        if (m_tooltipText)
        {
            nid.uFlags |= NIF_TIP;
            pin_ptr<const wchar_t> pinned = PtrToStringChars(m_tooltipText);
            wcsncpy_s(nid.szTip, pinned, _countof(nid.szTip) - 1);
        }
        if (m_infoTitleText)
        {
            nid.uFlags |= NIF_INFO;
            pin_ptr<const wchar_t> pinned = PtrToStringChars(m_infoTitleText);
            wcsncpy_s(nid.szInfoTitle, pinned, _countof(nid.szTip) - 1);
        }
        if (m_infoText)
        {
            nid.uFlags |= NIF_INFO;
            pin_ptr<const wchar_t> pinned = PtrToStringChars(m_infoText);
            wcsncpy_s(nid.szInfo, pinned, _countof(nid.szTip) - 1);
        }

        Shell_NotifyIcon(NIM_ADD, &nid);
    }

    void TrayMenu::RemoveFromTray()
    {
        NOTIFYICONDATA nid;
        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.uID = m_id;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        nid.hWnd = m_window->NativeWindow;
        Shell_NotifyIcon(NIM_DELETE, &nid);
    }

    void TrayMenu::Show(Vector2i^ position)
    {
        RECT* rectPtr = nullptr;

        if (m_rect)
        {
            RECT rect;

            rect.left = m_rect->X;
            rect.top = m_rect->Y;
            rect.right = m_rect->X + m_rect->Width;
            rect.bottom = m_rect->Y + m_rect->Height;

            rectPtr = &rect;
        }

        TrackPopupMenu(
            m_handle, TPM_RIGHTBUTTON,
            UnpacVec2(position),
            0, m_window->NativeWindow, rectPtr
        );
    }

    int TrayMenu::TextItem(String^ text)
    {
        count++;
        AppendMenuW(m_handle, MF_STRING, count, CStringToWChar(text));
        return count;
    }

    void TrayMenu::Separator()
    {
        AppendMenuW(m_handle, MF_SEPARATOR, 0, nullptr);
    }

    void TrayMenu::RemoveItem(int commandId)
    {
        RemoveMenu(m_handle, commandId, MF_BYCOMMAND);
    }
}
