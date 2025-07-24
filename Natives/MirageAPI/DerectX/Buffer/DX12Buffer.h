#pragma once

// Mirage ecosystem
#include "../Resource/DX12Resource.h"
#include "../Resource/DX12ResourceConfig.h"

namespace MirageAPI::DirectX
{
    ref class DX12CommandList;
}

// buffer
namespace MirageAPI::DirectX
{
    public ref class DX12Buffer abstract : public DX12Resource
    {
    internal:
        // description
        UINT m_elementCount;
        UINT m_stride;

    public:
        // constructors and deconstructors
        DX12Buffer(DX12ResourceConfig config);

        // other properties
        property UINT ElementCount
        {
            UINT get() { return m_elementCount; }
        }
        property UINT Stride
        {
            UINT get() { return m_stride; }
        }

        // buffer operations
        virtual void TransitionState(
            DX12CommandList^ commandList,
            DX12ResourceState newState
        );

        virtual void UploadData(
            array<System::Byte>^ data
        );
    };
}
