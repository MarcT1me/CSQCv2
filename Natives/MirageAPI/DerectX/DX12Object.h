#pragma once

// ReSharper disable once CppRedundantNamespaceDefinition
namespace MirageAPI::DirectX
{
    generic <typename T>
    where T : QuantumCore::Data::Meta::MetaData
    public ref class DX12Object abstract : public QuantumCore::Data::Meta::MetaObject<T>
    {
    protected:
        bool disposed = false;
        ID3D12Device* device;

    public:
        // constructors and deconstructors
        DX12Object(
            T data
        ) : MetaObject(data),
            device(DX12Device::GetNativeDevice(true))
        {
        }

        // ReSharper disable once CppHidingFunction
        ~DX12Object() { this->!DX12Object(); }
        !DX12Object() { disposed = true; }

        // other getters
        property bool Disposed
        {
            bool get() { return disposed; }
        }

        // other methods
        virtual void Validate() { if (disposed) throw gcnew DXException("Can't operate on disposed object"); }
    };
}
