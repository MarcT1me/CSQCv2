#include "pch.h"
#include "DX12FrameBuffer.h"


namespace MirageAPI::DirectX
{
    DX12FrameBuffer::~DX12FrameBuffer()
    {
        this->!DX12FrameBuffer();
    }

    DX12FrameBuffer::!DX12FrameBuffer()
    {
        if (m_rtvHandlePtr)
        {
            delete static_cast<D3D12_CPU_DESCRIPTOR_HANDLE*>(m_rtvHandlePtr);
            m_rtvHandlePtr = nullptr;
        }

        if (m_buffer)
        {
            Unmap();
            if (m_buffer->Release() == 0)
            {
                m_buffer = nullptr;
            }
        }
    }
}
