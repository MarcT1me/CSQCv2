#pragma once

namespace MirageAPI::Native
{
    class Rect
    {
    public:
        int X, Y, Width, Height;
        
        Rect()
            : X(0), Y(0), Width(0), Height(0)
        {
        }

        Rect(int x, int y, int width, int height)
            : X(x), Y(y), Width(width), Height(height)
        {
        }
        
        int GetLeft() const;
        void SetLeft(int value);
        int GetRight() const;
        void SetRight(int value);
        int GetTop() const;
        void SetTop(int value);
        int GetBottom() const;
        void SetBottom(int value);

        void GetPosition(int& x, int& y) const;
        void SetPosition(int x, int y);
        void GetSize(int& width, int& height) const;
        void SetSize(int width, int height);

        Rect* Copy() const;
    };
}
