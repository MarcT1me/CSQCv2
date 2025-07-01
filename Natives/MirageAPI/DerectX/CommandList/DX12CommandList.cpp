#include "pch.h"
#include "DX12CommandList.h"

#include "..\DX12Enums.h"
#include "..\DX12Context.h"
#include "..\Buffer\DX12Texture.h"

namespace MirageAPI::DirectX
{
    DX12CommandList::DX12CommandList(
        DX12CommandListType type
    ) : m_type(type)
    {
        auto device = DX12Context::GetDevice();

        ID3D12CommandAllocator* commandAllocator;
        HRESULT hr = device->CreateCommandAllocator(
            static_cast<D3D12_COMMAND_LIST_TYPE>(m_type),
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
            static_cast<D3D12_COMMAND_LIST_TYPE>(m_type),
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

    void DX12CommandList::SetDescriptorHeap(DX12DescriptorHeap^ heap)
    {
        if (!heap) return;
        ID3D12DescriptorHeap* heaps[] = {heap->NativeHeap};
        m_commandList->SetDescriptorHeaps(1, heaps);
    }

    void DX12CommandList::SetTextureSRV(UINT rootIndex, DX12Texture^ texture)
    {
        if (!texture || !texture->HasSRV) return;

        auto srvHeap = DX12Context::GetDescriptorHeap(DX12DescriptorHeapType::CBV_SRV_UAV, 256, true);

        D3D12_GPU_DESCRIPTOR_HANDLE handle = srvHeap->NativeHeap->GetGPUDescriptorHandleForHeapStart();
        handle.ptr += texture->SRVIndex * srvHeap->DescriptorSize;

        m_commandList->SetGraphicsRootDescriptorTable(rootIndex, handle);
    }

    void DX12CommandList::TransitionTexture(
        DX12Texture^ texture,
        DX12ResourceState newState
    )
    {
        if (!texture) return;
        texture->TransitionState(this, newState);
    }

    void DX12CommandList::SetViewport(
        float topLeftX, float topLeftY,
        float width, float height,
        float minDepth, float maxDepth
    )
    {
        if (!m_commandList) return;

        D3D12_VIEWPORT viewport = {
            topLeftX, topLeftY,
            width, height,
            minDepth, maxDepth
        };
        m_commandList->RSSetViewports(1, &viewport);
    }

    void DX12CommandList::SetScissorRect(int left, int top, int right, int bottom)
    {
        if (!m_commandList) return;

        D3D12_RECT rect = {left, top, right, bottom};
        m_commandList->RSSetScissorRects(1, &rect);
    }

    void DX12CommandList::ClearRenderTargetView(
        DX12FrameBuffer^ frameBuffer,
        float r, float g, float b, float a
    )
    {
        if (!m_commandList || frameBuffer == nullptr) return;

        const float clearColor[] = {r, g, b, a};
        m_commandList->ClearRenderTargetView(frameBuffer->RTVHandle, clearColor, 0, nullptr);
    }
}
