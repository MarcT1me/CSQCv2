#pragma once

namespace MirageAPI
{
    public ref struct SimpleRect
    {
        int X, Y, Width, Height;

        SimpleRect()
        {
        }

        SimpleRect(int x, int y, int width, int height)
            : X(x), Y(y), Width(width), Height(height)
        {
        }

        SimpleRect(Vector2i^ pos, Vector2i^ size)
            : X(pos->X), Y(pos->Y), Width(size->X), Height(size->Y)
        {
        }

        static SimpleRect^ FromVector(Vector4i vector)
        {
            auto result = gcnew SimpleRect();
            result->X = vector.X;
            result->Y = vector.Y;
            result->Width = vector.Z;
            result->Height = vector.W;
            return result;
        }
    };

    public ref struct DoubleRect
    {
        int Right, Left, Top, Bottom;
        int X, Y, Width, Height;

        DoubleRect()
        {
        }

        DoubleRect(Vector2i^ pos, Vector2i^ size)
            : Right(pos->X + size->X), Left(pos->X), Top(pos->Y), Bottom(pos->Y + size->Y),
              X(pos->X), Y(pos->Y), Width(size->X), Height(size->Y)
        {
        }

        DoubleRect(SimpleRect^ rect)
            : Right(rect->X + rect->Width), Left(rect->X), Top(rect->Y), Bottom(rect->Y + rect->Height),
              X(rect->X), Y(rect->Y), Width(rect->Width), Height(rect->Height)
        {
        }
    };
}
