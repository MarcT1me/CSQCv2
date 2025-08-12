#pragma once

namespace MirageAPI::DirectX
{
    public ref class DXException : public QuantumFailure
    {
    public:
        DXException(String^ msg) : QuantumFailure(gcnew String(msg))
        {
        }
    };

    public ref class HRException : public DXException
    {
        HRESULT m_hr;

    public:
        HRException(HRESULT hr, String^ msg) : DXException(gcnew String(msg)), m_hr(hr)
        {
        }

        HRESULT HR() { return m_hr; }
    };
}
