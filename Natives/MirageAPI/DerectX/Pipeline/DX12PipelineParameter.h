#pragma once

namespace MirageAPI::DirectX
{
    public value struct DX12PipelineParameter
    {
        DX12ResourceType Type;
        unsigned int RegisterSlot;
        unsigned int RegisterSpace;
        unsigned int NumConstants;
        
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