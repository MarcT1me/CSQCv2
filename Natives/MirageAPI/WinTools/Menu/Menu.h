#pragma once

namespace MirageAPI
{
    ref class Window;

    public ref class Menu abstract
    {
    protected:
        HMENU m_handle;
        Window^ m_window;

    public:
        Menu(Window^ window)
            : m_window(window)
        {
        }

        property HMENU NativeMenu { HMENU get() { return m_handle; } }

        delegate void ItemClickedDelegate(int id);
        event ItemClickedDelegate^ Callback;

        void RaiseCallback(int id)
        {
            Callback(id);
        }
    };
}
