#pragma once

#include "DX12CommandList.h"

namespace MirageAPI::DirectX
{
    public ref class DX12WindowCommandList sealed : public DX12CommandList
    {
    public:
        DX12WindowCommandList() : DX12CommandList(DX12CommandListType::Direct)
        {
        }
    };
}
