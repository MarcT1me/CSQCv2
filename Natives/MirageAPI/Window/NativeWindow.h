#pragma once

#include "../Events/NativeEvents.h"

#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace MirageAPI::Window
{
    public ref class NativeWindow
    {
        GLFWwindow* glfw_window;
        System::Runtime::InteropServices::GCHandle gch;
        VkSurfaceKHR surface = VK_NULL_HANDLE;

    internal:
        // rise methods (overhead...)
        void RaiseKeyEvent(Events::NativeKeyEvent event);
        void RaiseMouseEvent(Events::NativeMouseEvent event);
        void RaiseWindowEvent(Events::NativeWindowEvent event);
        void RaiseCharEvent(Events::NativeCharEvent event);
        void RaiseDropEvent(Events::NativeDropEvent event);

    public:
        // Keyboard

        delegate void KeyDelegate(Events::NativeKeyEvent event);
        event KeyDelegate^ OnKey;

        // Mouse

        delegate void MouseEventDelegate(Events::NativeMouseEvent event);
        event MouseEventDelegate^ OnMouse;

        // Window events

        delegate void WindowEventDelegate(Events::NativeWindowEvent event);
        event WindowEventDelegate^ OnWindow;

        // other

        delegate void CharDelegate(Events::NativeCharEvent event);
        event CharDelegate^ OnChar;

        delegate void DropDelegate(Events::NativeDropEvent event);
        event DropDelegate^ OnDrop;

        // initializations and property

        NativeWindow(
            int width, int height,
            System::String^ title,
            NativeWindow^ parent
        );
        ~NativeWindow();
        !NativeWindow();

        property System::IntPtr Handle
        {
            System::IntPtr get();
        }

        property VkSurfaceKHR VulkanSurface
        {
            VkSurfaceKHR get() { return surface; }
        }

        void CreateVulkanSurface(VkInstance instance);
        void CleanupVulkanSurface(VkInstance instance);


        static void SetVSync(bool enabled);
        static void SetViewport(int x, int y, int width, int height);
        static void Clear(float r, float g, float b, float a);

        // other methods
        void MakeCurrent();
        void SwapBuffers();
        bool ShouldClose();

        void SetSize(int width, int height);
        void SetSizeLimit(int minWidth, int minHeight, int maxWidth, int maxHeight);
        void SetPos(int xPos, int yPos);
        void SetOpacity(float opacity);

        void Focus();
        void Show();
        void Hide();
        void Maximize();
        void Restore();
    };
}
