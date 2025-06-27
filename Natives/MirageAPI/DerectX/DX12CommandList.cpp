#include "pch.h"
#include "DX12CommandList.h"

#include "DX12Context.h"

namespace MirageAPI::DirectX
{
    DX12CommandList::DX12CommandList(
        D3D12_COMMAND_LIST_TYPE type
    ) : m_type(type)
    {
        auto device = DX12Context::GetDevice();

        ID3D12CommandAllocator* commandAllocator;
        HRESULT hr = device->CreateCommandAllocator(
            m_type,
            IID_PPV_ARGS(&commandAllocator)
        );
        if (FAILED(hr))
        {
            throw gcnew System::Exception("Failed to create command allocator");
        }
        m_commandAllocator = commandAllocator;

        // Создаем командный список
        ID3D12GraphicsCommandList* commandList;
        hr = device->CreateCommandList(
            0,
            m_type,
            m_commandAllocator,
            nullptr,
            IID_PPV_ARGS(&commandList)
        );
        if (FAILED(hr))
        {
            throw gcnew System::Exception("Failed to create command list");
        }
        m_commandList = commandList;

        Close();
    }

    DX12CommandList::~DX12CommandList() { this->!DX12CommandList(); }

    DX12CommandList::!DX12CommandList()
    {
        if (m_commandList) m_commandList->Release();
        if (m_commandAllocator) m_commandAllocator->Release();
    }

    void DX12CommandList::Reset()
    {
        m_commandAllocator->Reset();
        m_commandList->Reset(m_commandAllocator, nullptr);
    }

    void DX12CommandList::Close()
    {
        m_commandList->Close();
    }

    void DX12CommandList::Execute()
    {
        ID3D12CommandList* ppCommandLists[] = {m_commandList};
        DX12Context::GetCommandQueue()->ExecuteCommandLists(1, ppCommandLists);
    }

    void DX12CommandList::WaitForCompletion()
    {
        // Создаем fence
        ID3D12Fence* fence;
        HRESULT hr = DX12Context::GetDevice()->CreateFence(
            0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
        if (FAILED(hr)) return;

        HANDLE eventHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (!eventHandle) return;

        // Сигнализируем fence
        hr = DX12Context::GetCommandQueue()->Signal(fence, 1);
        if (FAILED(hr))
        {
            CloseHandle(eventHandle);
            fence->Release();
            return;
        }

        // Ждем завершения
        if (fence->GetCompletedValue() < 1)
        {
            fence->SetEventOnCompletion(1, eventHandle);
            WaitForSingleObject(eventHandle, INFINITE);
        }

        CloseHandle(eventHandle);
        fence->Release();
    }
    
    void DX12CommandList::SetPipelineState(DX12PipelineState^ pipelineState)
    {
        m_commandList->SetPipelineState(pipelineState->NativePSO);
    }

    void DX12CommandList::SetGraphicsRootSignature(DX12PipelineState^ pipelineState)
    {
        m_commandList->SetGraphicsRootSignature(pipelineState->RootSignature);
    }

    void DX12CommandList::IASetPrimitiveTopology(PrimitiveTopology topology)
    {
        m_commandList->IASetPrimitiveTopology(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(topology));
    }

    void DX12CommandList::IASetVertexBuffer(DX12VertexBuffer^ vertexBuffer)
    {
        vertexBuffer->Bind(m_commandList);
    }

    void DX12CommandList::DrawInstanced(UINT vertexCount, UINT instanceCount, UINT startVertex, UINT startInstance)
    {
        m_commandList->DrawInstanced(vertexCount, instanceCount, startVertex, startInstance);
    }
}
