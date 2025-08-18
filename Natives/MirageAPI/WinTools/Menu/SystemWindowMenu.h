#pragma once

#include "Menu.h"
#include "MenuItem.h"

namespace MirageAPI
{
    public ref class SystemWindowMenu : public Menu
    {
    public:
        SystemWindowMenu(Window^ window);

        ~SystemWindowMenu() { this->!SystemWindowMenu(); }
        !SystemWindowMenu();
        
        void AddItem(MenuItem^ item);
        void RemoveItem(int commandId);
    };
}
