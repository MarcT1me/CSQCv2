#pragma once

// Mirage ecosystem
#include "../DX12Resource.h"
#include "../DX12ResourceConfig.h"

namespace MirageAPI::DirectX::Command
{
    ref class DX12CommandList;
}

// buffer
namespace MirageAPI::DirectX::Resource
{
    public ref class DX12Buffer : public DX12Resource
    {
    public:
        // constructors and deconstructors
        DX12Buffer(DX12ResourceConfig^ config);

        // buffer operations
        virtual void UploadData(
            array<Byte>^ data
        );
    };
}
