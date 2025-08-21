#pragma once

namespace MirageAPI
{
    public ref class MirageAPIException : public QuantumFailure
    {
    public:
        MirageAPIException(String^ msg) : QuantumFailure(gcnew String(msg))
        {
        }
    };
    
    public ref class NullptrException : public MirageAPIException
    {
    public:
        NullptrException(String^ msg) : MirageAPIException(gcnew String(msg))
        {
        }
    };
}
