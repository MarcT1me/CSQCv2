#pragma once

#include "enums.h"
#include "NativeDisplay.h"
#include "NativeDisplayInfo.h"
#include "../Events/NativeEvents.h"
#include "../DerectX/Context/DX12Context.h"

namespace MirageAPI::Window
{
    public ref class NativeWindow
    {
        bool isFullscreen;
        static NativeWindow^ mouseCaptureWindow;
        static bool isMouseVisible;
        DWORD savedStyle;
        DoubleRect^ savedRect;
        Vector2i^ savedMousePosition;
        Vector2i^ lastMousePosition;
        Vector2i^ mouseDelta;

        HWND hwnd;
        HINSTANCE hInstance = nullptr;

        Runtime::InteropServices::GCHandle gch;

        DirectX::DX12Context^ dxContext;

    internal:
        // rise methods (overhead...)
        void RaiseKeyEvent(Events::NativeKeyEvent^ event);
        void RaiseMouseEvent(Events::NativeMouseEvent^ event);
        void RaiseWindowEvent(Events::NativeWindowEvent^ event);
        void RaiseCharEvent(Events::NativeCharEvent^ event);
        void RaiseDropEvent(Events::NativeDropEvent^ event);

        void UpdateMousePosition(int x, int y);

    public:
        // Keyboard
        delegate void KeyDelegate(Events::NativeKeyEvent^ event);
        event KeyDelegate^ OnKey;
        // Mouse
        delegate void MouseEventDelegate(Events::NativeMouseEvent^ event);
        event MouseEventDelegate^ OnMouse;
        // Window events
        delegate void WindowEventDelegate(Events::NativeWindowEvent^ event);
        event WindowEventDelegate^ OnWindow;
        // other
        delegate void CharDelegate(Events::NativeCharEvent^ event);
        event CharDelegate^ OnChar;
        delegate void DropDelegate(Events::NativeDropEvent^ event);
        event DropDelegate^ OnDrop;

        // initializations and property

        static NativeWindow();

        NativeWindow(
            SimpleRect^ rect,
            String^ title,
            float opacity,
            bool isFullscreen,
            NativeWindow^ parent,
            WindowType wType,
            DirectX::DX12ContextConfig^ dxContextConfig
        );

        ~NativeWindow() { this->!NativeWindow(); }
        !NativeWindow();

        property IntPtr Handle
        {
            IntPtr get() { return IntPtr(hwnd); }
        }
        property IntPtr MonitorHandle
        {
            IntPtr get() { return IntPtr(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST)); }
        }

        property NativeMonitorInfo^ CurrentMonitor
        {
            NativeMonitorInfo^ get() { return NativeDisplay::GetMonitorFromHandle(MonitorHandle); }
        }
        property DirectX::DX12Context^ DXContext
        {
            DirectX::DX12Context^ get() { return dxContext; }
        }

        property DoubleRect^ CurrentWindowRect { DoubleRect^ get(); }
        property Vector2i^ MouseDelta { Vector2i^ get() { return mouseDelta; } }

        property float Opacity
        {
            float get();
            void set(float value);
        }
        property Vector2i^ MousePosition
        {
            Vector2i^ get();
            void set(Vector2i^ v);
        }

        property bool IsMinimized { bool get(); }
        property bool IsMaximized { bool get(); }
        property bool IsFullscreen { bool get() { return isFullscreen; } }

        static property bool IsMouseVisible { bool get() { return isMouseVisible; } }
        property NativeWindow^ CaptureWindow { NativeWindow^ get() { return mouseCaptureWindow; } }

        // window actions

        void Establish();

        void Show();
        void Hide();
        void Maximize();
        void Minimize();
        void Restore();

        void ToggleFullscreen();
        void SetFullscreen(bool isFullscreen);

        static void ToggleMouseVisibility();
        static void SetMouseVisibility(bool isVisible);

        void ToggleMouseCapture();
        void SetMouseCapture(bool isCapture);

        void BringToFront();
        void FlashWindow();

        void SetTitle(String^ title);
        void SetPositionAndSize(int x, int y, int width, int height);
        void SetPosition(int x, int y);
        void SetSize(int width, int height);
        void SetVSync(UINT interval);

        void MoveWindowToMonitor(NativeMonitorInfo^ monitorInfo);

        void Update();

        void BeginFrame();
        void Clear(float r, float g, float b, float a);
        void EndFrame();
        void Present();
    };
}
