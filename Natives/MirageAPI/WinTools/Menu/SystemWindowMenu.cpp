#include "pch.h"
#include "SystemWindowMenu.h"

#include "../Window.h"

namespace MirageAPI
{
    SystemWindowMenu::SystemWindowMenu(Window^ window) : Menu(window)
    {
        m_handle = GetSystemMenu(window->hwnd, FALSE);
    }

    void SystemWindowMenu::!SystemWindowMenu()
    {
        SafeDestroy(DestroyMenu, m_handle);
    }

    void SystemWindowMenu::AddItem(MenuItem^ item)
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

    void SystemWindowMenu::RemoveItem(int commandId)
    {
        RemoveMenu(m_handle, commandId, MF_BYCOMMAND);
    }
}
