#pragma once

#define TRAY_MENU_EVENT_TYPE WM_USER + 0x100

#include "Menu.h"

namespace MirageAPI
{
    ref class IconInfo;
    ref class Window;
}

namespace MirageAPI
{
    public ref class TrayMenu : public Menu
    {
        int m_id;
        int count;
        Rect^ m_rect;
        String^ m_tooltipText;
        String^ m_infoTitleText;
        String^ m_infoText;

    public:
        static constexpr UINT TrayMenuEventType = TRAY_MENU_EVENT_TYPE;

        TrayMenu(
            Window^ window, int id, Rect^ rect,
            String^ tooltipText, String^ infoTitleText, String^ infoText
        );

        ~TrayMenu() { this->!TrayMenu(); }
        !TrayMenu();

        void AddToTray();
        void RemoveFromTray();

        void Show(Vector2i^ position);

        int TextItem(String^ item);
        void Separator();
        void RemoveItem(int commandId);
    };
}
