#include "pch.h"
#include "DX12UploadBuffer.h"

#include "..\DX12Context.h"

namespace MirageAPI::DirectX
{
    DX12UploadBuffer::DX12UploadBuffer(DX12ResourceConfig config)
        : DX12Resource(config.Width, config.Format)
    {
        auto device = DX12Context::GetDevice();
        if (!device) throw gcnew System::Exception("DX12 device not initialized");

        D3D12_HEAP_PROPERTIES heapProps = {
            D3D12_HEAP_TYPE_UPLOAD,
            D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
            D3D12_MEMORY_POOL_UNKNOWN,
            0, 0
        };

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Width = config.Width;
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc = {1, 0};
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        ID3D12Resource* buffer = nullptr;
        HRESULT hr = device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            static_cast<D3D12_RESOURCE_STATES>(config.InitialState),
            nullptr,
            IID_PPV_ARGS(&buffer)
        );

        if (FAILED(hr))
        {
            throw gcnew System::Exception(
                "Failed to create buffer: " + hr
            );
        }

        m_nativeResource = buffer;
    }

    void DX12UploadBuffer::TransitionState(
        DX12CommandList^ commandList,
        DX12ResourceState newState)
    {
        // Upload буферы обычно не требуют переходов состояний
    }
}
