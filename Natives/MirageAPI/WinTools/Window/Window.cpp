#include "pch.h"
#include "Window.h"

#include "WindowClass.h"

namespace MirageAPI
{
    Window::Window(
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
    ) : wClass(wClass),
        type(parent ? WindowType::Child : wType),
        title(title),
        rect(rect),
        wStyle(style)
    {
        // positions (needs to check child)
        int x = rect->X;
        int y = rect->Y;
        if (parent && (x == USE_DEFAULT_POSITION || y == USE_DEFAULT_POSITION))
        {
            x = 0;
            y = 0;
        }

        gch = Runtime::InteropServices::GCHandle::Alloc(this);

        hwnd = CreateWindowExW(
            0, // idk whats this
            MAKEINTRESOURCEW(wClass->Name), // window class
            CStringToWChar(title), // window title
            static_cast<DWORD>(type), // style (window type)
            x, y, rect->Width, rect->Height, // position and size
            parent ? parent->hwnd : nullptr, // parent
            nullptr, // menu (can be changed in future)
            wClass->Instance->hInstance, // window instance
            Runtime::InteropServices::GCHandle::ToIntPtr(gch).ToPointer() // gc handler for window
        );
        CheckNull(hwnd)
        {
            gch.Free();
            throw gcnew NullptrException("Error creating Window, err: " + GetLastError());
        }
        wStyle->LinkToWindow(this);

        // other window data
        IsFullscreen = isFullscreen;
        Display = display;

        sysMenu = gcnew SystemMenu(this);
        mouse = gcnew MirageAPI::Mouse(this);
        dxContext = gcnew DirectX::DX12Context(hwnd, dxContextConfig);

        winList->Add(this);
    }

    Window::!Window()
    {
        if (winList->IndexOf(this))
            winList->Remove(this);

        SimpleDelete(dxContext);
        SimpleDelete(mouse);

        if (hwnd)
        {
            Destroy();
            hwnd = nullptr;
        }

        if (gch.IsAllocated)
        {
            gch.Free();
        }
    }

    void Window::WindowEventHandle(Events::WindowEvent^ event)
    {
        switch (event->Type)
        {
        case Events::EventType::WindowCreate:
        case Events::EventType::WindowDisplayChange:
            {
                DisplayManager::UpdateDisplayList();
                display = DisplayManager::GetDisplayFromHandle(DisplayHandle);
                break;
            }
        default: ;
        }
        OnWindow(event);
    }

    void Window::Destroy()
    {
        SafeDestroy(DestroyWindow, hwnd);
    }

    void Window::Title::set(String^ value)
    {
        title = value;
        SetWindowTextW(hwnd, CStringToWChar(value));
    }

    void Window::IsFullscreen::set(bool value)
    {
        if (type.HasFlag(WindowType::Child)) return;

        if (value && !isFullscreen)
        {
            rect = CurrentRect;
            wStyle->SetStyleForType(WindowType::Fullscreen);

            DisplayInfo^ displayInfo = Display;
            SetWindowPos(
                hwnd,
                HWND_TOPMOST,
                displayInfo->rect->X,
                displayInfo->rect->Y,
                displayInfo->rect->Width,
                displayInfo->rect->Height,
                SWP_FRAMECHANGED | SWP_SHOWWINDOW
            );
        }
        else if (isFullscreen && !value)
        {
            wStyle->SetStyleForType(type);

            SetWindowPos(
                hwnd,
                HWND_TOP,
                rect->X,
                rect->Y,
                rect->Width,
                rect->Height,
                SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOZORDER
            );
        }

        isFullscreen = value;
    }

    bool Window::IsHighContrastEnabled::get()
    {
                HIGHCONTRAST contrastInfo;
        contrastInfo.cbSize = sizeof(HIGHCONTRAST);
        return SystemParametersInfoW(
            SPI_GETHIGHCONTRAST, sizeof(HIGHCONTRAST),
            &contrastInfo, 0
        ) && contrastInfo.dwFlags & HCF_HIGHCONTRASTON;
    }

    Rect^ Window::CurrentRect::get()
    {
        RECT winRect;
        GetWindowRect(hwnd, &winRect);

        Rect^ rect = gcnew Rect();
        rect->X = winRect.left;
        rect->Y = winRect.top;
        rect->Width = winRect.right - winRect.left;
        rect->Height = winRect.bottom - winRect.top;
        return rect;
    }

    void Window::CurrentRect::set(Rect^ value)
    {
        CheckNull(value) return;

        if (isFullscreen)
            IsFullscreen = false;

        bool isPos = value->X != USE_DEFAULT_POSITION || value->Y != USE_DEFAULT_POSITION;
        bool isSize = value->Width != 0 || value->Height != 0;

        // all rect vectors
        if (isPos && isSize)
        {
            SetPositionAndSize(value->Pos, value->Size);
            rect = value;
            return;
        }
        // single vector
        if (isPos)
        {
            SetPosition(value->Pos);
            rect->Pos = value->Pos;
        }
        else if (isSize)
        {
            SetSize(value->Size);
            rect->Size = value->Size;
        }
    }

    void Window::Display::set(DisplayInfo^ value)
    {
        CheckNull(value) return;

        Rect^ rect = CurrentRect;
        DisplayInfo^ curMonitor = Display;

        int xRel = rect->Left - curMonitor->rect->X;
        int yRel = rect->Top - curMonitor->rect->Y;

        int x = value->rect->X + xRel;
        int y = value->rect->Y + yRel;

        SetPosition(gcnew Vector2i(x, y));
    }

    bool Window::IsEnabled::get()
    {
        return IsWindowEnabled(hwnd);
    }

    bool Window::IsVisible::get()
    {
        return IsWindowVisible(hwnd);
    }

    void Window::IsVisible::set(bool value)
    {
        if (value)
        {
            ShowWindow(hwnd, SW_SHOW);
        }
        else
        {
            ShowWindow(hwnd, SW_HIDE);
        }
    }

    bool Window::IsIcon::get()
    {
        return IsIconic(hwnd);
    }

    bool Window::IsMaximized::get()
    {
        return IsZoomed(hwnd);
    }

    void Window::IsMaximized::set(bool value)
    {
        if (value)
        {
            ShowWindow(hwnd, SW_MAXIMIZE);
        }
        else
        {
            ShowWindow(hwnd, SW_MINIMIZE);
        }
    }

    void Window::Restore()
    {
        ShowWindow(hwnd, SW_RESTORE);
    }

    void Window::BringToFront()
    {
        IsVisible = true;
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
        IsVisible = false;
        trayMenu->AddToTray();
    }

    void Window::ShowFromTray()
    {
        trayMenu->RemoveFromTray();
        BringToFront();
    }

    void Window::SetPosition(Vector2i^ pos)
    {
        SetWindowPos(hwnd, nullptr, UnpacVec2(pos), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    }

    void Window::SetSize(Vector2i^ size)
    {
        SetWindowPos(hwnd, nullptr, 0, 0, UnpacVec2(size), SWP_NOZORDER | SWP_NOMOVE);
    }

    void Window::SetPositionAndSize(Vector2i^ pos, Vector2i^ size)
    {
        SetWindowPos(hwnd, nullptr, UnpacVec2(pos), UnpacVec2(size), SWP_NOZORDER);
    }

    void Window::SetRegion(Vector2i^ size)
    {
        HRGN region = CreateRoundRectRgn(0, 0, rect->Width, rect->Height, UnpacVec2(size));
        SetWindowRgn(hwnd, region, true);
        DeleteObject(region);
    }

    void Window::Update()
    {
        UpdateWindow(hwnd);
    }

    void Window::FrameChanged()
    {
        SetWindowPos(
            hwnd, nullptr,
            0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
        ); // Frame Changed
    }

    void Window::Establish()
    {
        IsVisible = true;
        FrameChanged();
        Update();
    }

    void Window::BeginFrame()
    {
        dxContext->SetWinRect(CurrentRect);
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
