#pragma once

namespace MirageAPI
{
    public ref class NullptrException : public QuantumFailure
    {
    public:
        NullptrException(System::String^ msg) : QuantumFailure(gcnew System::String(msg))
        {
        }
    };
}
