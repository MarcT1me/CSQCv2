#pragma once

#include "..\Resource\DX12Resource.h"
#include "..\Resource\DX12ResourceConfig.h"

namespace MirageAPI::DirectX
{
    public ref class DX12Buffer abstract : public DX12Resource
    {
    internal:
        unsigned int m_elementCount;
        unsigned int m_stride;

    public:
        DX12Buffer(DX12ResourceConfig config);

        void TransitionState(
            DX12CommandList^ commandList,
            DX12ResourceState newState
            ) override;

        property unsigned int ElementCount
        {
            unsigned int get() { return m_elementCount; }
        }

        property unsigned int Stride
        {
            unsigned int get() { return m_stride; }
        }

        virtual void UploadData(
            array<System::Byte>^ data
        );
    };
}
