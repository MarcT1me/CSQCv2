#include "pch.h"
#include "DX12ShaderResource.h"

#include "../Descriptors/DX12DescriptorHeap.h"

namespace MirageAPI::DirectX::Resource
{
    void DX12ShaderResource::!DX12ShaderResource()
    {
        _descriptors->Clear();
    }

    bool DX12ShaderResource::AllocateResource(Descriptors::DX12DescriptorHeap^ heap)
    {
        try
        {
            Descriptors::DX12Descriptor^ descriptor = heap->Allocate(this);
            CheckNull(descriptor) return false;

            CreateDeviceView(descriptor);

            _descriptors[heap->Id] = descriptor;
            return true;
        }
        catch (QuantumFailure^ e)
        {
            QLog(Error, "Failed to allocate resource " + Id + " (inner exception): " + e->Message);
            return false;
        }
        catch (Exception^ ex)
        {
            QLog(Error, "Failed to allocate resource " + Id + " (exception): " + ex->Message);
            return false;
        }
        catch (std::exception* e)
        {
            QLog(Error, "Failed to allocate resource " + Id + " (unknown error): " + gcnew String(e->what()));
            return false;
        }
    }

    void DX12ShaderResource::CreateDeviceView(Descriptors::DX12Descriptor^ descriptor)
    {
        D3D12_SHADER_RESOURCE_VIEW_DESC desc = CreateResourceDesc();
        
        device->CreateShaderResourceView(
            _nativeResource,
            &desc,
            descriptor->_CPUHandle
        );
        CheckHResult(DX12Device::GetDeviceRemovedReason(), "Creating SRV error");
    }

    D3D12_SHADER_RESOURCE_VIEW_DESC DX12ShaderResource::CreateResourceDesc()
    {
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = static_cast<DXGI_FORMAT>(MetaData->Format);
        srvDesc.ViewDimension = static_cast<D3D12_SRV_DIMENSION>(MetaData->Dimension);
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        return srvDesc;
    }

    void DX12ShaderResource::ReleaseResource(Descriptors::DX12DescriptorHeap^ heap)
    {
        Descriptors::DX12Descriptor^ descriptor;
        if (_descriptors->TryRemove(heap->Id, descriptor))
        {
            descriptor->_isDisposed = true;
        }
    }
}
