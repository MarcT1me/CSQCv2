#pragma once

#include "enums.h"
#include "Display/DisplayManager.h"
#include "Cursor/CursorInfo.h"
#include "Events/Events.h"
#include "../DerectX/Context/DX12Context.h"
#include "Icon/IconInfo.h"
#include "Menu/SystemWindowMenu.h"
#include "Menu/TrayIconMenu.h"

namespace MirageAPI
{
    public ref class Window
    {
        bool isFullscreen;
        static Window^ mouseCaptureWindow;
        bool isMouseVisible = true;

        Color4 m_borderColor;
        Color4 m_captionColor;

        WindowType savedStyle;
        DoubleRect^ savedRect;

        Vector2i^ savedMousePosition;
        Vector2i^ lastMousePosition;
        Vector2i^ mouseDelta;

        CursorInfo^ m_cursor;
        IconInfo^ m_icon;

        TrayIconMenu^ m_trayMenu;
        SystemWindowMenu^ m_sysMenu;

        Runtime::InteropServices::GCHandle gch;

        DirectX::DX12Context^ dxContext;

    internal:
        HWND hwnd;
        static HINSTANCE hInstance = nullptr;

        static CSList<Window^>^ winList = gcnew CSList<Window^>();

        // rise methods (overhead...)
        void RaiseKeyEvent(Events::NativeKeyEvent^ event);
        void RaiseMouseEvent(Events::NativeMouseEvent^ event);
        void RaiseWindowEvent(Events::NativeWindowEvent^ event);
        void RaiseCharEvent(Events::NativeCharEvent^ event);
        void RaiseDropEvent(Events::NativeDropEvent^ event);

        void CursorLeaveHandle();
        void UpdateMousePosition(int x, int y);

        void Destroy();

    public:
        static int USE_DEFAULT_POSITION = CW_USEDEFAULT;

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

        static Window();

        Window(
            // most important
            WindowType wType,
            SimpleRect^ rect,
            String^ title,
            // not most important
            float opacity,
            bool isFullscreen,
            // other data
            Window^ parent,
            DisplayInfo^ display,
            IconInfo^ icon,
            CursorInfo^ cursor,
            // DX12
            DirectX::DX12ContextConfig^ dxContextConfig
        );

        ~Window() { this->!Window(); }
        !Window();

        property IntPtr Handle
        {
            IntPtr get() { return IntPtr(hwnd); }
        }
        property IntPtr DisplayHandle
        {
            IntPtr get() { return IntPtr(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST)); }
        }
        property DisplayInfo^ Display
        {
            DisplayInfo^ get() { return DisplayManager::GetDisplayFromHandle(DisplayHandle); }
            void set(DisplayInfo^ value);
        }
        property CursorInfo^ Cursor
        {
            CursorInfo^ get() { return m_cursor; }
            void set(CursorInfo^ value);
        }
        property IconInfo^ Icon
        {
            IconInfo^ get() { return m_icon; }
            void set(IconInfo^ value);
        }
        property TrayIconMenu^ IconMenu
        {
            TrayIconMenu^ get() { return m_trayMenu; }
            void set(TrayIconMenu^ value) { m_trayMenu = value; }
        }
        property SystemWindowMenu^ SysMenu
        {
            SystemWindowMenu^ get() { return m_sysMenu; }
        }

        property DirectX::DX12Context^ DXContext
        {
            DirectX::DX12Context^ get() { return dxContext; }
        }

        property UINT VSync
        {
            UINT get() { return dxContext->VSync; }
            void set(UINT value) { dxContext->VSync = value; }
        }
        property float Opacity
        {
            float get();
            void set(float value);
        }

        property bool DarkMode
        {
            bool get();
            void set(bool value);
        }
        property Color4 BorderColor
        {
            Color4 get() { return m_borderColor; }
            void set(Color4 value);
        }
        property Color4 CaptionColor
        {
            Color4 get() { return m_captionColor; }
            void set(Color4 value);
        }

        property DoubleRect^ CurrentWindowRect
        {
            DoubleRect^ get();
        }
        property Vector2i^ MousePosition
        {
            Vector2i^ get();
            void set(Vector2i^ v);
        }
        property Vector2i^ MouseDelta
        {
            Vector2i^ get() { return mouseDelta; }
        }

        property bool IsMinimized
        {
            bool get();
        }
        property bool IsMaximized
        {
            bool get();
        }
        property bool IsVisible
        {
            bool get();
        }
        property bool IsUnicode
        {
            bool get();
        }
        property bool IsEnabled
        {
            bool get();
        }
        property bool IsFullscreen
        {
            bool get() { return isFullscreen; }
        }
        property bool IsMouseVisible
        {
            bool get() { return isMouseVisible; }
        }
        property Window^ CaptureWindow
        {
            Window^ get() { return mouseCaptureWindow; }
        }

        // window actions

        void Establish();

        void Show();
        void Hide();

        void Maximize();

        void Minimize();
        void Restore();

        void ToggleFullscreen();
        void SetFullscreen(bool isFullscreen);

        void ToggleMouseVisibility();
        void SetMouseVisibility(bool isVisible);

        void ToggleMouseCapture();
        void SetMouseCapture(bool isCapture);

        void BringToFront();
        void Flash(UINT count, UINT timeout);

        void HideToTray();
        void ShowFromTray();

        void SetTitle(String^ title);
        void SetPosition(Vector2i^ pos);
        void SetSize(Vector2i^ size);
        void SetPositionAndSize(Vector2i^ pos, Vector2i^ size);

        void Update();

        void BeginFrame();
        void Clear(Color4 color);
        void EndFrame();
        void Present();
    };
}
