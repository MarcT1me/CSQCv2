#pragma once

namespace MirageAPI::DirectX
{
    public value struct DX12RootParameter
    {
        DX12ResourceType Type;
        unsigned int RegisterSlot;
        unsigned int RegisterSpace;
        unsigned int NumConstants;
        
        static property DX12RootParameter Default
        {
            DX12RootParameter get()
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