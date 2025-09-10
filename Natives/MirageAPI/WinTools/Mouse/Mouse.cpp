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
    ) : window(window),
        delta(Vector2i::Zero)
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
        lastPosition = savedPosition;
        savedPosition = pos;

        delta = gcnew Vector2i(
            savedPosition->X - lastPosition->X,
            savedPosition->Y - lastPosition->Y
        );

        if (IsCapture)
        {
            Rect^ rect = window->CurrentRect;
            Vector2i^ center = gcnew Vector2i(
                static_cast<int>(rect->Width) / 2,
                static_cast<int>(rect->Height) / 2
            );
            Position = center;

            lastPosition = center;
            savedPosition = lastPosition;
        }
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
        POINT center = {UnpacVec2(value)};
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
        POINT center = {UnpacVec2(value)};
        SetCursorPos(center.x, center.y);
    }

    void Mouse::IsVisible::set(bool value)
    {
        ShowCursor(value);
        isMouseVisible = value;
    }

    void Mouse::CaptureWindow::set(Window^ value)
    {
        if (value)
        {
            if (mouseCaptureWindow)
            {
                QLog(Warning, "Mouse has already been captured in another window. Trying to release capture mouse");
                CaptureWindow = nullptr;
            }

            Rect^ rect = value->CurrentRect;
            Vector2i^ center = gcnew Vector2i(
                static_cast<int>(rect->Width) / 2,
                static_cast<int>(rect->Height) / 2
            );

            value->Mouse->Position =
                value->Mouse->lastPosition =
                value->Mouse->savedPosition = center;
        }

        mouseCaptureWindow = value;
    }
}
