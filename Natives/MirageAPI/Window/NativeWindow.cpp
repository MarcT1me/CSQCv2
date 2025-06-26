#include "pch.h"
#include "NativeWindow.h"

#include <msclr/marshal_cppstd.h>

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
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = reinterpret_cast<WNDPROC>(Events::NativeEventProc::QuantumWindowProc);
        wc.hInstance = GetModuleHandle(nullptr);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = L"QuantumWindowClass";
        RegisterClassEx(&wc);

        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    }

    NativeWindow::NativeWindow(
        WindowRect^ rect,
        System::String^ title,
        float opacity,
        NativeWindow^ parent,
        WindowType wType,
        DirectX::DX12WindowContextConfig^ dxConfig
    )
    {
        hwnd = CreateWindowEx(
            0, L"QuantumWindowClass",
            msclr::interop::marshal_as<std::wstring>(title).c_str(),
            (parent ? WS_CHILDWINDOW : static_cast<unsigned long>(wType)) | WS_EX_LAYERED,
            rect->x, rect->y,
            rect->width, rect->height,
            parent ? parent->hwnd : nullptr,
            nullptr,
            hInstance, nullptr
        );
        SetOpacity(opacity);

        gch = System::Runtime::InteropServices::GCHandle::Alloc(this);
        void* native_ptr = System::Runtime::InteropServices::GCHandle::ToIntPtr(gch).ToPointer();
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(native_ptr));

        dxContext = gcnew DirectX::DX12WindowContext(hwnd, rect->width, rect->height, dxConfig);
    }

    NativeWindow::~NativeWindow()
    {
        delete dxContext;

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

    NativeWindow::!NativeWindow()
    {
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
        dxContext->Resize(width, height);
    }

    void NativeWindow::SetPositionAndSize(int x, int y, int width, int height)
    {
        SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER);
        dxContext->Resize(width, height);
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
    
    WindowRect NativeWindow::Rect::get()
    {
        RECT winApiRect;
        GetWindowRect(hwnd, &winApiRect);

        WindowRect rect;
        rect.left = winApiRect.left;
        rect.right = winApiRect.right;
        rect.top = winApiRect.top;
        rect.bottom = winApiRect.bottom;
        
        rect.x = winApiRect.left;
        rect.y = winApiRect.top;
        rect.width = winApiRect.right - rect.left;
        rect.height = winApiRect.bottom - rect.top;
        return rect;
    }

    float NativeWindow::Opacity::get()
    {
        BYTE alpha = 0;
        DWORD flags = 0;
        if (GetLayeredWindowAttributes(hwnd, nullptr, &alpha, &flags) && 
            (flags & LWA_ALPHA))
        {
            return static_cast<float>(alpha) / 255.0f;
        }
        return 1.0f; // Полностью непрозрачное по умолчанию
    }

    bool NativeWindow::IsMinimized::get()
    {
        return IsIconic(hwnd) != FALSE;
    }

    bool NativeWindow::IsMaximized::get()
    {
        return IsZoomed(hwnd) != FALSE;
    }

    void NativeWindow::HandleResize(int width, int height)
    {
        dxContext->Resize(width, height);
    }

    void NativeWindow::SetVSync(bool enabled)
    {
        dxContext->VSync = enabled ? 1 : 0;
    }

    void NativeWindow::BeginFrame()
    {
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
