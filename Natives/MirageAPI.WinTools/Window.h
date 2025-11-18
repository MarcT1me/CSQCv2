#pragma once
#include <string>

#include "Rect.h"

namespace MirageAPI::Native::WinTools
{
    class Mouse;
    class Display;

    enum class WindowType
    {
        None = 0,
        Borderless = 1,
        Decorated = 2,
        Resizable = 4,
        Floating = 8,
        Transparent = 16,

        Normal = Decorated | Resizable | 32,
        Popup = Floating | Transparent | 64,
    };

    enum class FullscreenMode
    {
        None = 0,
        Borderless = 1,
        Desktop = 2,
        DesktopFull = 2,
    };

    class Window
    {
        GLFWwindow* _window = nullptr;

        Mouse* _mouse = nullptr;
        Display* _display = nullptr;

        std::string _title;
        WindowType _windowType;
        Rect* _rect = nullptr;
        FullscreenMode _fullscreenMode = FullscreenMode::None;

    public:
        Window(std::string title, WindowType windowType, Rect* rect, Display* display = nullptr);
        Window(std::string title, WindowType windowType, FullscreenMode fullscreenMode, Display* display = nullptr);
        ~Window();

        void setFullscreenMode(FullscreenMode mode, Display* display = nullptr);

        GLFWwindow* getHandle() const { return _window; }
        
        Mouse* getMouse() const { return _mouse; }
        Display* getDisplay() const { return _display; }
        void setDisplay(Display* display);
        
        std::string getTitle() const { return _title; }
        void setTitle(std::string title);
        Rect* getRect();
        void setRect(Rect* rect);

        WindowType getWindowType() const { return _windowType; }
        FullscreenMode getFullscreenMode() const { return _fullscreenMode; }
        
        void establish();
        void close();
        
        void show();
        void hide();
        bool isVisible();
    };
}
