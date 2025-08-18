#pragma once

namespace MirageAPI
{
    public enum class CursorType
    {
        ARROW = 32512,
        IBEAM = 32513,
        WAIT = 32514,
        CROSS = 32515,
        UP_ARROW = 32515,
        SIZE_NWSE = 32642,
        SIZE_NESW = 32643,
        SIZE_WE = 32644,
        SIZE_NS = 32645,
        SIZE_ALL = 32646

#if(WINVER >= 0x0500)
        , HAND = 32649
#endif
#if(WINVER >= 0x0400)
        , HELP = 32651
#endif
    };
}
