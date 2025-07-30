#include "pch.h"
#include "NativeEventProc.h"

#include "NativeEventManager.h"

namespace MirageAPI::Events
{
    Window::NativeWindow^ NativeEventProc::GetNativeWindow(HWND hWnd)
    {
        LONG_PTR ptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (ptr == 0) return nullptr;

        System::IntPtr managed_ptr(reinterpret_cast<void*>(ptr));
        auto gch = System::Runtime::InteropServices::GCHandle::FromIntPtr(managed_ptr);
        return safe_cast<Window::NativeWindow^>(gch.Target);
    }

    void matchMouseBtnMsg(UINT msg, int* button, int* action)
    {
        switch (msg)
        {
        case WM_LBUTTONDOWN:
            *button = 1;
            *action = 1;
            break;
        case WM_LBUTTONUP:
            *button = 1;
            *action = 0;
            break;

        case WM_MBUTTONDOWN:
            *button = 2;
            *action = 1;
            break;
        case WM_MBUTTONUP:
            *button = 2;
            *action = 0;
            break;

        case WM_RBUTTONDOWN:
            *button = 3;
            *action = 1;
            break;
        case WM_RBUTTONUP:
            *button = 3;
            *action = 0;
            break;
            
        default:
            *button = -1;
            *action = -1;
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
                    const int scancode = lParam >> 16 & 0xFF;

                    // Получаем модификаторы
                    int mods = 0;
                    if (GetKeyState(VK_SHIFT) & 0x8000) mods |= 0x0001;
                    if (GetKeyState(VK_CONTROL) & 0x8000) mods |= 0x0002;
                    if (GetKeyState(VK_MENU) & 0x8000) mods |= 0x0004;
                    if (GetKeyState(VK_CAPITAL) & 0x0001) mods |= 0x0010;

                    NativeEventManager::KeyCallback(
                        window,
                        static_cast<int>(wParam),
                        scancode,
                        msg == WM_KEYDOWN ? 1 : 0, // action: 1=press, 0=release
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
                    int mods = 0;
                    if (wParam & MK_CONTROL) mods |= 0x0002;
                    if (wParam & MK_SHIFT) mods |= 0x0001;

                    int button;
                    int action;
                    matchMouseBtnMsg(msg, &button, &action);

                    NativeEventManager::MouseButtonCallback(
                        window,
                        button,
                        action,
                        mods
                    );
                    break;
                }

            case WM_MOUSEWHEEL:
                {
                    NativeEventManager::ScrollCallback(
                        window,
                        LOWORD(lParam),
                        HIWORD(lParam)
                    );
                    break;
                }
            case WM_MOUSEMOVE:
                {
                    NativeEventManager::CursorPositionCallback(
                        window,
                        LOWORD(lParam),
                        HIWORD(lParam)
                    );
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
                    NativeEventManager::WindowCloseCallback(
                        window
                    );
                    PostQuitMessage(0);
                    return 0;
                }
            default: ;
            }
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}
