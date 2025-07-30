#pragma once

namespace MirageAPI::DirectX
{
    public ref class DXException : public System::Exception
    {
    public:
        DXException(System::String^ msg) : Exception(gcnew System::String(msg))
        {
        }
    };

    public ref class HRException : public DXException
    {
        HRESULT m_hr;

    public:
        HRException(HRESULT hr, System::String^ msg) : DXException(gcnew System::String(msg)), m_hr(hr)
        {
        }

        HRESULT HR() { return m_hr; }
    };
}
