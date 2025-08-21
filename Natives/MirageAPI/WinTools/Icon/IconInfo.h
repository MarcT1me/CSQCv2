#pragma once

#include "../../NativeInstance.h"
#include "../../Resource.h"

namespace MirageAPI
{
    ref class NativeInstance;
}

namespace MirageAPI
{
    public ref class IconInfo
    {
        HICON m_hIcon;

        IconInfo(
            NativeInstance^ instance,
            int resource
        ); // from .dll

        IconInfo(
            int resource
        ); // from MirageAPI..dll

    public:
        // default system icons
        static IconInfo^ Default = Application;
        static IconInfo^ Application = gcnew IconInfo(IconType::Application);
        static IconInfo^ Hand = gcnew IconInfo(IconType::Hand);
        static IconInfo^ Question = gcnew IconInfo(IconType::Question);
        static IconInfo^ Exclamation = gcnew IconInfo(IconType::Exclamation);
        static IconInfo^ Asterisk = gcnew IconInfo(IconType::Asterisk);
        static IconInfo^ WinLogo = gcnew IconInfo(IconType::WinLogo);
        static IconInfo^ Shield = gcnew IconInfo(IconType::Shield);
        // default engine icons
        static IconInfo^ QDefault = gcnew IconInfo(Engine::Asset::AssetLoader::AssetDirectory + "/Native/qEngine.ico");

        IconInfo(
            IconType iconType
        ); // default system icons

        IconInfo(
            String^ path
        ); // loading from file

        IconInfo(
            array<Byte>^ data
        ); // loading from data

        ~IconInfo() { this->!IconInfo(); }
        !IconInfo();

        property HICON NativeIcon
        {
            HICON get() { return m_hIcon; }
        }
    };
}
