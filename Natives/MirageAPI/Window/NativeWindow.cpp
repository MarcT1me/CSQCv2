#include "pch.h"
#include "NativeWindow.h"

#include "../Events/NativeEventProc.h"

namespace MirageAPI::Window
{
    // event rises
    void NativeWindow::RaiseKeyEvent(Events::NativeKeyEvent event)
    {
        OnKey(event);
    }

    void NativeWindow::RaiseMouseEvent(Events::NativeMouseEvent event)
    {
        OnMouse(event);
    }

    void NativeWindow::RaiseWindowEvent(Events::NativeWindowEvent event)
    {
        OnWindow(event);
    }

    void NativeWindow::RaiseCharEvent(Events::NativeCharEvent event)
    {
        OnChar(event);
    }

    void NativeWindow::RaiseDropEvent(Events::NativeDropEvent event)
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
        System::String^ title,
        float opacity,
        bool isFullscreen,
        NativeWindow^ parent,
        WindowType wType,
        DirectX::DX12ContextConfig^ dxContextConfig
    )
    {
        SavedRect = DoubleRect::FromSimple(rect);
        SavedStyle = (parent ? WS_CHILDWINDOW : static_cast<unsigned long>(wType)) | WS_EX_LAYERED;

        hwnd = CreateWindowEx(
            0, L"QWindow",
            msclr::interop::marshal_as<std::wstring>(title).c_str(),
            SavedStyle,
            SavedRect->X, SavedRect->Y,
            SavedRect->Width, SavedRect->Height,
            parent ? parent->hwnd : nullptr,
            nullptr, hInstance, nullptr
        );
        SetOpacity(opacity);
        if (isFullscreen)
            ToggleFullscreen();

        {
            gch = System::Runtime::InteropServices::GCHandle::Alloc(this);
            void* native_ptr = System::Runtime::InteropServices::GCHandle::ToIntPtr(gch).ToPointer();
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
        if (!IsFullscreen)
        {
            SavedRect = CurrentWindowRect;
            DoubleRect^ screenRect = CurrentMonitorRect;

            SetWindowLongPtr(
                hwnd, GWL_STYLE,
                SavedStyle & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU)
            );

            SetWindowPos(
                hwnd,
                HWND_TOP,
                screenRect->X, screenRect->Y,
                screenRect->Width, screenRect->Height,
                SWP_FRAMECHANGED | SWP_SHOWWINDOW
            );

            IsFullscreen = true;
        }
        else
        {
            SetWindowLongPtr(
                hwnd, GWL_STYLE,
                SavedStyle
            );

            SetWindowPos(
                hwnd,
                nullptr,
                SavedRect->X,
                SavedRect->Y,
                SavedRect->Width,
                SavedRect->Height,
                SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOZORDER
            );

            IsFullscreen = false;
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

    void NativeWindow::SetTitle(System::String^ title)
    {
        SetWindowText(hwnd, msclr::interop::marshal_as<std::wstring>(title).c_str());
    }

    void NativeWindow::SetPosition(int x, int y)
    {
        SetWindowPos(hwnd, nullptr, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    }

    void NativeWindow::SetSize(int width, int height)
    {
        SetWindowPos(hwnd, nullptr, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
    }

    void NativeWindow::SetPositionAndSize(int x, int y, int width, int height)
    {
        SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER);
    }

    void NativeWindow::SetOpacity(float opacity)
    {
        opacity = std::min(std::max(opacity, 0.0f), 1.0f);
        BYTE alpha = static_cast<BYTE>(opacity * 255);
        SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
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

    DoubleRect^ NativeWindow::CurrentMonitorRect::get()
    {
        MONITORINFO monitorInfo;
        monitorInfo.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(CurrentMonitor, &monitorInfo);
        RECT screenRect = monitorInfo.rcMonitor;

        DoubleRect^ rect = gcnew DoubleRect();
        rect->Left = screenRect.left;
        rect->Right = screenRect.right;
        rect->Top = screenRect.top;
        rect->Bottom = screenRect.bottom;

        rect->X = screenRect.left;
        rect->Y = screenRect.top;
        rect->Width = screenRect.right - screenRect.left;
        rect->Height = screenRect.bottom - screenRect.top;
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

    void NativeWindow::Clear(float r, float g, float b, float a)
    {
        dxContext->Clear(r, g, b, a);
    }
}
