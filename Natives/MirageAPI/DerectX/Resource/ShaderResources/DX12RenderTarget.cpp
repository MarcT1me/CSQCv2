#include "pch.h"
#include "DX12RenderTarget.h"

#include "../../Descriptors/DX12DescriptorHeap.h"

namespace MirageAPI::DirectX::Resource
{
    DX12RenderTarget::DX12RenderTarget(
        DX12ResourceConfig^ config,
        ID3D12Resource* resource
    ) : DX12Texture(config, nullptr)
    {
        _nativeResource = resource;
    }

    DX12RenderTarget::DX12RenderTarget(
        DX12ResourceConfig^ config
    ) : DX12Texture(config)
    {
        D3D12_HEAP_PROPERTIES heapProps = {
            D3D12_HEAP_TYPE_DEFAULT,
            D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
            D3D12_MEMORY_POOL_UNKNOWN,
            0, 0
        };

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(MetaData->TextureType);
        desc.Width = MetaData->Width;
        desc.Height = MetaData->Height;
        desc.DepthOrArraySize = MetaData->Depth;
        desc.MipLevels = MetaData->MipLevels;
        desc.Format = static_cast<DXGI_FORMAT>(MetaData->Format);
        desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        desc.Flags = static_cast<D3D12_RESOURCE_FLAGS>(MetaData->Flags);
        if (MetaData->SampleCount > 0)
        {
            desc.SampleDesc = {MetaData->SampleCount, MetaData->SampleQuality};
        }

        D3D12_CLEAR_VALUE* clearValuePtr = nullptr;
        D3D12_CLEAR_VALUE clearValue = {};

        if (MetaData->Flags.HasFlag(DX12ResourceFlags::AllowRenderTarget))
        {
            // clear value
            clearValue.Format = static_cast<DXGI_FORMAT>(MetaData->Format);
            clearValue.Color[0] = 0.0f;
            clearValue.Color[1] = 0.0f;
            clearValue.Color[2] = 0.0f;
            clearValue.Color[3] = 1.0f;
            clearValuePtr = &clearValue;
        }
        else if (MetaData->Flags.HasFlag(DX12ResourceFlags::AllowDepthStencil))
        {
            desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
            // depth stencil
            clearValue.Format = static_cast<DXGI_FORMAT>(MetaData->Format);
            clearValue.DepthStencil.Depth = 1.0f;
            clearValue.DepthStencil.Stencil = 0;
            clearValuePtr = &clearValue;
        }

        ID3D12Resource* texture = nullptr;
        CheckHResult(
            device->CreateCommittedResource(
                &heapProps,
                D3D12_HEAP_FLAG_NONE,
                &desc,
                static_cast<D3D12_RESOURCE_STATES>(MetaData->InitialState),
                clearValuePtr,
                IID_PPV_ARGS(&texture)
            ),
            "Failed to create buffer"
        );
        _nativeResource = texture;
    }

    void DX12RenderTarget::CreateDeviceView(Descriptors::DX12Descriptor^ descriptor)
    {
        if (descriptor->_heap->MetaData->Type != Descriptors::DX12DescriptorHeapType::RTV)
        {
            return DX12ShaderResource::CreateDeviceView(descriptor);
        }

        device->CreateRenderTargetView(
            _nativeResource,
            nullptr,
            descriptor->_CPUHandle
        );
        CheckHResult(DX12Device::GetDeviceRemovedReason(), "Creating RTV view error");
    }

    String^ DX12RenderTarget::ToString()
    {
        return CSFormat(
            "DX12RenderTarget<{0}>()",
            Id
        );
    }
}
