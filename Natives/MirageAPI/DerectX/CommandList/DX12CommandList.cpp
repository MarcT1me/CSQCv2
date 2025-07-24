#include "pch.h"
#include "DX12CommandList.h"

#include "../Pipeline/DX12PipelineState.h"
#include "../DX12DescriptorHeap.h"
#include "../Buffer/DX12Texture.h"
#include "../Buffer/DX12FrameBuffer.h"

namespace MirageAPI::DirectX
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
        DX12_CHECK(device, hr, "Failed to create command allocator");
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
        DX12_CHECK(device, hr, "Failed to create command list");
        m_commandList = commandList;
        m_commandList->Close(); // instantly close

        // command queue
        D3D12_COMMAND_QUEUE_DESC queueDesc;
        queueDesc.Type = static_cast<D3D12_COMMAND_LIST_TYPE>(m_type);
        queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.NodeMask = 0;

        ID3D12CommandQueue* commandQueue;
        hr = device->CreateCommandQueue(
            &queueDesc,
            IID_PPV_ARGS(&commandQueue)
        );
        DX12_CHECK(device, hr, "Failed to create command queue.");
        m_commandQueue = commandQueue;
    }

    DX12CommandList::!DX12CommandList()
    {
        Validate();

        if (m_commandQueue && m_commandQueue->Release() == 0)
            m_commandQueue = nullptr;
        if (m_commandList && m_commandList->Release() == 0)
            m_commandList = nullptr;
        if (m_commandAllocator && m_commandAllocator->Release() == 0)
            m_commandAllocator = nullptr;
    }

    // other properties

    void DX12CommandList::DescriptorHeap::set(DX12DescriptorHeap^ heap)
    {
        ID3D12DescriptorHeap* heaps[] = {heap->NativeHeap};
        m_commandList->SetDescriptorHeaps(1, heaps);
        m_descriptorHeap = heap;
    }

    void DX12CommandList::PipelineState::set(DX12PipelineState^ pipelineState)
    {
        m_commandList->SetPipelineState(pipelineState->NativePSO);
        m_commandList->SetGraphicsRootSignature(pipelineState->RootSignature);
        m_pipelineState = pipelineState;
    }

    // command list operations

    void DX12CommandList::Reset()
    {
        Validate();

        m_commandAllocator->Reset();
        m_commandList->Reset(m_commandAllocator, nullptr);
    }

    void DX12CommandList::Close()
    {
        Validate();

        m_commandList->Close();
    }

    void DX12CommandList::Execute()
    {
        Validate();

        ID3D12CommandList* ppCommandLists[] = {m_commandList};
        m_commandQueue->ExecuteCommandLists(1, ppCommandLists);
    }

    void DX12CommandList::WaitForCompletion()
    {
        Validate();

        // creating one-time fence
        ID3D12Fence* fence;
        HRESULT hr = device->CreateFence(
            0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)
        );
        DX12_CHECK(device, hr, "some error in a command queue executing waiting");

        // fence event
        HANDLE eventHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (!eventHandle) return;

        // waiting for ends of all operations
        if (m_commandQueue->Signal(fence, 1) && fence->GetCompletedValue() < 1)
        {
            fence->SetEventOnCompletion(1, eventHandle);
            WaitForSingleObject(eventHandle, INFINITE);
        }

        // close operation
        CloseHandle(eventHandle);
        fence->Release();
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
        m_commandList->IASetPrimitiveTopology(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(topology));
    }

    void DX12CommandList::DrawInstanced(
        UINT vertexCount,
        UINT count,
        UINT startVertex,
        UINT startInstance
    )
    {
        m_commandList->DrawInstanced(
            vertexCount,
            count,
            startVertex,
            startInstance
        );
    }

    void DX12CommandList::ClearRenderTargetView(
        DX12FrameBuffer^ frameBuffer,
        float r, float g, float b, float a
    )
    {
        Validate();

        const float clearColor[] = {r, g, b, a};
        m_commandList->ClearRenderTargetView(frameBuffer->RTVHandle, clearColor, 0, nullptr);
    }

    // bindings

    void DX12CommandList::SetTexture(UINT rootIndex, DX12Texture^ texture)
    {
        Validate();
        m_descriptorHeap->Validate();

        // installing descriptor heaps in this command list
        ID3D12DescriptorHeap* heaps[] = {m_descriptorHeap->NativeHeap};
        m_commandList->SetDescriptorHeaps(1, heaps);

        // calculate GPU descriptor
        D3D12_GPU_DESCRIPTOR_HANDLE handle = m_descriptorHeap->NativeHeap->GetGPUDescriptorHandleForHeapStart();
        handle.ptr += texture->SRVIndex * m_descriptorHeap->DescriptorSize;

        // installing descriptor heap table
        m_commandList->SetGraphicsRootDescriptorTable(rootIndex, handle);
    }

    void DX12CommandList::SetRootConstants(UINT rootIndex, UINT constantSize, float data[], UINT offset)
    {
        m_commandList->SetGraphicsRoot32BitConstants(
            rootIndex,
            constantSize,
            data,
            offset
        );
    }

    void DX12CommandList::BindBuffer(DX12IndexBuffer^ indexBuffer)
    {
        indexBuffer->Bind(m_commandList);
    }

    void DX12CommandList::BindBuffer(DX12VertexBuffer^ vertexBuffer)
    {
        vertexBuffer->Bind(m_commandList);
    }

    void DX12CommandList::BindBuffer(UINT rootIndex, DX12ConstantBuffer^ constantBuffer)
    {
        m_commandList->SetGraphicsRootConstantBufferView(
            rootIndex,
            constantBuffer->GPUAddress
        );
    }

    void DX12CommandList::BindBuffer(UINT rootIndex, DX12StructuredBuffer^ structuredBuffer)
    {
        m_commandList->SetGraphicsRootShaderResourceView(
            rootIndex,
            structuredBuffer->GPUAddress
        );
    }

    // other

    void DX12CommandList::Validate()
    {
        if (!m_descriptorHeap)
        {
            throw gcnew System::InvalidOperationException("Command List has not a Descriptor Heap");
        }
    }
}
