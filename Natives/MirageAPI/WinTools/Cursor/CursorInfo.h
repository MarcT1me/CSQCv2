#pragma once

#include "../../NativeInstance.h"
#include "../../Resource.h"

namespace MirageAPI
{
    ref class NativeInstance;
}

namespace MirageAPI
{
    public ref class CursorInfo
    {
        HCURSOR m_hCursor;

        CursorInfo(
            NativeInstance^ instance,
            int resource
        ); // from .dll

        CursorInfo(
            int resource
        ); // from MirageAPI..dll

    public:
        // default (system) cursors
        static initonly CursorInfo^ Arrow = gcnew CursorInfo(CursorType::ARROW);
        static initonly CursorInfo^ IBeam = gcnew CursorInfo(CursorType::IBEAM);
        static initonly CursorInfo^ Wait = gcnew CursorInfo(CursorType::WAIT);
        static initonly CursorInfo^ Cross = gcnew CursorInfo(CursorType::CROSS);
        static initonly CursorInfo^ SizeRight = gcnew CursorInfo(CursorType::SIZE_RIGHT);
        static initonly CursorInfo^ SizeLeft = gcnew CursorInfo(CursorType::SIZE_LEFT);
        static initonly CursorInfo^ SizeVertical = gcnew CursorInfo(CursorType::SIZE_VERTICAL);
        static initonly CursorInfo^ SizeHorizontal = gcnew CursorInfo(CursorType::SIZE_HORIZONTAL);
        static initonly CursorInfo^ SizeAll = gcnew CursorInfo(CursorType::SIZE_ALL);
        static initonly CursorInfo^ Unavailable = gcnew CursorInfo(CursorType::Unavailable);
        static initonly CursorInfo^ Hand = gcnew CursorInfo(CursorType::HAND);
        static initonly CursorInfo^ AppStarting = gcnew CursorInfo(CursorType::APP_STARTING);
        static initonly CursorInfo^ Help = gcnew CursorInfo(CursorType::HELP);
        static initonly CursorInfo^ Pin = gcnew CursorInfo(CursorType::PIN);
        static initonly CursorInfo^ Person = gcnew CursorInfo(CursorType::PERSON);
        static initonly CursorInfo^ Default = Arrow;
        // default (engine) cursors
        static initonly CursorInfo^ QArrow = gcnew CursorInfo(Engine::Asset::AssetLoader::AssetDirectory + "/Native/Cursors/arrow.cur");
        static initonly CursorInfo^ QIBeam = gcnew CursorInfo(Engine::Asset::AssetLoader::AssetDirectory + "/Native/Cursors/ibeam.cur");
        static initonly CursorInfo^ QWait = gcnew CursorInfo(Engine::Asset::AssetLoader::AssetDirectory + "/Native/Cursors/wait.ani");
        static initonly CursorInfo^ QCross = gcnew CursorInfo(Engine::Asset::AssetLoader::AssetDirectory + "/Native/Cursors/cross.cur");
        static initonly CursorInfo^ QSizeRight = gcnew CursorInfo(Engine::Asset::AssetLoader::AssetDirectory + "/Native/Cursors/size_right.cur");
        static initonly CursorInfo^ QSizeLeft = gcnew CursorInfo(Engine::Asset::AssetLoader::AssetDirectory + "/Native/Cursors/size_left.cur");
        static initonly CursorInfo^ QSizeVertical = gcnew CursorInfo(Engine::Asset::AssetLoader::AssetDirectory + "/Native/Cursors/size_vertical.cur");
        static initonly CursorInfo^ QSizeHorizontal = gcnew CursorInfo(Engine::Asset::AssetLoader::AssetDirectory + "/Native/Cursors/size_horizontal.cur");
        static initonly CursorInfo^ QSizeAll = gcnew CursorInfo(Engine::Asset::AssetLoader::AssetDirectory + "/Native/Cursors/size_all.cur");
        static initonly CursorInfo^ QUnavailable = gcnew CursorInfo(Engine::Asset::AssetLoader::AssetDirectory + "/Native/Cursors/unavailable.cur");
        static initonly CursorInfo^ QHand = gcnew CursorInfo(Engine::Asset::AssetLoader::AssetDirectory + "/Native/Cursors/hand.cur");
        static initonly CursorInfo^ QAppStarting = gcnew CursorInfo(Engine::Asset::AssetLoader::AssetDirectory + "/Native/Cursors/app_starting.ani");
        static initonly CursorInfo^ QHelp = gcnew CursorInfo(Engine::Asset::AssetLoader::AssetDirectory + "/Native/Cursors/help.cur");
        static initonly CursorInfo^ QPin = gcnew CursorInfo(Engine::Asset::AssetLoader::AssetDirectory + "/Native/Cursors/pin.cur");
        static initonly CursorInfo^ QPerson = gcnew CursorInfo(Engine::Asset::AssetLoader::AssetDirectory + "/Native/Cursors/person.cur");
        static initonly CursorInfo^ QDefault = QArrow;

        CursorInfo(
            CursorType cursorType
        ); // default system cursors

        CursorInfo(
            String^ path
        ); // from file

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
