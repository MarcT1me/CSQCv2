#include "pch.h"
#include "TrayIconMenu.h"

#include "shellapi.h"

#include "../Window.h"

namespace MirageAPI
{
    TrayIconMenu::TrayIconMenu(
        Window^ window,
        String^ tooltipText,
        int id,
        SimpleRect^ rect
    ) : Menu(window),
        m_id(id),
        m_rect(rect),
        m_tooltipText(tooltipText)
    {
        m_handle = CreatePopupMenu();
    }

    void TrayIconMenu::!TrayIconMenu()
    {
        RemoveFromTray();
        SafeDestroy(DestroyMenu, m_handle);
    }

    void TrayIconMenu::AddToTray()
    {
        NOTIFYICONDATA nid;
        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.uID = m_id;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        nid.uCallbackMessage = TrayMenuEventType;
        nid.hWnd = m_window->hwnd;
        nid.hIcon = m_window->Icon->NativeIcon;

        pin_ptr<const wchar_t> pinned = PtrToStringChars(m_tooltipText);
        wcsncpy_s(nid.szTip, pinned, _countof(nid.szTip) - 1);

        Shell_NotifyIcon(NIM_ADD, &nid);
    }

    void TrayIconMenu::RemoveFromTray()
    {
        NOTIFYICONDATA nid;
        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.uID = m_id;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        nid.hWnd = m_window->hwnd;
        Shell_NotifyIcon(NIM_DELETE, &nid);
    }

    void TrayIconMenu::Show(Vector2i^ position)
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
            m_handle,
            TPM_RIGHTBUTTON,
            position->X, position->Y,
            0, m_window->hwnd, rectPtr
        );
    }

    void TrayIconMenu::AddItem(MenuItem^ item)
    {
        switch (static_cast<long>(item->Type))
        {
        case MF_SEPARATOR:
            {
                AppendMenu(
                    m_handle, MF_SEPARATOR,
                    0, nullptr
                );
                break;
            }
        case MF_STRING:
            {
                String^ text = item->Text;
                AppendMenu(
                    m_handle, MF_STRING,
                    item->Id, CStringToWChar(text)
                );
                break;
            }
        default: ;
        }
    }

    void TrayIconMenu::RemoveItem(int commandId)
    {
        RemoveMenu(m_handle, commandId, MF_BYCOMMAND);
    }
}
