#include "pch.h"
#include "NativeWindow.h"

#include <msclr/marshal_cppstd.h>
#include <d3d12.h>

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
        const int width, const int height,
        System::String^ title,
        NativeWindow^ parent
    )
    {
        if (width <= 0 || height <= 0)
        {
            throw gcnew System::ArgumentException("Invalid window size");
        }

        hwnd = CreateWindowEx(
            0, L"QuantumWindowClass",
            msclr::interop::marshal_as<std::wstring>(title).c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            width, height,
            parent ? parent->hwnd : nullptr,
            nullptr,
            hInstance, nullptr
        );

        gch = System::Runtime::InteropServices::GCHandle::Alloc(this);
        void* native_ptr = System::Runtime::InteropServices::GCHandle::ToIntPtr(gch).ToPointer();
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(native_ptr));

        dxContext = gcnew DirectX::DX12WindowContext(hwnd, width, height);

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
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

    System::IntPtr NativeWindow::Handle::get()
    {
        return System::IntPtr(hwnd);
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
