#pragma once

namespace MirageAPI
{
    public enum class CursorType : int
    {
        ARROW = 32512,
        IBEAM = 32513,
        WAIT = 32514,
        CROSS = 32515,
        UP_ARROW = 32516,
        
        SIZE_RIGHT = 32642,
        SIZE_LEFT = 32643,
        SIZE_VERTICAL = 32644,
        SIZE_HORIZONTAL = 32645,
        SIZE_ALL = 32646,
        
        Unavailable = 32648,
        
        HAND = 32649,
        
        APP_STARTING = 32650,
        
        HELP = 32651,
        
        PIN = 32671,
        PERSON = 32672
    };
}
