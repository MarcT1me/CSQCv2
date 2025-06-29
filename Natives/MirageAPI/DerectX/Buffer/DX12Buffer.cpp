#include "pch.h"
#include "DX12Buffer.h"

#include <iterator>

namespace MirageAPI::DirectX
{
    DX12Buffer::DX12Buffer(
        UINT width,
        UINT height,
        UINT stride,
        DX12BufferType type
    ) : m_width(width),
        m_height(height),
        m_stride(stride),
        m_size(width * height)
    {
        auto device = DX12Context::GetDevice();
        if (!device)
        {
            throw gcnew System::InvalidOperationException(
                "DirectX 12 device not initialized. Call DX12Context::Initialize() first."
            );
        }

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Width = width;
        desc.Height = height;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        D3D12_HEAP_TYPE heapType = D3D12_HEAP_TYPE_DEFAULT;
        if (type == DX12BufferType::Upload || type == DX12BufferType::Vertex)
        {
            heapType = D3D12_HEAP_TYPE_UPLOAD;
        }

        D3D12_HEAP_PROPERTIES heapProps;
        heapProps.Type = heapType;
        heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heapProps.CreationNodeMask = 1;
        heapProps.VisibleNodeMask = 1;

        D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_COMMON;
        if (heapType == D3D12_HEAP_TYPE_UPLOAD)
        {
            initialState = D3D12_RESOURCE_STATE_GENERIC_READ;
        }

        ID3D12Resource* buffer = nullptr;
        HRESULT hr = device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            initialState,
            nullptr,
            IID_PPV_ARGS(&buffer)
        );

        if (FAILED(hr))
        {
            throw gcnew System::Exception(
                "Failed to create buffer: " + hr
            );
        }

        m_buffer = buffer;
    }

    DX12Buffer::~DX12Buffer()
    {
        this->!DX12Buffer();
    }

    DX12Buffer::!DX12Buffer()
    {
        if (m_buffer)
        {
            Unmap();
            if (m_buffer->Release() == 0)
            {
                m_buffer = nullptr;
            }
        }
    }

    void DX12Buffer::UploadData(array<System::Byte>^ data)
    {
        if (!m_buffer || data->Length != Size) return;

        if (void* pData = this->Map())
        {
            pin_ptr<System::Byte> pinData = &data[0];
            memcpy(pData, pinData, Size);
            Unmap();
        }
    }

    void* DX12Buffer::Map()
    {
        if (!m_buffer) return nullptr;

        void* pData = nullptr;
        D3D12_RANGE range = {0, Size};
        if (SUCCEEDED(m_buffer->Map(0, &range, &pData)))
            return pData;

        return nullptr;
    }

    void DX12Buffer::Unmap()
    {
        if (m_buffer)
        {
            D3D12_RANGE range = {0, Size};
            m_buffer->Unmap(0, &range);
        }
    }
}
