#include "pch.h"
#include "DX12CommandList.h"

#include "DX12CommandQueue.h"
#include "../Pipeline/DX12PipelineState.h"
#include "../Resource/DX12ShaderResource.h"
#include "../Descriptors/DX12DescriptorHeap.h"
#include "../Resource/Buffer/DX12Buffer.h"

namespace MirageAPI::DirectX::Command
{
    DX12CommandList::DX12CommandList(
        QIdentifier^ identifier,
        DX12CommandQueue^ queue
    ) : DX12Object(gcnew DX12ObjectData(identifier)),
        _type(queue->Type)
    {
        // command allocator
        ID3D12CommandAllocator* commandAllocator;
        CheckHResult(
            device->CreateCommandAllocator(
                static_cast<D3D12_COMMAND_LIST_TYPE>(_type),
                IID_PPV_ARGS(&commandAllocator)
            ), "Failed to create command allocator"
        );
        _commandAllocator = commandAllocator;

        // command list
        ID3D12GraphicsCommandList* commandList;
        CheckHResult(
            device->CreateCommandList(
                0,
                static_cast<D3D12_COMMAND_LIST_TYPE>(_type),
                _commandAllocator,
                nullptr,
                IID_PPV_ARGS(&commandList)
            ),
            "Failed to create command list"
        );
        _commandList = commandList;
        _commandList->Close(); // instantly close
    }

    DX12CommandList::!DX12CommandList()
    {
        Validate();

        SimpleRelease(_commandList);
        SimpleRelease(_commandAllocator);
    }

    // command list operations

    void DX12CommandList::Reset()
    {
        Validate();

        // native resset
        CheckHResult(_commandAllocator->Reset(), "reset command allocator");
        CheckHResult(_commandList->Reset(_commandAllocator, nullptr), "reset command list");

        // resset target
        _targetDescriptor = nullptr;
    }

    void DX12CommandList::Close()
    {
        Validate();

        // native close
        CheckHResult(_commandList->Close(), "close command list");
    }

    void DX12CommandList::SetPipelineState(Pipeline::DX12PipelineState^ pipelineState)
    {
        Validate();

        _commandList->SetPipelineState(pipelineState->NativePSO);
        _commandList->SetGraphicsRootSignature(pipelineState->RootSignature);
    }

    void DX12CommandList::SetDescriptorHeap(Descriptors::DX12DescriptorHeap^ heap)
    {
        Validate();

        ID3D12DescriptorHeap* arr[] = {heap->NativeHeap};
        _commandList->SetDescriptorHeaps(1, arr);
    }

    void DX12CommandList::SetDescriptorHeaps(array<Descriptors::DX12DescriptorHeap^>^ heaps)
    {
        Validate();

        // native heap arr
        array<ID3D12DescriptorHeap*>^ arr = gcnew array<ID3D12DescriptorHeap*>(heaps->Length);

        for (int i = 0; i < heaps->Length; i++)
            arr[i] = heaps[i]->NativeHeap;

        // set
        pin_ptr<ID3D12DescriptorHeap*> pointer = &arr[0];
        _commandList->SetDescriptorHeaps(heaps->Length, pointer);
    }

    // Render field

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
        _commandList->RSSetViewports(1, &viewport);
    }

    void DX12CommandList::SetScissorRect(
        int left, int top,
        int right, int bottom
    )
    {
        Validate();

        D3D12_RECT rect = {
            left, top,
            right, bottom
        };
        _commandList->RSSetScissorRects(1, &rect);
    }

    // render

    void DX12CommandList::SetPrimitiveTopology(DX12PrimitiveTopology topology)
    {
        Validate();

        _commandList->IASetPrimitiveTopology(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(topology));
    }

    void DX12CommandList::DrawInstanced(UINT vertexCount, UINT count, UINT startVertex, UINT startInstance)
    {
        Validate();

        _commandList->DrawInstanced(vertexCount, count, startVertex, startInstance);
    }

    void DX12CommandList::ClearTarget(Color4 color)
    {
        ClearTarget(color.R, color.G, color.B, color.A);
    }

    void DX12CommandList::ClearTarget(float r, float g, float b, float a)
    {
        CheckNull(_targetDescriptor) throw gcnew DXException("Target is null");
        Validate();

        const float clearColor[] = {r, g, b, a};
        _commandList->ClearRenderTargetView(_targetDescriptor->_CPUHandle, clearColor, 0, nullptr);
    }

    // Render Target

    void DX12CommandList::SetRenderTarget(Descriptors::DX12Descriptor^ targetDescriptor)
    {
        _targetDescriptor = targetDescriptor;

        CheckNull(_targetDescriptor) return;
        Validate();

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = _targetDescriptor->_CPUHandle;
        _commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
    }

    // Geometry

    void DX12CommandList::SetIndexBuffer(Resource::DX12Buffer^ buffer)
    {
        Validate();

        D3D12_INDEX_BUFFER_VIEW view;
        view.BufferLocation = buffer->GPUAddress;
        view.SizeInBytes = buffer->Size;
        view.Format = static_cast<DXGI_FORMAT>(buffer->MetaData->Format);

        _commandList->IASetIndexBuffer(&view);
    }

    void DX12CommandList::SetVertexBuffer(Resource::DX12Buffer^ buffer)
    {
        Validate();

        D3D12_VERTEX_BUFFER_VIEW view;
        view.BufferLocation = buffer->GPUAddress;
        view.SizeInBytes = buffer->Size;
        view.StrideInBytes = buffer->MetaData->Stride;

        _commandList->IASetVertexBuffers(0, 1, &view);
    }

    // resources

    void DX12CommandList::BindResource(UINT index, Resource::DX12ShaderResource^ resource)
    {
        Validate();

        _commandList->SetGraphicsRootShaderResourceView(index, resource->GPUAddress);
    }

    void DX12CommandList::BindConstantBuffer(UINT index, Resource::DX12Buffer^ resource)
    {
        Validate();

        _commandList->SetGraphicsRootConstantBufferView(index, resource->GPUAddress);
    }

    void DX12CommandList::BindDescriptor(UINT index, Descriptors::DX12Descriptor^ descriptor)
    {
        _commandList->SetGraphicsRootDescriptorTable(index, descriptor->_GPUHandle);
    }

    // root constants

    void DX12CommandList::SetRootConstants(UINT index, UINT constantSize, array<float>^ data, UINT offset)
    {
        Validate();

        pin_ptr<float> pointer = &data[0];
        _commandList->SetGraphicsRoot32BitConstants(index, constantSize, pointer, offset);
    }

    void DX12CommandList::SetRootConstant(UINT index, UINT data, UINT offset)
    {
        Validate();

        _commandList->SetGraphicsRoot32BitConstant(index, data, offset);
    }
}
