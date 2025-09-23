#pragma once

namespace MirageAPI::DirectX
{
    public ref class DX12ObjectData : public QuantumCore::Data::Meta::MetaData
    {
    public:
        DX12ObjectData(QIdentifier^ identifier) : MetaData(identifier)
        {
        }
    };
}
