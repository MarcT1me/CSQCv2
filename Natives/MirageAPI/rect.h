#pragma once

namespace MirageAPI
{
    public ref struct Rect
    {
        int X, Y, Width, Height;

        Rect()
        {
        }

        Rect(int x, int y, int width, int height)
            : X(x), Y(y), Width(width), Height(height)
        {
        }

        Rect(RECT nativeRect)
            : X(nativeRect.left), Y(nativeRect.top),
              Width(nativeRect.right - nativeRect.left), Height(nativeRect.bottom - nativeRect.top)
        {
        }

        Rect(Vector4i^ vector)
            : X(vector->X), Y(vector->Y),
              Width(vector->Z), Height(vector->W)
        {
        }

        Rect(Vector2i^ pos, Vector2i^ size)
            : X(pos ? pos->X : CW_USEDEFAULT), Y(pos ? pos->Y : CW_USEDEFAULT),
              Width(size ? size->X : 0), Height(size ? size->Y : 0)
        {
        }

        property int Left
        {
            int get() { return X + Width; }
            void set(int value) { X = value; }
        }
        property int Right
        {
            int get() { return X; }
            void set(int value) { X = value - Width; }
        }
        property int Top
        {
            int get() { return Y; }
            void set(int value) { Y = value; }
        }
        property int Bottom
        {
            int get() { return Y + Height; }
            void set(int value) { Y = value - Height; }
        }

        property Vector2i^ Pos
        {
            Vector2i^ get() { return gcnew Vector2i(X, Y); }

            void set(Vector2i^ value)
            {
                this->X = value->X;
                this->Y = value->Y;
            }
        }

        property Vector2i^ Size
        {
            Vector2i^ get() { return gcnew Vector2i(Width, Height); }

            void set(Vector2i^ value)
            {
                this->Width = value->X;
                this->Height = value->Y;
            }
        }

        Rect^ Copy()
        {
            return gcnew Rect(X, Y, Width, Height);
        }
    };
}
