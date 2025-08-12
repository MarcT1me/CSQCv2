#pragma once

namespace MirageAPI
{
    public ref class NullptrException : public QuantumFailure
    {
    public:
        NullptrException(String^ msg) : QuantumFailure(gcnew String(msg))
        {
        }
    };
}
