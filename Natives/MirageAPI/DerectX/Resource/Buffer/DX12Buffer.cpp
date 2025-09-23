#include "pch.h"
#include "DX12Buffer.h"

namespace MirageAPI::DirectX::Resource
{
    DX12Buffer::DX12Buffer(
        DX12ResourceConfig^ config
    ) : DX12Resource(config)
    {
        // creating heap info
        D3D12_HEAP_PROPERTIES heapProps = {
            static_cast<D3D12_HEAP_TYPE>(config->HeapType),
            // constant
            D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
            D3D12_MEMORY_POOL_UNKNOWN,
            0, 0
        };

        // creating resource description
        D3D12_RESOURCE_DESC desc;
        desc.Width = _size;
        desc.Format = static_cast<DXGI_FORMAT>(config->Format);
        desc.Flags = static_cast<D3D12_RESOURCE_FLAGS>(config->Flags);
        // constant
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment = 0;
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.SampleDesc = {1, 0};
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        // creating resource himself
        ID3D12Resource* buffer = nullptr;
        CheckHResult(
            device->CreateCommittedResource(
                &heapProps,
                static_cast<D3D12_HEAP_FLAGS>(D3D12_HEAP_FLAG_NONE),
                &desc,
                static_cast<D3D12_RESOURCE_STATES>(config->InitialState),
                nullptr,
                IID_PPV_ARGS(&buffer)
            ),
            "Failed to create buffer"
        );
        _nativeResource = buffer;
    }

    void DX12Buffer::UploadData(
        array<Byte>^ data
    )
    {
        // validate
        Validate();
        CheckMissmatch(data->Length, _size) return QLog(Warning, "Data size missmatch");

        // map and check ptr
        void* pData = this->Map();
        CheckNull(pData) return QLog(Warning, "Error mapping data in DX12Buffer::UploadData");
        // copy data
        {
            pin_ptr<Byte> pinData = &data[0];
            memcpy(pData, pinData, _size);
        }
        // unmap
        Unmap();
    }
}
