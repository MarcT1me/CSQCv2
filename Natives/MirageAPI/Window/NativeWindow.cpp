#include "pch.h"
#include "NativeWindow.h"

#include "../Events/NativeEventManager.h"
#include "../Events/NativeEventProc.h"

namespace MirageAPI::Window
{
    // event rises
    void NativeWindow::RaiseKeyEvent(Events::NativeKeyEvent^ event)
    {
        OnKey(event);
    }

    void NativeWindow::RaiseMouseEvent(Events::NativeMouseEvent^ event)
    {
        OnMouse(event);
    }

    void NativeWindow::RaiseWindowEvent(Events::NativeWindowEvent^ event)
    {
        OnWindow(event);
    }

    void NativeWindow::RaiseCharEvent(Events::NativeCharEvent^ event)
    {
        OnChar(event);
    }

    void NativeWindow::RaiseDropEvent(Events::NativeDropEvent^ event)
    {
        OnDrop(event);
    }

    // initializations and property

    static NativeWindow::NativeWindow()
    {
        WNDCLASSEX QWindow = {};
        QWindow.cbSize = sizeof(WNDCLASSEX);
        QWindow.style = CS_HREDRAW | CS_VREDRAW;
        QWindow.lpfnWndProc = reinterpret_cast<WNDPROC>(Events::NativeEventProc::QuantumWindowProc);
        QWindow.hInstance = GetModuleHandle(nullptr);
        QWindow.hCursor = LoadCursor(nullptr, IDC_ARROW);
        QWindow.lpszClassName = L"QWindow";
        RegisterClassEx(&QWindow);

        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    }

    NativeWindow::NativeWindow(
        SimpleRect^ rect,
        String^ title,
        float opacity,
        bool isFullscreen,
        NativeWindow^ parent,
        NativeDisplayInfo^ display,
        WindowType wType,
        DirectX::DX12ContextConfig^ dxContextConfig
    )
    {
        savedRect = gcnew DoubleRect(rect);
        savedStyle = (parent ? WS_CHILDWINDOW : static_cast<unsigned long>(wType)) | WS_EX_LAYERED;

        hwnd = CreateWindowEx(
            0, L"QWindow",
            msclr::interop::marshal_as<std::wstring>(title).c_str(),
            savedStyle,
            savedRect->X,
            savedRect->Y,
            savedRect->Width,
            savedRect->Height,
            parent ? parent->hwnd : nullptr,
            nullptr, hInstance, nullptr
        );
        Opacity = opacity;
        SetFullscreen(isFullscreen);
        savedMousePosition = MousePosition;
        if (display != nullptr) MoveOnDisplay(display);

        {
            gch = Runtime::InteropServices::GCHandle::Alloc(this);
            void* native_ptr = Runtime::InteropServices::GCHandle::ToIntPtr(gch).ToPointer();
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(native_ptr));
        }
        dxContext = gcnew DirectX::DX12Context(hwnd, dxContextConfig);
    }

    NativeWindow::!NativeWindow()
    {
        SimpleDelete(dxContext);

        if (hwnd)
        {
            if (gch.IsAllocated)
            {
                gch.Free();
            }
            DestroyWindow(hwnd);
            hwnd = nullptr;
        }
    }

    void NativeWindow::Establish()
    {
        Show();
        Update();
    }

    void NativeWindow::Show()
    {
        ShowWindow(hwnd, SW_SHOW);
    }

    void NativeWindow::Hide()
    {
        ShowWindow(hwnd, SW_HIDE);
    }

    void NativeWindow::Maximize()
    {
        ShowWindow(hwnd, SW_MAXIMIZE);
    }

    void NativeWindow::Minimize()
    {
        ShowWindow(hwnd, SW_MINIMIZE);
    }

    void NativeWindow::Restore()
    {
        ShowWindow(hwnd, SW_RESTORE);
    }

    void NativeWindow::ToggleFullscreen()
    {
        SetFullscreen(!isFullscreen);
    }

    void NativeWindow::SetFullscreen(bool isFull)
    {
        if (isFull && !isFullscreen)
        {
            savedRect = CurrentWindowRect;
            NativeDisplayInfo^ displayInfo = CurrentMonitor;

            SetWindowLongPtr(
                hwnd, GWL_STYLE,
                savedStyle & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU)
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
                savedStyle
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

    void NativeWindow::ToggleMouseVisibility()
    {
        SetMouseVisibility(!isMouseVisible);
    }

    void NativeWindow::SetMouseVisibility(bool isVisible)
    {
        ShowCursor(isVisible);
    }

    void NativeWindow::ToggleMouseCapture()
    {
        SetMouseCapture(!mouseCaptureWindow);
    }

    void NativeWindow::SetMouseCapture(bool isCapture)
    {
        if (isCapture && mouseCaptureWindow && mouseCaptureWindow != this)
        {
            QuantumLog(Warning, "Mouse has already been captured in another window\nTrying to release capture mouse");
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

    void NativeWindow::CursorLeaveHandle()
    {
        SetMouseCapture(false);
        savedMousePosition = nullptr;
    }

    void NativeWindow::UpdateMousePosition(int x, int y)
    {
        if (savedMousePosition)
        {
            lastMousePosition = savedMousePosition;
        }
        else
        {
            lastMousePosition = MousePosition;
            Events::NativeEventManager::CursorEnterCallback(this);
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

    void NativeWindow::BringToFront()
    {
        SetForegroundWindow(hwnd);
        SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }

    void NativeWindow::FlashWindow()
    {
        FLASHWINFO flashInfo;
        flashInfo.cbSize = sizeof(FLASHWINFO);
        flashInfo.hwnd = hwnd;
        flashInfo.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
        flashInfo.uCount = 3;
        flashInfo.dwTimeout = 0;
        FlashWindowEx(&flashInfo);
    }

    void NativeWindow::SetTitle(String^ title)
    {
        SetWindowText(hwnd, msclr::interop::marshal_as<std::wstring>(title).c_str());
    }

    void NativeWindow::SetPosition(Vector2i^ pos)
    {
        SetWindowPos(hwnd, nullptr, pos->X, pos->Y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    }

    void NativeWindow::SetSize(Vector2i^ size)
    {
        SetWindowPos(hwnd, nullptr, 0, 0, size->X, size->Y, SWP_NOZORDER | SWP_NOMOVE);
    }

    void NativeWindow::SetPositionAndSize(Vector2i^ pos, Vector2i^ size)
    {
        SetWindowPos(hwnd, nullptr, pos->X, pos->Y, size->X, size->Y, SWP_NOZORDER);
    }

    void NativeWindow::Update()
    {
        UpdateWindow(hwnd);
    }

    DoubleRect^ NativeWindow::CurrentWindowRect::get()
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

    float NativeWindow::Opacity::get()
    {
        BYTE alpha = 0;
        DWORD flags = 0;
        if (GetLayeredWindowAttributes(hwnd, nullptr, &alpha, &flags) && flags & LWA_ALPHA)
        {
            return static_cast<float>(alpha) / 255.0f;
        }
        return 1.0f;
    }

    void NativeWindow::Opacity::set(float value)
    {
        value = std::min(std::max(value, 0.0f), 1.0f);
        BYTE alpha = static_cast<BYTE>(value * 255);
        SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
    }

    Vector2i^ NativeWindow::MousePosition::get()
    {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(hwnd, &point);
        return gcnew Vector2i(point.x, point.y);
    }

    void NativeWindow::MousePosition::set(Vector2i^ value)
    {
        POINT center = {static_cast<LONG>(value->X), static_cast<LONG>(value->Y)};
        ClientToScreen(hwnd, &center);
        SetCursorPos(center.x, center.y);
    }

    bool NativeWindow::IsMinimized::get()
    {
        return IsIconic(hwnd) && TRUE;
    }

    bool NativeWindow::IsMaximized::get()
    {
        return IsZoomed(hwnd) && TRUE;
    }

    void NativeWindow::SetVSync(UINT interval)
    {
        dxContext->VSync = interval;
    }

    void NativeWindow::MoveOnDisplay(NativeDisplayInfo^ monitorInfo)
    {
        DoubleRect^ rect = CurrentWindowRect;
        NativeDisplayInfo^ curMonitor = CurrentMonitor;

        int xRel = rect->Left - curMonitor->Position.X;
        int yRel = rect->Top - curMonitor->Position.Y;

        int x = monitorInfo->Position.X + xRel;
        int y = monitorInfo->Position.Y + yRel;

        SetPosition(gcnew Vector2i(x, y));
    }

    void NativeWindow::BeginFrame()
    {
        dxContext->SetWinRect(CurrentWindowRect);
        dxContext->BeginFrame();
    }

    void NativeWindow::EndFrame()
    {
        dxContext->EndFrame();
    }

    void NativeWindow::Present()
    {
        dxContext->Present();
    }

    void NativeWindow::Clear(Color4 color)
    {
        dxContext->Clear(color);
    }
}
