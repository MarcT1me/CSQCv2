#pragma once

namespace MirageAPI
{
    public ref class CursorInfo
    {
        HCURSOR m_hCursor;

    public:
        CursorInfo(
            CursorType cursorType
        ); // default system cursors

        CursorInfo(
            String^ path
        ); // from file

        CursorInfo(
            array<Byte>^ pixelData,
            int width, int height,
            int hotX, int hotY
        ); // from data (pixel array)

        CursorInfo(
            array<Byte>^ data
        ); // from data (cursor data buffer)

        ~CursorInfo() { this->!CursorInfo(); }
        !CursorInfo();

        property HCURSOR NativeCursor
        {
            HCURSOR get() { return m_hCursor; }
        }
    };
}
