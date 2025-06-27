#pragma once

#include "DX12Context.h"

namespace MirageAPI::DirectX
{
    public enum class DX12BufferType
    {
        Vertex,
        Index,
        Constant,
        Structured,
        Readback,
        Upload
    };

    public ref class DX12Buffer
    {
    internal:
        ID3D12Resource* m_buffer;
        
        UINT m_size;
        UINT m_stride;

    public:
        DX12Buffer(UINT size, UINT stride, DX12BufferType type);
        ~DX12Buffer();
        !DX12Buffer();

        void UploadData(array<System::Byte>^ data);
        void* Map();
        void Unmap();

        property ID3D12Resource* NativeResource { 
            ID3D12Resource* get() { return m_buffer; } 
        }
    
        property D3D12_GPU_VIRTUAL_ADDRESS GPUAddress { 
            D3D12_GPU_VIRTUAL_ADDRESS get() { 
                return m_buffer ? m_buffer->GetGPUVirtualAddress() : 0; 
            } 
        }
        
        property UINT Size { UINT get() { return m_size; } }
        property UINT Stride { UINT get() { return m_stride; } }
    };
}