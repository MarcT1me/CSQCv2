#pragma once

#include "Menu.h"
#include "MenuItem.h"

#define TRAY_MENU_EVENT_TYPE WM_USER + 0x100

namespace MirageAPI
{
    ref class Window;

    public ref class TrayIconMenu : public Menu
    {
        int m_id;
        SimpleRect^ m_rect;
        String^ m_tooltipText;

    public:
        static constexpr UINT TrayMenuEventType = TRAY_MENU_EVENT_TYPE;

        TrayIconMenu(
            Window^ window, String^ tooltipText,
            int id, SimpleRect^ rect
        );

        ~TrayIconMenu() { this->!TrayIconMenu(); }
        !TrayIconMenu();

        void AddToTray();
        void RemoveFromTray();

        void Show(Vector2i^ position);

        void AddItem(MenuItem^ item);
        void RemoveItem(int commandId);
    };
}
