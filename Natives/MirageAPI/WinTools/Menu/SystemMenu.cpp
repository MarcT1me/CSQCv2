#include "pch.h"
#include "SystemMenu.h"

#include "../Window/Window.h"

namespace MirageAPI
{
    SystemMenu::SystemMenu(Window^ window) : Menu(window)
    {
        m_handle = GetSystemMenu(window->NativeWindow, FALSE);
    }

    void SystemMenu::!SystemMenu()
    {
        SafeDestroy(DestroyMenu, m_handle);
    }

    int SystemMenu::TextItem(String^ text)
    {
        count++;
        AppendMenuW(m_handle, MF_STRING, count, CStringToWChar(text));
        return count;
    }
    
    void SystemMenu::Separator()
    {
        AppendMenuW(m_handle, MF_SEPARATOR, 0, nullptr);
    }

    void SystemMenu::RemoveItem(int commandId)
    {
        RemoveMenu(m_handle, commandId, MF_BYCOMMAND);
    }
}
