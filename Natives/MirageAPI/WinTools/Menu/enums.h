#pragma once

namespace MirageAPI
{
    public enum class MenuItemType
    {
        String = MF_STRING,
        Separator = MF_SEPARATOR
    };
    
    public enum class MenuIdRanges : UINT
    {
        SystemMenuStart = 0x1000,
        TrayMenuStart = 0x2000,
        UserMenuStart = 0x3000
    };
}
