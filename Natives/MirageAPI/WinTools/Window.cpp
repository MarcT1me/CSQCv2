#include "pch.h"
#include "Window.h"

#pragma comment(lib, "dwmapi.lib")
#include <dwmapi.h>

#include "Events/EventProc.h"
#include "Events/EventManager.h"

namespace MirageAPI
{
    // event rises
    void Window::RaiseKeyEvent(Events::NativeKeyEvent^ event)
    {
        OnKey(event);
    }

    void Window::RaiseMouseEvent(Events::NativeMouseEvent^ event)
    {
        OnMouse(event);
    }

    void Window::RaiseWindowEvent(Events::NativeWindowEvent^ event)
    {
        OnWindow(event);
    }

    void Window::RaiseCharEvent(Events::NativeCharEvent^ event)
    {
        OnChar(event);
    }

    void Window::RaiseDropEvent(Events::NativeDropEvent^ event)
    {
        OnDrop(event);
    }

    // initializations and property

    static Window::Window()
    {
        hInstance = GetModuleHandle(nullptr);

        WNDCLASSEX QWindow = {};
        QWindow.cbSize = sizeof(WNDCLASSEX);
        QWindow.style = CS_HREDRAW | CS_VREDRAW;
        QWindow.lpfnWndProc = reinterpret_cast<WNDPROC>(Events::EventProc::QuantumWindowProc);
        QWindow.hInstance = hInstance;
        QWindow.hCursor = LoadCursor(nullptr, IDC_ARROW);
        QWindow.lpszClassName = L"QWindow";

        if (!RegisterClassEx(&QWindow))
        {
            DWORD error = GetLastError();
            throw gcnew Exception("Window class registration error: " + error);
        }

        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
        SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    }

    Window::Window(
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
    )
    {
        savedRect = gcnew DoubleRect(rect);
        savedStyle = parent
                         ? WindowType::Child
                         : wType;

        int x = savedRect->X;
        int y = savedRect->Y;

        if (parent && (x == CW_USEDEFAULT || y == CW_USEDEFAULT))
        {
            x = 0;
            y = 0;
        }

        hwnd = CreateWindowEx(
            0,
            L"QWindow",
            CStringToWChar(title),
            static_cast<UINT>(savedStyle),
            x, y, savedRect->Width, savedRect->Height,
            parent ? parent->hwnd : nullptr,
            nullptr, hInstance, nullptr
        );
        ThrowIfNull(hwnd, "Error creating Window");
        SetFullscreen(isFullscreen);
        Opacity = opacity;
        Display = display;
        Icon = icon;
        Cursor = cursor;
        m_sysMenu = gcnew SystemWindowMenu(this);
        savedMousePosition = MousePosition;

        {
            gch = Runtime::InteropServices::GCHandle::Alloc(this);
            void* native_ptr = Runtime::InteropServices::GCHandle::ToIntPtr(gch).ToPointer();
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(native_ptr));
        }

        dxContext = gcnew DirectX::DX12Context(hwnd, dxContextConfig);

        winList->Add(this);
    }

    Window::!Window()
    {
        SimpleDelete(dxContext);

        if (winList->IndexOf(this))
        {
            winList->Remove(this);
        }

        if (hwnd)
        {
            if (gch.IsAllocated)
            {
                gch.Free();
            }
            hwnd = nullptr;
        }
    }

    DoubleRect^ Window::CurrentWindowRect::get()
    {
        RECT winRect;
        GetWindowRect(hwnd, &winRect);

        DoubleRect^ rect = gcnew DoubleRect();
        rect->Left = winRect.left;
        rect->Right = winRect.right;
        rect->Top = winRect.top;
        rect->Bottom = winRect.bottom;

        rect->X = winRect.left;
        rect->Y = winRect.top;
        rect->Width = winRect.right - winRect.left;
        rect->Height = winRect.bottom - winRect.top;
        return rect;
    }

    float Window::Opacity::get()
    {
        BYTE alpha = 0;
        DWORD flags = 0;
        if (GetLayeredWindowAttributes(hwnd, nullptr, &alpha, &flags) && flags & LWA_ALPHA)
        {
            return static_cast<float>(alpha) / 255.0f;
        }
        return 1.0f;
    }

    void Window::Opacity::set(float value)
    {
        value = std::min(std::max(value, 0.0f), 1.0f);
        BYTE alpha = static_cast<BYTE>(value * 255);
        SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
    }

    bool Window::DarkMode::get()
    {
        bool isDarkMode;
        DwmGetWindowAttribute(
            hwnd,
            DWMWA_USE_IMMERSIVE_DARK_MODE,
            &isDarkMode,
            sizeof(isDarkMode)
        );
        return isDarkMode;
    }

    void Window::DarkMode::set(bool value)
    {
        DwmSetWindowAttribute(
            hwnd,
            DWMWA_USE_IMMERSIVE_DARK_MODE,
            &value,
            sizeof(value)
        );
    }

    void Window::BorderColor::set(Color4 value)
    {
        m_borderColor = value;
        COLORREF color = value.ToArgb();
        DwmSetWindowAttribute(
            hwnd,
            DWMWA_BORDER_COLOR,
            &color,
            sizeof(color)
        );
    }

    void Window::CaptionColor::set(Color4 value)
    {
        m_captionColor = value;
        COLORREF color = value.ToArgb();
        DwmSetWindowAttribute(
            hwnd,
            DWMWA_CAPTION_COLOR,
            &color,
            sizeof(color)
        );
    }

    Vector2i^ Window::MousePosition::get()
    {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(hwnd, &point);
        return gcnew Vector2i(point.x, point.y);
    }

    void Window::MousePosition::set(Vector2i^ value)
    {
        POINT center = {static_cast<LONG>(value->X), static_cast<LONG>(value->Y)};
        ClientToScreen(hwnd, &center);
        SetCursorPos(center.x, center.y);
    }

    bool Window::IsMinimized::get()
    {
        return IsIconic(hwnd) && TRUE;
    }

    bool Window::IsMaximized::get()
    {
        return IsZoomed(hwnd) && TRUE;
    }

    bool Window::IsVisible::get()
    {
        return IsWindowVisible(hwnd) && TRUE;
    }

    bool Window::IsUnicode::get()
    {
        return IsWindowUnicode(hwnd) && TRUE;
    }

    bool Window::IsEnabled::get()
    {
        return IsWindowEnabled(hwnd) && TRUE;
    }

    void Window::Display::set(DisplayInfo^ value)
    {
        CheckNull(value) return;

        DoubleRect^ rect = CurrentWindowRect;
        DisplayInfo^ curMonitor = Display;

        int xRel = rect->Left - curMonitor->Position.X;
        int yRel = rect->Top - curMonitor->Position.Y;

        int x = value->Position.X + xRel;
        int y = value->Position.Y + yRel;

        SetPosition(gcnew Vector2i(x, y));
    }

    void Window::Cursor::set(CursorInfo^ value)
    {
        m_cursor = value;

        CheckNull(value) return;

        Events::EventManager::SendEvent(
            this, WM_SETCURSOR,
            reinterpret_cast<WPARAM>(hwnd),
            MAKELPARAM(HTCLIENT, 0)
        );
    }

    void Window::Icon::set(IconInfo^ value)
    {
        m_icon = value;

        CheckNull(value) return;

        Events::EventManager::SendEvent(
            this, WM_SETICON,
            ICON_SMALL,
            reinterpret_cast<LPARAM>(value->NativeIcon)
        );
        Events::EventManager::SendEvent(
            this, WM_SETICON,
            ICON_BIG,
            reinterpret_cast<LPARAM>(value->NativeIcon)
        );
    }

    void Window::Establish()
    {
        Show();
        Update();
    }

    void Window::Show()
    {
        ShowWindow(hwnd, SW_SHOW);
    }

    void Window::Hide()
    {
        ShowWindow(hwnd, SW_HIDE);
    }

    void Window::Maximize()
    {
        ShowWindow(hwnd, SW_MAXIMIZE);
    }

    void Window::Minimize()
    {
        ShowWindow(hwnd, SW_MINIMIZE);
    }

    void Window::Restore()
    {
        ShowWindow(hwnd, SW_RESTORE);
    }

    void Window::ToggleFullscreen()
    {
        SetFullscreen(!isFullscreen);
    }

    void Window::SetFullscreen(bool isFull)
    {
        if (isFull && !isFullscreen)
        {
            savedRect = CurrentWindowRect;
            DisplayInfo^ displayInfo = Display;

            SetWindowLongPtr(
                hwnd, GWL_STYLE,
                static_cast<DWORD>(WindowType::Fullscreen)
            );

            SetWindowPos(
                hwnd,
                HWND_TOP,
                displayInfo->Position.X,
                displayInfo->Position.Y,
                displayInfo->Size.X,
                displayInfo->Size.Y,
                SWP_FRAMECHANGED | SWP_SHOWWINDOW
            );

            isFullscreen = true;
        }
        else if (isFullscreen && !isFull)
        {
            SetWindowLongPtr(
                hwnd, GWL_STYLE,
                static_cast<UINT>(savedStyle)
            );

            SetWindowPos(
                hwnd,
                nullptr,
                savedRect->X,
                savedRect->Y,
                savedRect->Width,
                savedRect->Height,
                SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOZORDER
            );

            isFullscreen = false;
        }
    }

    void Window::ToggleMouseVisibility()
    {
        SetMouseVisibility(!isMouseVisible);
    }

    void Window::SetMouseVisibility(bool isVisible)
    {
        ShowCursor(isVisible);
        isMouseVisible = isVisible;
    }

    void Window::ToggleMouseCapture()
    {
        SetMouseCapture(!mouseCaptureWindow);
    }

    void Window::SetMouseCapture(bool isCapture)
    {
        if (isCapture && mouseCaptureWindow && mouseCaptureWindow != this)
        {
            QLog(Warning, "Mouse has already been captured in another window\nTrying to release capture mouse");
            mouseCaptureWindow->SetMouseCapture(false);
        }

        if (isCapture && !mouseCaptureWindow)
        {
            lastMousePosition = savedMousePosition = MousePosition;

            mouseCaptureWindow = this;
        }
        else if (mouseCaptureWindow && !isCapture)
        {
            mouseCaptureWindow = nullptr;
        }
    }

    void Window::CursorLeaveHandle()
    {
        SetMouseCapture(false);
        savedMousePosition = nullptr;
    }

    void Window::UpdateMousePosition(int x, int y)
    {
        if (savedMousePosition)
        {
            lastMousePosition = savedMousePosition;
        }
        else
        {
            lastMousePosition = MousePosition;
            Events::EventManager::CursorEnterCallback(this);
        }
        savedMousePosition = gcnew Vector2i(x, y);

        mouseDelta = gcnew Vector2i(
            savedMousePosition->X - lastMousePosition->X,
            savedMousePosition->Y - lastMousePosition->Y
        );

        if (mouseCaptureWindow == this)
        {
            DoubleRect^ rect = CurrentWindowRect;

            int centerX = static_cast<int>(rect->Width) / 2;
            int centerY = static_cast<int>(rect->Height) / 2;

            MousePosition = gcnew Vector2i(centerX, centerY);

            lastMousePosition = Vector2i(centerX, centerY);
            savedMousePosition = lastMousePosition;
        }
    }

    void Window::Destroy()
    {
        DestroyWindow(hwnd);
    }

    void Window::BringToFront()
    {
        Show();
        SetForegroundWindow(hwnd);
        SetWindowPos(
            hwnd, HWND_TOP,
            0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW
        );
    }

    void Window::Flash(UINT count, UINT timeout)
    {
        FLASHWINFO flashInfo;
        flashInfo.cbSize = sizeof(FLASHWINFO);
        flashInfo.hwnd = hwnd;
        flashInfo.dwFlags = FLASHW_ALL;

        if (timeout > 0)
        {
            flashInfo.dwFlags |= FLASHW_TIMER;
            flashInfo.dwTimeout = timeout;
        }
        else
        {
            flashInfo.dwFlags |= FLASHW_TIMERNOFG;
        }

        flashInfo.uCount = count > 0 ? count : 1;

        Console::Beep();

        FlashWindowEx(&flashInfo);
    }

    void Window::HideToTray()
    {
        Hide();
        m_trayMenu->AddToTray();
    }

    void Window::ShowFromTray()
    {
        m_trayMenu->RemoveFromTray();
        BringToFront();
    }

    void Window::SetTitle(String^ title)
    {
        SetWindowText(hwnd, CStringToWChar(title));
    }

    void Window::SetPosition(Vector2i^ pos)
    {
        SetWindowPos(hwnd, nullptr, pos->X, pos->Y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    }

    void Window::SetSize(Vector2i^ size)
    {
        SetWindowPos(hwnd, nullptr, 0, 0, size->X, size->Y, SWP_NOZORDER | SWP_NOMOVE);
    }

    void Window::SetPositionAndSize(Vector2i^ pos, Vector2i^ size)
    {
        SetWindowPos(hwnd, nullptr, pos->X, pos->Y, size->X, size->Y, SWP_NOZORDER);
    }

    void Window::Update()
    {
        UpdateWindow(hwnd);
    }

    void Window::BeginFrame()
    {
        dxContext->SetWinRect(CurrentWindowRect);
        dxContext->BeginFrame();
    }

    void Window::EndFrame()
    {
        dxContext->EndFrame();
    }

    void Window::Present()
    {
        dxContext->Present();
    }

    void Window::Clear(Color4 color)
    {
        dxContext->Clear(color);
    }
}
