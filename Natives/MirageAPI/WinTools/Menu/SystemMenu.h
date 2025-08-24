#pragma once

#include "Menu.h"

namespace MirageAPI
{
    public ref class SystemMenu : public Menu
    {
        int count = static_cast<int>(MenuIdRanges::SystemMenuStart);
        
    public:
        SystemMenu(Window^ window);

        ~SystemMenu() { this->!SystemMenu(); }
        !SystemMenu();
        
        int TextItem(String^ item);
        void Separator();
        void RemoveItem(int commandId);
    };
}
