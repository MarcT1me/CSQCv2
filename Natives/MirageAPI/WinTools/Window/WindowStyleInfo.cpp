#include "pch.h"
#include "WindowStyleInfo.h"

#include <dwmapi.h>

#include "Window.h"
#include "WindowClass.h"
#include "../Events/EventManager.h"
#include "../Icon/IconInfo.h"
#include "../Cursor/CursorInfo.h"

namespace MirageAPI
{
    WindowStyleInfo::WindowStyleInfo(
        Byte opacity,
        bool isDarkMode,
        bool pixelTransparency,
        bool isAcrylic,
        Color4^ borderColor,
        Color4^ captionColor,
        IconInfo^ icon,
        CursorInfo^ cursor
    ) : opacity(opacity),
        isDarkMode(isDarkMode),
        pixelTransparency(pixelTransparency),
        isAcrylic(isAcrylic),
        borderColor(borderColor),
        captionColor(captionColor),
        icon(icon),
        cursor(cursor)
    {
    }

    WindowStyleInfo^ WindowStyleInfo::LinkToWindow(Window^ window)
    {
        this->window = window;

        DarkMode = this->isDarkMode;
        PixelTransparency = this->pixelTransparency;

        if (this->borderColor)
        {
            BorderColor = this->borderColor;
        }
        {
            COLORREF borderColor;
            DwmGetWindowAttribute(
                window->NativeWindow, DWMWA_BORDER_COLOR,
                &borderColor, sizeof(borderColor)
            );
            this->borderColor = Color4(
                static_cast<float>(borderColor & 0x00ff0000),
                static_cast<float>(borderColor & 0x0000ff00),
                static_cast<float>(borderColor & 0x000000ff),
                static_cast<float>(borderColor & 0xff000000)
            );
        }

        if (this->captionColor)
        {
            CaptionColor = this->captionColor;
        }
        else
        {
            COLORREF captionColor;
            DwmGetWindowAttribute(
                window->NativeWindow, DWMWA_CAPTION_COLOR,
                &captionColor, sizeof(captionColor)
            );
            this->captionColor = Color4(
                static_cast<float>(captionColor & 0x00ff0000),
                static_cast<float>(captionColor & 0x0000ff00),
                static_cast<float>(captionColor & 0x000000ff),
                static_cast<float>(captionColor & 0xff000000)
            );
        }

        Opacity = this->opacity;

        // info classes
        Icon = this->icon ? this->icon : window->Class->Icon;
        Cursor = this->cursor ? this->cursor : window->Class->Cursor;

        return this;
    }

    void WindowStyleInfo::SetStyleForType(WindowType wType)
    {
        SetWindowLongPtrW(
            window->NativeWindow, GWL_STYLE,
            static_cast<DWORD>(wType)
        ); // Apply style
        window->FrameChanged();
    }

    void WindowStyleInfo::PixelTransparency::set(bool value)
    {
        pixelTransparency = value;
        if (value)
        {
            SetWindowLongW(
                window->NativeWindow, GWL_EXSTYLE,
                GetWindowLongW(window->NativeWindow, GWL_EXSTYLE) | WS_EX_LAYERED
            );
            int o = Opacity;
            Opacity = 255;
            opacity = o;
        }
        else
        {
            SetWindowLong(
                window->NativeWindow, GWL_EXSTYLE,
                GetWindowLongW(window->NativeWindow, GWL_EXSTYLE) & ~WS_EX_LAYERED
            );
            Opacity = opacity;
        }
    }

    void WindowStyleInfo::Opacity::set(Byte value)
    {
        opacity = value;
        SetLayeredWindowAttributes(
            window->NativeWindow, 0, value, LWA_ALPHA
        );
    }

    void WindowStyleInfo::DarkMode::set(bool value)
    {
        isDarkMode = value;
        DwmSetWindowAttribute(
            window->NativeWindow, DWMWA_USE_IMMERSIVE_DARK_MODE,
            &value, sizeof(value)
        );
    }

    void WindowStyleInfo::BorderColor::set(Color4^ value)
    {
        borderColor = value;
        COLORREF color = value->ToArgb();
        DwmSetWindowAttribute(
            window->NativeWindow, DWMWA_BORDER_COLOR,
            &color, sizeof(color)
        );
    }

    void WindowStyleInfo::CaptionColor::set(Color4^ value)
    {
        captionColor = value;
        COLORREF color = value->ToArgb();
        DwmSetWindowAttribute(
            window->NativeWindow, DWMWA_CAPTION_COLOR,
            &color, sizeof(color)
        );
    }

    void WindowStyleInfo::Icon::set(IconInfo^ value)
    {
        icon = value;

        CheckNull(value) return;

        Events::EventManager::SendEvent(
            window, WM_SETICON,
            ICON_SMALL,
            reinterpret_cast<LPARAM>(value->NativeIcon)
        );
        Events::EventManager::SendEvent(
            window, WM_SETICON,
            ICON_BIG,
            reinterpret_cast<LPARAM>(value->NativeIcon)
        );
    }

    void WindowStyleInfo::Cursor::set(CursorInfo^ value)
    {
        cursor = value;

        CheckNull(value) return;

        Events::EventManager::SendEvent(
            window, WM_SETCURSOR,
            reinterpret_cast<WPARAM>(window->NativeWindow),
            MAKELPARAM(HTCLIENT, 0)
        );
    }

    void WindowStyleInfo::SetAcrylicEffect(Color4 tintColor, float tintOpacity)
    {
        // DWM_ACRYLIC_EFFECT acrylic;
        // acrylic.tintColor = tintColor.ToArgb();
        // acrylic.tintOpacity = tintOpacity;
        // acrylic.fallbackColor = 0;
        //
        // DwmSetWindowAttribute(
        //     window->NativeWindow, DWMWA_ACRYLIC_EFFECT,
        //     &acrylic, sizeof(acrylic)
        // );
    }
}
