#pragma once

#include "enums.h"
#include "../Events/NativeEvents.h"
#include "../DerectX/Context/DX12Context.h"

namespace MirageAPI::Window
{
    public ref class NativeWindow
    {
        HWND hwnd;
        HINSTANCE hInstance = nullptr;
        System::Runtime::InteropServices::GCHandle gch;

        DirectX::DX12Context^ dxContext;

        bool IsFullscreen;
        unsigned long SavedStyle;
        WindowRect^ SavedRect;

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
            SimpleRect^ rect,
            System::String^ title,
            float opacity,
            bool isFullscreen,
            NativeWindow^ parent,
            WindowType wType,
            DirectX::DX12ContextConfig^ dxContextConfig
        );
        
        ~NativeWindow() { this->!NativeWindow(); }
        !NativeWindow();

        void Establish();

        void Show();
        void Hide();

        void Maximize();
        void Minimize();
        void Restore();
        void ToggleFullscreen();

        void BringToFront();
        void FlashWindow();

        void SetTitle(System::String^ title);
        void SetPosition(int x, int y);
        void SetSize(int width, int height);
        void SetPositionAndSize(int x, int y, int width, int height);
        void SetOpacity(float opacity);

        void HandleResize(int width, int height);
        void Update();

        property WindowRect CurrentRect { WindowRect get(); }
        property float Opacity { float get(); }

        property bool IsMinimized { bool get(); }
        property bool IsMaximized { bool get(); }

        property System::IntPtr Handle { System::IntPtr get() { return System::IntPtr(hwnd); } }
        property DirectX::DX12Context^ DXContext { DirectX::DX12Context^ get() { return dxContext; } }

        void SetVSync(UINT interval);
        void Clear(float r, float g, float b, float a);

        // other methods
        void BeginFrame();
        void EndFrame();
        void Present();
    };
}
