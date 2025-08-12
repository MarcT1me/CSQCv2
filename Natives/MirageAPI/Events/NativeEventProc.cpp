#include "pch.h"
#include "NativeEventProc.h"

#include "NativeEventManager.h"

namespace MirageAPI::Events
{
    Window::NativeWindow^ NativeEventProc::GetNativeWindow(HWND hWnd)
    {
        LONG_PTR ptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (ptr == 0) return nullptr;

        IntPtr managed_ptr(reinterpret_cast<void*>(ptr));
        auto gch = Runtime::InteropServices::GCHandle::FromIntPtr(managed_ptr);
        return safe_cast<Window::NativeWindow^>(gch.Target);
    }

    void matchMouseBtnMsg(UINT msg, int* button, bool* pressed)
    {
        switch (msg)
        {
        case WM_LBUTTONDOWN:
            *button = 1;
            *pressed = true;
            break;
        case WM_LBUTTONUP:
            *button = 1;
            *pressed = false;
            break;

        case WM_MBUTTONDOWN:
            *button = 2;
            *pressed = true;
            break;
        case WM_MBUTTONUP:
            *button = 2;
            *pressed = false;
            break;

        case WM_RBUTTONDOWN:
            *button = 3;
            *pressed = true;
            break;
        case WM_RBUTTONUP:
            *button = 3;
            *pressed = false;
            break;

        default:
            *button = -1;
            *pressed = false;
            break;
        }
    }

    LRESULT NativeEventProc::QuantumWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (Window::NativeWindow^ window = GetNativeWindow(hwnd))
        {
            switch (msg)
            {
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

                    NativeEventManager::KeyCallback(
                        window,
                        static_cast<int>(wParam),
                        msg == WM_KEYDOWN,
                        static_cast<int>(lParam >> 16 & 0xFF),
                        mods
                    );
                    break;
                }

            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_MBUTTONUP:
            case WM_LBUTTONUP:
                {
                    int button;
                    bool pressed;
                    matchMouseBtnMsg(msg, &button, &pressed);

                    NativeEventManager::MouseButtonCallback(
                        window,
                        button,
                        pressed,
                        static_cast<int>(wParam)
                    );
                    break;
                }

            case WM_MOUSEWHEEL:
                {
                    int x = LOWORD(lParam);
                    int y = HIWORD(lParam);

                    NativeEventManager::ScrollCallback(window, x, y);
                    break;
                }
            case WM_MOUSEMOVE:
                {
                    int x = LOWORD(lParam);
                    int y = HIWORD(lParam);

                    window->UpdateMousePosition(x, y);

                    NativeEventManager::CursorPositionCallback(window, x, y);
                    break;
                }

            case WM_SIZE:
                {
                    int width = LOWORD(lParam);
                    int height = HIWORD(lParam);

                    NativeEventManager::WindowResizeCallback(window, width, height);
                    break;
                }
            case WM_MOVE:
                {
                    int x = LOWORD(lParam);
                    int y = HIWORD(lParam);

                    NativeEventManager::WindowMoveCallback(window, x, y);
                    break;
                }

            case WM_DESTROY:
                {
                    PostQuitMessage(0);
                    NativeEventManager::WindowCloseCallback(window);
                    return 0;
                }
            default: ;
            }
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}
