#include "pch.h"
#include "Rect.h"

namespace MirageAPI::Native
{
    int Rect::GetLeft() const
    {
        return X;
    }

    void Rect::SetLeft(int value)
    {
        X = value;
    }

    int Rect::GetRight() const
    {
        return X + Width;
    }

    void Rect::SetRight(int value)
    {
        X = value - Width;
    }

    int Rect::GetTop() const
    {
        return Y;
    }

    void Rect::SetTop(int value)
    {
        Y = value;
    }

    int Rect::GetBottom() const
    {
        return Y + Height;
    }

    void Rect::SetBottom(int value)
    {
        Y = value - Height;
    }

    void Rect::GetPosition(int& x, int& y) const
    {
        x = X;
        y = Y;
    }

    void Rect::SetPosition(int x, int y)
    {
        X = x;
        Y = y;
    }

    void Rect::GetSize(int& width, int& height) const
    {
        width = Width;
        height = Height;
    }

    void Rect::SetSize(int width, int height)
    {
        Width = width;
        Height = height;
    }

    Rect* Rect::Copy() const
    {
        return new Rect(X, Y, Width, Height);
    }
}
