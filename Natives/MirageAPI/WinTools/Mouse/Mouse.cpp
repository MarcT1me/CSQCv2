#include "pch.h"
#include "Mouse.h"

#include "../Window/Window.h"

namespace MirageAPI
{
    static Mouse::Mouse()
    {
        savedDisplayPosition = lastDisplayPosition = PositionOnDisplay;
    }

    Mouse::Mouse(
        Window^ window
    ) : window(window)
    {
        if (window)
            savedPosition = lastPosition = Position;
    }

    void Mouse::MouseEventHandle(Window^ eventWindow, Events::MouseEvent^ event)
    {
        if (eventWindow)
        {
            switch (event->Type)
            {
            case Events::EventType::MouseMove:
                eventWindow->Mouse->Update(event->Pos);
            case Events::EventType::MouseEnter:
                eventWindow->Mouse->lastPosition = eventWindow->Mouse->Position;

            default: ;
            }
        }
        OnMouse(event);
    }

    void Mouse::Update(Vector2i^ pos)
    {
        if (IsCapture)
        {
            Rect^ rect = window->CurrentRect;

            Position = pos = gcnew Vector2i(
                static_cast<int>(rect->Width) / 2,
                static_cast<int>(rect->Height) / 2
            );
        }

        lastPosition = savedPosition;
        savedPosition = pos;

        delta = gcnew Vector2i(
            pos->X - lastPosition->X,
            pos->Y - lastPosition->Y
        );
    }

    void Mouse::UpdateDisplayPositions()
    {
        lastDisplayPosition = savedDisplayPosition;
        savedDisplayPosition = PositionOnDisplay;

        displayDelta = gcnew Vector2i(
            savedDisplayPosition->X - lastDisplayPosition->X,
            savedDisplayPosition->Y - lastDisplayPosition->Y
        );
    }

    Vector2i^ Mouse::Position::get()
    {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(window->NativeWindow, &point);
        return gcnew Vector2i(point.x, point.y);
    }

    void Mouse::Position::set(Vector2i^ value)
    {
        POINT center = {static_cast<LONG>(value->X), static_cast<LONG>(value->Y)};
        ClientToScreen(window->NativeWindow, &center);
        SetCursorPos(center.x, center.y);
    }

    Vector2i^ Mouse::PositionOnDisplay::get()
    {
        POINT point;
        GetCursorPos(&point);
        return gcnew Vector2i(point.x, point.y);
    }

    void Mouse::PositionOnDisplay::set(Vector2i^ value)
    {
        POINT center = {static_cast<LONG>(value->X), static_cast<LONG>(value->Y)};
        SetCursorPos(center.x, center.y);
    }

    void Mouse::IsVisible::set(bool value)
    {
        ShowCursor(value);
        isMouseVisible = value;
    }

    void Mouse::CaptureWindow::set(Window^ value)
    {
        if (mouseCaptureWindow)
        {
            QLog(Warning, "Mouse has already been captured in another window\nTrying to release capture mouse");
            CaptureWindow = nullptr;
        }

        if (value)
        {
            auto mouse = value->Mouse;
            mouse->lastPosition = mouse->savedPosition = mouse->Position;
        }

        mouseCaptureWindow = value;
    }
}
