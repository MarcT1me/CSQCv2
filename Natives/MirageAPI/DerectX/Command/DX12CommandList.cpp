#include "pch.h"
#include "DX12CommandList.h"

#include "../Pipeline/DX12PipelineState.h"
#include "../DX12DescriptorHeap.h"
#include "../Resource/Matrix/DX12FrameBuffer.h"

namespace MirageAPI::DirectX::Command
{
    DX12CommandList::DX12CommandList(
        DX12CommandListType type
    ) : m_type(type)
    {
        // command allocator
        ID3D12CommandAllocator* commandAllocator;
        HRESULT hr = device->CreateCommandAllocator(
            static_cast<D3D12_COMMAND_LIST_TYPE>(m_type),
            IID_PPV_ARGS(&commandAllocator)
        );
        CheckHResult(hr, "Failed to create command allocator");
        m_commandAllocator = commandAllocator;

        // command list
        ID3D12GraphicsCommandList* commandList;
        hr = device->CreateCommandList(
            0,
            static_cast<D3D12_COMMAND_LIST_TYPE>(m_type),
            m_commandAllocator,
            nullptr,
            IID_PPV_ARGS(&commandList)
        );
        CheckHResult(hr, "Failed to create command list");
        m_commandList = commandList;
        m_commandList->Close(); // instantly close
    }

    DX12CommandList::!DX12CommandList()
    {
        Validate();

        SimpleRelease(m_commandList);
        SimpleRelease(m_commandAllocator);
    }

    // command list operations

    void DX12CommandList::UpdatePipelineState()
    {
        Validate();
        m_commandList->SetPipelineState(m_pipelineState->NativePSO);
        m_commandList->SetGraphicsRootSignature(m_pipelineState->RootSignature);
    }

    void DX12CommandList::UpdateDescriptorHeap()
    {
        Validate();
        CheckNull(m_descriptorHeap) return;

        ID3D12DescriptorHeap* heaps[] = {m_descriptorHeap->NativeHeap};
        m_commandList->SetDescriptorHeaps(1, heaps);
        m_commandList->SetGraphicsRootDescriptorTable(0, m_descriptorHeap->StartGPUHandle);
    }

    void DX12CommandList::Reset()
    {
        Validate();

        CheckHResult(m_commandAllocator->Reset(), "reset command allocator");
        CheckHResult(m_commandList->Reset(m_commandAllocator, nullptr), "reset command list");
    }

    void DX12CommandList::Close()
    {
        Validate();

        CheckHResult(m_commandList->Close(), "close command list");
    }

    // Viewport and other

    void DX12CommandList::SetViewport(
        float topLeftX, float topLeftY,
        float width, float height,
        float minDepth, float maxDepth
    )
    {
        Validate();

        D3D12_VIEWPORT viewport = {
            topLeftX, topLeftY,
            width, height,
            minDepth, maxDepth
        };
        m_commandList->RSSetViewports(1, &viewport);
    }

    void DX12CommandList::SetScissorRect(
        int left, int top,
        int right, int bottom
    )
    {
        Validate();

        D3D12_RECT rect = {left, top, right, bottom};
        m_commandList->RSSetScissorRects(1, &rect);
    }

    // render

    void DX12CommandList::SetPrimitiveTopology(
        DX12PrimitiveTopology topology
    )
    {
        Validate();

        m_commandList->IASetPrimitiveTopology(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(topology));
    }

    void DX12CommandList::DrawInstanced(
        UINT vertexCount,
        UINT count,
        UINT startVertex,
        UINT startInstance
    )
    {
        Validate();

        m_commandList->DrawInstanced(
            vertexCount,
            count,
            startVertex,
            startInstance
        );
    }

    void DX12CommandList::ClearRenderTargetView(
        Resource::DX12FrameBuffer^ frameBuffer,
        float r, float g, float b, float a
    )
    {
        Validate();

        const float clearColor[] = {r, g, b, a};
        m_commandList->ClearRenderTargetView(frameBuffer->SRVHandleForCPU, clearColor, 0, nullptr);
    }

    // bindings

    void DX12CommandList::BindBuffer(Resource::DX12FrameBuffer^ frameBuffer)
    {
        Validate();

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = frameBuffer->SRVHandleForCPU;
        m_commandList->OMSetRenderTargets(
            1,
            &rtvHandle,
            FALSE,
            nullptr
        );
    }

    void DX12CommandList::BindBuffer(Resource::DX12IndexBuffer^ indexBuffer)
    {
        Validate();

        indexBuffer->Bind(m_commandList);
    }

    void DX12CommandList::BindBuffer(Resource::DX12VertexBuffer^ vertexBuffer)
    {
        Validate();

        vertexBuffer->Bind(m_commandList);
    }

    void DX12CommandList::BindBuffer(UINT rootIndex, Resource::DX12ConstantBuffer^ constantBuffer)
    {
        Validate();

        m_commandList->SetGraphicsRootConstantBufferView(
            rootIndex,
            constantBuffer->GPUAddress
        );
    }

    void DX12CommandList::BindBuffer(UINT rootIndex, Resource::DX12StructuredBuffer^ structuredBuffer)
    {
        Validate();

        m_commandList->SetGraphicsRootShaderResourceView(
            rootIndex,
            structuredBuffer->GPUAddress
        );
    }

    void DX12CommandList::SetRootConstants(UINT rootIndex, UINT constantSize, float data[], UINT offset)
    {
        Validate();

        m_commandList->SetGraphicsRoot32BitConstants(
            rootIndex,
            constantSize,
            data,
            offset
        );
    }
}
