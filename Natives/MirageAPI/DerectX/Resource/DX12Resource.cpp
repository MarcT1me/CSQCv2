#include "pch.h"
#include "DX12Resource.h"

#include "../Command/DX12CommandList.h"

namespace MirageAPI::DirectX::Resource
{
    void DX12Resource::!DX12Resource()
    {
        Validate();

        if (_nativeResource)
        {
            Unmap();
            SimpleRelease(_nativeResource);
        }
    }

    void DX12Resource::AddRef()
    {
        Threading::Interlocked::Increment(_refCount);
    }

    void DX12Resource::Release()
    {
        if (Threading::Interlocked::Decrement(_refCount) == 0)
        {
            delete this;
        }
    }

    void DX12Resource::TransitionState(
        Command::DX12CommandList^ commandList,
        DX12ResourceState newState
    )
    {
        Validate();

        if (_currentState == newState)
            return;

        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = _nativeResource;
        barrier.Transition.StateBefore = static_cast<D3D12_RESOURCE_STATES>(_currentState);
        barrier.Transition.StateAfter = static_cast<D3D12_RESOURCE_STATES>(newState);
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        commandList->NativeList->ResourceBarrier(1, &barrier);
        _currentState = newState;
    }

    void* DX12Resource::Map()
    {
        Validate();

        // mapping data to range
        void* pData = nullptr;
        D3D12_RANGE range = {0, _size};

        CheckHResult(
            _nativeResource->Map(0, &range, &pData),
            "Any err in resource map operation"
        );

        return pData;
    }

    void DX12Resource::Unmap()
    {
        Validate();

        // just unmapping
        D3D12_RANGE range = {0, _size};
        _nativeResource->Unmap(0, &range);
    }
}
