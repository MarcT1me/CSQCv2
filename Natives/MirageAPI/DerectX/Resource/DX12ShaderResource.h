#pragma once

#include "DX12Resource.h"
#include "../Descriptors/DX12DEscriptor.h"

namespace MirageAPI::DirectX::Resource
{
    public ref class DX12ShaderResource abstract : public DX12Resource
    {
        QuantumCore::Data::Collections::ConcurrentIdentifierMap<Descriptors::DX12Descriptor^>^ _descriptors = gcnew QuantumCore::Data::Collections::ConcurrentIdentifierMap<Descriptors::DX12Descriptor^>(nullptr);

    protected:
        virtual D3D12_SHADER_RESOURCE_VIEW_DESC CreateResourceDesc();

    internal:
        virtual void CreateDeviceView(Descriptors::DX12Descriptor^ descriptor);

    public:
        DX12ShaderResource(DX12ResourceConfig^ config) : DX12Resource(config)
        {
        }

        ~DX12ShaderResource() { this->!DX12ShaderResource(); }
        !DX12ShaderResource();

        bool AllocateResource(Descriptors::DX12DescriptorHeap^ heap);
        void ReleaseResource(Descriptors::DX12DescriptorHeap^ heap);
    };
}
