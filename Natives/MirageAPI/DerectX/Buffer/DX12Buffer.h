#pragma once

#include "..\Resource\DX12Resource.h"
#include "..\Resource\DX12ResourceConfig.h"

namespace MirageAPI::DirectX
{
    public ref class DX12Buffer : public DX12Resource
    {
    internal:
        unsigned int  m_stride;
        unsigned int m_elementCount;
        DX12ResourceType m_bufferType;

    public:
        DX12Buffer(DX12ResourceConfig config);

        void TransitionState(
            DX12CommandList^ commandList,
            DX12ResourceState newState
        ) override;

        virtual property DX12ResourceType ResourceType
        {
            DX12ResourceType get() override { return m_bufferType; }
        }

        virtual void UploadData(array<System::Byte>^ data);

        property unsigned int  Stride { unsigned int  get() { return m_stride; } }
    };
}
