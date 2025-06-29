#pragma once

#include "DX12CommandList.h"
#include "../Buffer/DX12FrameBuffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12WindowCommandList sealed : public DX12CommandList
    {
    public:
        DX12WindowCommandList(
        ) : DX12CommandList(
            DX12CommandListType::Direct
        )
        {
        }

        void ClearRenderTargetView(DX12FrameBuffer^ frameBuffer, float r, float g, float b, float a);
    };
}
