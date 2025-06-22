#include "pch.h"
#include "NativeEventManager.h"

#include "GLFW/glfw3.h"

#include "../Window/NativeWindow.h"

namespace MirageAPI::Events
{
    Window::NativeWindow^ NativeEventManager::PrepareWindow(void* ptr)
    {
        auto handle = System::Runtime::InteropServices::GCHandle::FromIntPtr(System::IntPtr(ptr));
        return safe_cast<Window::NativeWindow^>(handle.Target);
    }

    // key

    void NativeEventManager::GLFW_KeyCallback(
        GLFWwindow* window,
        const int key,
        const int scancode,
        const int action, const int mods
    )
    {
        // getting window ptr
        void* ptr = glfwGetWindowUserPointer(window);
        if (!ptr) return;

        // format event
        NativeKeyEvent e;

        e.Key = key;
        e.Scancode = scancode;
        e.Action = action;
        e.Mods = mods;

        // raise event
        auto wrapper = PrepareWindow(ptr);
        e.windowID = wrapper->Handle;
        wrapper->RaiseKeyEvent(e);
    }

    // mouse

    void NativeEventManager::GLFW_MouseButtonCallback(
        GLFWwindow* window,
        const int button,
        const int action, const int mode
    )
    {
        // getting window ptr
        void* ptr = glfwGetWindowUserPointer(window);
        if (!ptr) return;

        // format event
        NativeMouseEvent e;
        e.Type = NativeMouseEventType::Button;

        e.Button = button;
        e.Action = action;
        e.Mode = mode;

        // raise event
        auto wrapper = PrepareWindow(ptr);
        e.windowID = wrapper->Handle;
        wrapper->RaiseMouseEvent(e);
    }

    void NativeEventManager::GLFW_ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
    {
        // getting window ptr
        void* ptr = glfwGetWindowUserPointer(window);
        if (!ptr) return;

        // format event
        NativeMouseEvent e;
        e.Type = NativeMouseEventType::Scroll;

        e.X = xOffset;
        e.Y = yOffset;

        // raise event
        auto wrapper = PrepareWindow(ptr);
        e.windowID = wrapper->Handle;
        wrapper->RaiseMouseEvent(e);
    }

    void NativeEventManager::GLFW_CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
    {
        // getting window ptr
        void* ptr = glfwGetWindowUserPointer(window);
        if (!ptr) return;

        // format event
        NativeMouseEvent e;
        e.Type = NativeMouseEventType::Move;

        e.X = xPos;
        e.Y = yPos;

        // raise event
        auto wrapper = PrepareWindow(ptr);
        e.windowID = wrapper->Handle;
        wrapper->RaiseMouseEvent(e);
    }

    // window

    void NativeEventManager::GLFW_WindowFocusedCallback(GLFWwindow* window, int focused)
    {
        // getting window ptr
        void* ptr = glfwGetWindowUserPointer(window);
        if (!ptr) return;

        // format event
        NativeWindowEvent e;
        e.Type = NativeWindowEventType::Focus;

        e.X = focused;

        // raise event
        auto wrapper = PrepareWindow(ptr);
        e.windowID = wrapper->Handle;
        wrapper->RaiseWindowEvent(e);
    }

    void NativeEventManager::GLFW_WindowMaximizeCallback(GLFWwindow* window, int maximize)
    {
        // getting window ptr
        void* ptr = glfwGetWindowUserPointer(window);
        if (!ptr) return;

        // format event
        NativeWindowEvent e;
        e.Type = NativeWindowEventType::Maximize;

        e.X = maximize;

        // raise event
        auto wrapper = PrepareWindow(ptr);

        wrapper->RaiseWindowEvent(e);
    }

    void NativeEventManager::GLFW_WindowIconifyCallback(GLFWwindow* window, int iconify)
    {
        // getting window ptr
        void* ptr = glfwGetWindowUserPointer(window);
        if (!ptr) return;

        // format event
        NativeWindowEvent e;
        e.Type = NativeWindowEventType::Iconify;

        e.X = iconify;

        // raise event
        auto wrapper = PrepareWindow(ptr);
        e.windowID = wrapper->Handle;
        wrapper->RaiseWindowEvent(e);
    }

    void NativeEventManager::GLFW_WindowResizeCallback(GLFWwindow* window, const int width, const int height)
    {
        // getting window ptr
        void* ptr = glfwGetWindowUserPointer(window);
        if (!ptr) return;

        // format event
        NativeWindowEvent e;
        e.Type = NativeWindowEventType::Resize;

        e.X = width;
        e.Y = height;

        // raise event
        auto wrapper = PrepareWindow(ptr);
        e.windowID = wrapper->Handle;
        wrapper->RaiseWindowEvent(e);
    }

    void NativeEventManager::GLFW_WindowMoveCallback(GLFWwindow* window, int x, int y)
    {
        // getting window ptr
        void* ptr = glfwGetWindowUserPointer(window);
        if (!ptr) return;

        // format event
        NativeWindowEvent e;
        e.Type = NativeWindowEventType::Move;

        e.X = x;
        e.Y = y;

        // raise event
        auto wrapper = PrepareWindow(ptr);
        e.windowID = wrapper->Handle;
        wrapper->RaiseWindowEvent(e);
    }

    void NativeEventManager::GLFW_WindowRefreshCallback(GLFWwindow* window)
    {
        // getting window ptr
        void* ptr = glfwGetWindowUserPointer(window);
        if (!ptr) return;

        // format event
        NativeWindowEvent e;
        e.Type = NativeWindowEventType::Refresh;

        // raise event
        auto wrapper = PrepareWindow(ptr);
        e.windowID = wrapper->Handle;
        wrapper->RaiseWindowEvent(e);
    }

    void NativeEventManager::GLFW_WindowCloseCallback(GLFWwindow* window)
    {
        // getting window ptr
        void* ptr = glfwGetWindowUserPointer(window);
        if (!ptr) return;

        // format event
        NativeWindowEvent e;
        e.Type = NativeWindowEventType::Close;

        // raise event
        auto wrapper = PrepareWindow(ptr);
        e.windowID = wrapper->Handle;
        wrapper->RaiseWindowEvent(e);
    }

    // other

    void NativeEventManager::GLFW_CharCallback(GLFWwindow* window, unsigned int codepoint)
    {
        // getting window ptr
        void* ptr = glfwGetWindowUserPointer(window);
        if (!ptr) return;

        // format event
        NativeCharEvent e;

        e.codepoint = codepoint;

        // raise event
        auto wrapper = PrepareWindow(ptr);
        e.windowID = wrapper->Handle;
        wrapper->RaiseCharEvent(e);
    }

    void NativeEventManager::GLFW_DropCallback(GLFWwindow* window, int count, const char** paths)
    {
        // getting window ptr
        void* ptr = glfwGetWindowUserPointer(window);
        if (!ptr) return;

        // format event
        NativeDropEvent e;

        e.count = count;
        e.paths = paths;

        // raise event
        auto wrapper = PrepareWindow(ptr);
        e.windowID = wrapper->Handle;
        wrapper->RaiseDropEvent(e);
    }

    // other global

    void NativeEventManager::GLFW_JoystickCallback(int joystick, int event)
    {
        NativeJoystickEvent e;
        e.JoystickID = joystick;
        e.Connected = event == GLFW_CONNECTED;
        OnJoystick(e);
    }

    void NativeEventManager::GLFW_MonitorCallback(GLFWmonitor* monitor, int event)
    {
        NativeEvent e;
        e.Type = NativeEventType::Monitor;
        e.data = System::IntPtr(monitor); // Безопасное приведение
        e.event = event;
        OnMonitor(e);
    }

    // initialize

    void NativeEventManager::Initialize()
    {
        glfwSetJoystickCallback(reinterpret_cast<GLFWjoystickfun>(GLFW_JoystickCallback));
        glfwSetMonitorCallback(reinterpret_cast<GLFWmonitorfun>(GLFW_MonitorCallback));
    }

    void NativeEventManager::InitializeCallbacks(GLFWwindow* window)
    {
        // keyboard
        glfwSetKeyCallback(window, reinterpret_cast<GLFWkeyfun>(GLFW_KeyCallback));

        // mouse
        glfwSetMouseButtonCallback(window, reinterpret_cast<GLFWmousebuttonfun>(GLFW_MouseButtonCallback));
        glfwSetScrollCallback(window, reinterpret_cast<GLFWscrollfun>(GLFW_ScrollCallback));
        glfwSetCursorPosCallback(window, reinterpret_cast<GLFWcursorposfun>(GLFW_CursorPositionCallback));

        // window
        glfwSetWindowFocusCallback(window, reinterpret_cast<GLFWwindowfocusfun>(GLFW_WindowFocusedCallback));
        glfwSetWindowMaximizeCallback(window, reinterpret_cast<GLFWwindowmaximizefun>(GLFW_WindowMaximizeCallback));
        glfwSetWindowIconifyCallback(window, reinterpret_cast<GLFWwindowiconifyfun>(GLFW_WindowIconifyCallback));
        glfwSetWindowSizeCallback(window, reinterpret_cast<GLFWwindowsizefun>(GLFW_WindowResizeCallback));
        glfwSetWindowPosCallback(window, reinterpret_cast<GLFWwindowposfun>(GLFW_WindowMoveCallback));
        glfwSetWindowRefreshCallback(window, reinterpret_cast<GLFWwindowrefreshfun>(GLFW_WindowRefreshCallback));
        glfwSetWindowCloseCallback(window, reinterpret_cast<GLFWwindowclosefun>(GLFW_WindowCloseCallback));

        // other
        glfwSetCharCallback(window, reinterpret_cast<GLFWcharfun>(GLFW_CharCallback));
        glfwSetDropCallback(window, reinterpret_cast<GLFWdropfun>(GLFW_DropCallback));
    }

    // other methods

    void NativeEventManager::PollEvents()
    {
        glfwPollEvents();
    }

    void NativeEventManager::PollJoystickStates()
    {
        for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; ++jid)
        {
            if (glfwJoystickPresent(jid))
            {
                NativeJoystickState state;
                state.JoystickID = jid;

                // Получаем оси
                state.Axes = glfwGetJoystickAxes(jid, &state.AxesCount);

                // Получаем кнопки
                state.Buttons = glfwGetJoystickButtons(jid, &state.ButtonCount);

                // получаем шляпы
                state.Hats = glfwGetJoystickHats(jid, &state.HatCount);

                OnJoystickState(state);
            }
        }
    }

    void NativeEventManager::InitializeJoysticks()
    {
        for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++)
        {
            if (glfwJoystickPresent(jid))
            {
                // Генерируем событие подключения
                GLFW_JoystickCallback(jid, GLFW_CONNECTED);
            }
        }
    }

    int NativeEventManager::GetJoystickMaxCount()
    {
        return GLFW_JOYSTICK_LAST + 1;
    }

    bool NativeEventManager::IsJoystickPresent(int jid)
    {
        return glfwJoystickPresent(jid) == GLFW_TRUE;
    }

    System::String^ NativeEventManager::GetJoystickName(int jid)
    {
        const char* name = glfwGetJoystickName(jid);
        return gcnew System::String(name ? name : "Unknown");
    }

    bool NativeEventManager::JoystickIsGamepad(int jid)
    {
        return glfwJoystickIsGamepad(jid) == 1;
    }

    System::String^ NativeEventManager::GetJoystickGuid(int jid)
    {
        const char* name = glfwGetJoystickGUID(jid);
        return gcnew System::String(name ? name : "");
    }
}
