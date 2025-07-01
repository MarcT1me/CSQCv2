#pragma once

#include "..\Resource\DX12Resource.h"
#include "..\Resource\DX12ResourceConfig.h"

namespace MirageAPI::DirectX
{
    public ref class DX12UploadBuffer : public DX12Resource
    {
    public:
        DX12UploadBuffer(DX12ResourceConfig config);

        void TransitionState(
            DX12CommandList^ commandList,
            DX12ResourceState newState
        ) override;

        virtual property DX12ResourceType ResourceType
        {
            DX12ResourceType get() override { return DX12ResourceType::UploadBuffer; }
        }
    };
}
