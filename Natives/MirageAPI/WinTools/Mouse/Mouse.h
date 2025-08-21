#pragma once

namespace MirageAPI
{
    ref class Window;

    public ref class Mouse
    {
        Window^ window;

        Vector2i^ savedPosition;
        Vector2i^ lastPosition;
        Vector2i^ delta;

        static Vector2i^ savedDisplayPosition;
        static Vector2i^ lastDisplayPosition;
        static Vector2i^ displayDelta;

        static bool isMouseVisible = true;
        static Window^ mouseCaptureWindow;

    internal:
        bool isTracking = false;
        bool isInWindow = false;

        void Update(Vector2i^ pos);
        static void UpdateDisplayPositions();
        static void MouseEventHandle(Window^ eventWindow, Events::MouseEvent^ event);

    public:
        delegate void MouseEventDelegate(Events::MouseEvent^ event);
        static event MouseEventDelegate^ OnMouse;

        static Mouse();

        Mouse(Window^ window);

        property Vector2i^ Position
        {
            Vector2i^ get();
            void set(Vector2i^ v);
        }
        property Vector2i^ Delta
        {
            Vector2i^ get() { return delta; }
        }
        property bool IsCapture
        {
            bool get() { return mouseCaptureWindow == window; }
        }
        property bool IsInWindow
        {
            bool get() { return isInWindow; }
        }

        static property Vector2i^ PositionOnDisplay
        {
            Vector2i^ get();
            void set(Vector2i^ value);
        }

        static property Vector2i^ DeltaForDisplay
        {
            Vector2i^ get() { return displayDelta; }
        }

        static property bool IsVisible
        {
            bool get() { return isMouseVisible; }
            void set(bool value);
        }
        static property Window^ CaptureWindow
        {
            Window^ get() { return mouseCaptureWindow; }
            void set(Window^ value);
        }
    };
}
