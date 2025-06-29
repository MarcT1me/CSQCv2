#include "pch.h"
#include "DX12WindowCommandList.h"

namespace MirageAPI::DirectX
{
    void DX12WindowCommandList::ClearRenderTargetView(
        DX12FrameBuffer^ frameBuffer,
        float r, float g, float b, float a
    )
    {
        if (!m_commandList || frameBuffer == nullptr) return;

        D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle = frameBuffer->RTVHandle;
        if (rtvHandle == nullptr) return;
        
        const float clearColor[] = {r, g, b, a};
        m_commandList->ClearRenderTargetView(*rtvHandle, clearColor, 0, nullptr);
    }
}
