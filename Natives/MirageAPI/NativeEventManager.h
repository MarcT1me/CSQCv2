#pragma once

#include "NativeEvents.h"
#include "NativeWindow.h"

struct GLFWwindow;
struct GLFWmonitor;

namespace MirageAPI::Events
{
    // manager

    public ref class NativeEventManager
    {
        static Window::NativeWindow^ PrepareWindow(void* ptr);

        // key

        static void GLFW_KeyCallback(
            GLFWwindow* window,
            int key, int scancode,
            int action, int mods
        );

        // mouse

        static void GLFW_MouseButtonCallback(GLFWwindow* window, int button, int action, int mode);
        static void GLFW_ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
        static void GLFW_CursorPositionCallback(GLFWwindow* window, double xPos, double yPos);

        // window

        static void GLFW_WindowFocusedCallback(GLFWwindow* window, int focused);
        static void GLFW_WindowMaximizeCallback(GLFWwindow* window, int maximize);
        static void GLFW_WindowIconifyCallback(GLFWwindow* window, int iconify);
        static void GLFW_WindowResizeCallback(GLFWwindow* window, int width, int height);
        static void GLFW_WindowMoveCallback(GLFWwindow* window, int x, int y);
        static void GLFW_WindowCloseCallback(GLFWwindow* window);

        // other

        static void GLFW_CharCallback(GLFWwindow* window, unsigned int codepoint);
        static void GLFW_DropCallback(GLFWwindow* window, int count, const char** paths);

        // other global

        static void GLFW_JoystickCallback(int joystick, int event);
        static void GLFW_MonitorCallback(GLFWmonitor* monitor, int event);

    public:
        delegate void JoystickDelegate(NativeJoystickEvent  event);
        static event JoystickDelegate^ OnJoystick;
        
        delegate void JoystickStateDelegate(NativeJoystickState state);
        static event JoystickStateDelegate^ OnJoystickState;

        delegate void MonitorDelegate(NativeEvent event);
        static event MonitorDelegate^ OnMonitor;

        // Инициализация системы событий

        static void Initialize();

        static void PollEvents();
        static void PollJoystickStates();

    internal:
        static void InitializeCallbacks(GLFWwindow* window);
    };
}
