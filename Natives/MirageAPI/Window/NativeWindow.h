#pragma once

#include <Windows.h>

#include "../Events/NativeEvents.h"
#include "../DerectX/DX12WindowContext.h"


namespace MirageAPI::Window
{
    public ref class NativeWindow
    {
        HWND hwnd;
        HINSTANCE hInstance = nullptr;
        System::Runtime::InteropServices::GCHandle gch;

        DirectX::DX12WindowContext^ dxContext;

    internal:
        // rise methods (overhead...)
        void RaiseKeyEvent(Events::NativeKeyEvent event);
        void RaiseMouseEvent(Events::NativeMouseEvent event);
        void RaiseWindowEvent(Events::NativeWindowEvent event);
        void RaiseCharEvent(Events::NativeCharEvent event);
        void RaiseDropEvent(Events::NativeDropEvent event);

    public:
        // Keyboard

        delegate void KeyDelegate(Events::NativeKeyEvent event);
        event KeyDelegate^ OnKey;

        // Mouse

        delegate void MouseEventDelegate(Events::NativeMouseEvent event);
        event MouseEventDelegate^ OnMouse;

        // Window events

        delegate void WindowEventDelegate(Events::NativeWindowEvent event);
        event WindowEventDelegate^ OnWindow;

        // other

        delegate void CharDelegate(Events::NativeCharEvent event);
        event CharDelegate^ OnChar;

        delegate void DropDelegate(Events::NativeDropEvent event);
        event DropDelegate^ OnDrop;

        // initializations and property

        static NativeWindow();

        NativeWindow(
            int width, int height,
            System::String^ title,
            NativeWindow^ parent
        );
        ~NativeWindow();
        !NativeWindow();

        property System::IntPtr Handle { System::IntPtr get(); }
        property DirectX::DX12WindowContext^ DXContext { DirectX::DX12WindowContext^ get() { return dxContext; } }

        void HandleResize(int width, int height);
        void SetVSync(bool enabled);
        void Clear(float r, float g, float b, float a);

        // other methods
        void BeginFrame();
        void EndFrame();
        void SwapBuffers();
    };
}
