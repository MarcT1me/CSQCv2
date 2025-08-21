#pragma once

#include "WindowStyleInfo.h"
#include "../Mouse/Mouse.h"
#include "../Display/DisplayManager.h"
#include "../Events/Events.h"
#include "../../DerectX/Context/DX12Context.h"
#include "../Menu/SystemMenu.h"
#include "../Menu/TrayMenu.h"

namespace MirageAPI
{
    ref class WindowClass;
}

namespace MirageAPI
{
    public ref class Window
    {
        // defaults
        WindowClass^ wClass;
        WindowType type;
        String^ title;
        Rect^ rect;
        bool isFullscreen;
        WindowStyleInfo^ wStyle;

        // other window params
        TrayMenu^ trayMenu;
        SystemMenu^ sysMenu;
        Mouse^ mouse;
        DisplayInfo^ display;

        // hwnd GCHandle & DirectX context
        HWND hwnd;
        Runtime::InteropServices::GCHandle gch;
        DirectX::DX12Context^ dxContext;

    internal:
        // list of all windows
        static CSList<Window^>^ winList = gcnew CSList<Window^>();

        // other methods
        void WindowEventHandle(Events::WindowEvent^ event);
        void Destroy();

    public:
        static constexpr int USE_DEFAULT_POSITION = CW_USEDEFAULT;

        // Window events
        delegate void WindowEventDelegate(Events::WindowEvent^ event);
        event WindowEventDelegate^ OnWindow;

        // initializations and property

        Window(
            WindowClass^ wClass,
            WindowType wType,
            String^ title,
            Rect^ rect,
            bool isFullscreen,
            WindowStyleInfo^ style,
            // other data
            Window^ parent,
            DisplayInfo^ display,
            // DX12
            DirectX::DX12ContextConfig^ dxContextConfig
        );

        ~Window() { this->!Window(); }
        !Window();

        property WindowClass^ Class
        {
            WindowClass^ get() { return wClass; }
        }
        property WindowType Type
        {
            WindowType get() { return type; }
        }
        property String^ Title
        {
            String^ get() { return title; }
            void set(String^ value);
        }
        property Rect^ CurrentRect
        {
            Rect^ get();
            void set(Rect^ value);
        }
        property bool IsFullscreen
        {
            bool get() { return isFullscreen; }
            void set(bool value);
        }
        property WindowStyleInfo^ Style
        {
            WindowStyleInfo^ get() { return wStyle; }
            void set(WindowStyleInfo^ value) { wStyle = value->LinkToWindow(this); }
        }
        property bool IsHighContrastEnabled
        {
            bool get();
        }

        property TrayMenu^ TrayMenu
        {
            MirageAPI::TrayMenu^ get() { return trayMenu; }
            void set(MirageAPI::TrayMenu^ value) { trayMenu = value; }
        }
        property SystemMenu^ SysMenu
        {
            SystemMenu^ get() { return sysMenu; }
        }

        property Mouse^ Mouse
        {
            MirageAPI::Mouse^ get() { return mouse; }
        }
        property DisplayInfo^ Display
        {
            DisplayInfo^ get() { return display; }
            void set(DisplayInfo^ value);
        }

        property HWND NativeWindow
        {
            HWND get() { return hwnd; }
        }
        property IntPtr Handle
        {
            IntPtr get() { return IntPtr(hwnd); }
        }
        property IntPtr DisplayHandle
        {
            IntPtr get() { return IntPtr(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST)); }
        }
        property DirectX::DX12Context^ DXContext
        {
            DirectX::DX12Context^ get() { return dxContext; }
        }

        property bool IsEnabled
        {
            bool get();
        }
        property bool IsVisible
        {
            bool get();
            void set(bool value);
        }
        property bool IsIcon
        {
            bool get();
        }
        property bool IsMaximized
        {
            bool get();
            void set(bool value);
        }

        // window actions
        void SetPosition(Vector2i^ pos);
        void SetSize(Vector2i^ size);
        void SetPositionAndSize(Vector2i^ pos, Vector2i^ size);
        void SetRegion(Vector2i^ size);

        void Restore();
        void BringToFront();
        void Flash(UINT count, UINT timeout);
        void HideToTray();
        void ShowFromTray();
        void Update();
        void FrameChanged();

        void Establish();

        void BeginFrame();
        void Clear(Color4 color);
        void EndFrame();
        void Present();
    };
}
