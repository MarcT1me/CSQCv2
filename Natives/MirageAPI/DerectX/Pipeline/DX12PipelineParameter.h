#pragma once

namespace MirageAPI::DirectX::Pipeline
{
    public value struct DX12PipelineParameter
    {
        DX12ResourceType Type;
        UINT RegisterSlot;
        UINT RegisterSpace;
        UINT NumConstants;
        
        static property DX12PipelineParameter Default
        {
            DX12PipelineParameter get()
            {
                return {
                    .Type = DX12ResourceType::ConstantBuffer,
                    .RegisterSlot = 0U,
                    .RegisterSpace = 0U,
                    .NumConstants = 0U,
                };
            }
        }
    };
}