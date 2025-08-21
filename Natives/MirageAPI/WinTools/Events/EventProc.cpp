#include "pch.h"
#include "EventProc.h"

#include "EventManager.h"
#include "../Window/WindowClass.h"

namespace MirageAPI::Events
{
    // native logic

    LRESULT EventProc::QuantumProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (msg == WM_NCCREATE)
        {
            CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
            SetWindowLongPtrW(
                hwnd, GWLP_USERDATA,
                reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams)
            );

            return DefWindowProcW(hwnd, msg, wParam, lParam);
        }

        auto window = GetNativeWindow(hwnd);
        auto params = gcnew EventParams(wParam, lParam);

        if (window)
        {
            if (auto eventProc = window->Class->EventProc)
            {
                return eventProc->Proc(window, msg, params);
            }
        }
        return DefaultProc(window, msg, params);
    }

    Window^ EventProc::GetNativeWindow(HWND hwnd)
    {
        if (!hwnd) return nullptr;

        LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (ptr == 0) return nullptr;

        IntPtr managed_ptr(reinterpret_cast<void*>(ptr));
        auto gch = Runtime::InteropServices::GCHandle::FromIntPtr(managed_ptr);
        return safe_cast<Window^>(gch.Target);
    }

    // default proc

    void MatchMouseBtnMsg(UINT msg, WPARAM wParam, int& button, bool& pressed)
    {
        switch (msg)
        {
        case WM_LBUTTONDOWN:
            button = 1;
            pressed = true;
            break;
        case WM_LBUTTONUP:
            button = 1;
            pressed = false;
            break;
        case WM_RBUTTONDOWN:
            button = 2;
            pressed = true;
            break;
        case WM_RBUTTONUP:
            button = 2;
            pressed = false;
            break;
        case WM_MBUTTONDOWN:
            button = 3;
            pressed = true;
            break;
        case WM_MBUTTONUP:
            button = 3;
            pressed = false;
            break;
        case WM_XBUTTONDOWN:
            button = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? 4 : 5;
            pressed = true;
            break;
        case WM_XBUTTONUP:
            button = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? 4 : 5;
            pressed = false;
            break;
        default: ;
        }
    }

    void StartMouseTracking(Window^ window)
    {
        TRACKMOUSEEVENT tme = {};
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = window->NativeWindow;
        TrackMouseEvent(&tme);

        window->Mouse->isTracking = true;
        window->Mouse->isInWindow = true;
    }

    void StopMouseTracking(Window^ window)
    {
        window->Mouse->isTracking = false;
        window->Mouse->isInWindow = false;
    }

    LRESULT EventProc::DefaultProc(Window^ window, UINT eventType, EventParams^ params)
    {
        switch (eventType)
        {
        // mouse
        case WM_MOUSEMOVE:
            {
                if (window && !window->Mouse->isTracking)
                {
                    EventManager::MouseEnterCallback(window);
                    StartMouseTracking(window);
                }
                EventManager::MouseMoveCallback(window, params->lowL, params->highL);
                break;
            }
        case WM_MOUSELEAVE:
            {
                if (window && window->Mouse->isTracking)
                {
                    EventManager::MouseLeaveCallback(window);
                    StopMouseTracking(window);
                }
                break;
            }
        default: ;
        }

        // window required
        if (window)
        {
            switch (eventType)
            {
            // keyboard
            case WM_KEYDOWN:
            case WM_KEYUP:
                {
                    int mods = 0;

                    if (GetKeyState(VK_SHIFT) & 0x8000)
                        mods |= 1 << 0;
                    if (GetKeyState(VK_CONTROL) & 0x8000)
                        mods |= 1 << 1;
                    if (GetKeyState(VK_MENU) & 0x8000)
                        mods |= 1 << 2;

                    if (GetKeyState(VK_CAPITAL) & 0x0001)
                        mods |= 1 << 3;
                    if (GetKeyState(VK_NUMLOCK) & 0x0001)
                        mods |= 1 << 4;
                    if (GetKeyState(VK_SCROLL) & 0x0001)
                        mods |= 1 << 5;

                    EventManager::KeyCallback(
                        window,
                        static_cast<int>(params->wParam),
                        eventType == WM_KEYDOWN,
                        static_cast<int>(params->highL & 0xFF),
                        mods
                    );
                    break;
                }

            // mouse
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP:
            case WM_XBUTTONDOWN:
            case WM_XBUTTONUP:
                {
                    int button = 0;
                    bool pressed = false;

                    MatchMouseBtnMsg(eventType, params->wParam, button, pressed);

                    EventManager::MouseButtonCallback(
                        window,
                        button,
                        pressed,
                        static_cast<int>(params->wParam)
                    );
                    break;
                }
            // wheel
            case WM_MOUSEWHEEL:
                {
                    EventManager::MouseScrollCallback(window, params->highW);
                    break;
                }

            // window
            case WM_SIZE:
                {
                    EventManager::WindowSizeCallback(
                        window,
                        params->lowL, params->highL,
                        static_cast<int>(params->wParam),
                        false
                    );
                    break;
                }
            case WM_MOVE:
                {
                    EventManager::WindowMoveCallback(
                        window,
                        params->lowL, params->highL,
                        false
                    );
                    break;
                }
            case WM_DISPLAYCHANGE:
                {
                    EventManager::WindowDisplayCallback(window);
                    break;
                }
            // focus
            case WM_SETFOCUS:
            case WM_KILLFOCUS:
                {
                    EventManager::WindowFocusCallback(window, eventType == WM_SETFOCUS);
                    break;
                }
            // life cycle
            case WM_CREATE:
                {
                    EventManager::WindowCreateCallback(window);
                    break;
                }
            case WM_CLOSE:
                {
                    EventManager::WindowCloseCallback(window);
                    break;
                }
            case WM_DESTROY:
                {
                    EventManager::WindowDestroyCallback(window);
                    break;
                }

            // display
            case WM_DPICHANGED:
                {
                    RECT* rect = reinterpret_cast<RECT*>(params->lParam);
                    EventManager::WindowDpiCallback(
                        window, params->highL,
                        gcnew Rect(
                            rect->left,
                            rect->top,
                            rect->right - rect->left,
                            rect->bottom - rect->top
                        )
                    );
                    break;
                }
            // cursor
            case WM_SETCURSOR:
                {
                    if (params->lowL == HTCLIENT && window->Style->Cursor)
                    {
                        if (auto hCursor = window->Style->Cursor->NativeCursor)
                        {
                            SetCursor(hCursor);
                            return TRUE;
                        }
                    }
                    break;
                }
            // hit test = cursor in window?
            case WM_NCHITTEST:
                {
                    if (!window->Style->PixelTransparency) break;

                    auto point = window->Mouse->Position;
                    COLORREF color = GetPixel(
                        GetDC(window->NativeWindow),
                        UnpacVec2(point)
                    );

                    return (color & 0xFF000000) > 0 ? HTCLIENT : HTTRANSPARENT;
                }

            // menu
            case TRAY_MENU_EVENT_TYPE:
                {
                    if (params->lParam == WM_RBUTTONUP)
                    {
                        window->TrayMenu->Show(Mouse::PositionOnDisplay);
                    }
                    else if (params->lParam == WM_LBUTTONDOWN)
                    {
                        window->BringToFront();
                    }
                    break;
                }
            case WM_COMMAND:
                {
                    int commandId = params->lowW;
                    window->TrayMenu->RaiseCallback(commandId);
                    break;
                }
            case WM_SYSCOMMAND:
                {
                    int commandId = params->lowW & 0xFFF0;
                    if (commandId >= 0xF000) break;

                    window->SysMenu->RaiseCallback(commandId);
                    break;
                }
            default: ;
            }
        }

        return DefWindowProcW(window ? window->NativeWindow : nullptr, eventType, params->wParam, params->lParam);
    }

    // configurable proc

    long long EventProc::Proc(Window^ window, UINT eventType, EventParams^ params)
    {
        return DefaultProc(window, eventType, params);
    }
}
