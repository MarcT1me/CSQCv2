#pragma once

#include "..\DX12Context.h"
#include "..\DX12Enums.h"

namespace MirageAPI::DirectX
{
    public ref class DX12Buffer
    {
    internal:
        ID3D12Resource* m_buffer;

        UINT m_width;
        UINT m_height;
        UINT m_stride;
        UINT m_size;

    public:
        DX12Buffer(
            ID3D12Resource* resource
        ) : m_buffer(resource)
        {
        }

        DX12Buffer(
            UINT width,
            UINT height,
            UINT stride,
            DX12BufferType type
        );
        virtual ~DX12Buffer();
        !DX12Buffer();

        void UploadData(array<System::Byte>^ data);
        void* Map();
        void Unmap();

        property ID3D12Resource* NativeResource
        {
            ID3D12Resource* get() { return m_buffer; }
        }

        property D3D12_GPU_VIRTUAL_ADDRESS GPUAddress
        {
            D3D12_GPU_VIRTUAL_ADDRESS get()
            {
                return m_buffer ? m_buffer->GetGPUVirtualAddress() : 0;
            }
        }

        property UINT Width { UINT get() { return m_width; } }
        property UINT Height { UINT get() { return m_height; } }
        property UINT Size { UINT get() { return m_size; } }
        property UINT Stride { UINT get() { return m_stride; } }
    };
}
