#pragma once

namespace MirageAPI
{
    public ref class IconInfo
    {
        HICON m_hIcon;

    public:
        IconInfo(
            String^ path
        ); // loading from file

        IconInfo(
            array<Byte>^ pixelData
        ); // loading from data

        ~IconInfo() { this->!IconInfo(); }
        !IconInfo();

        property HICON NativeIcon
        {
            HICON get() { return m_hIcon; }
        }
    };
}
