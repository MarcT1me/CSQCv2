#pragma once

namespace MirageAPI::DirectX
{
    public value struct DX12BlendConfig
    {
        bool BlendEnable;
        DX12BlendOptions SrcBlend;
        DX12BlendOptions DestBlend;
        DX12BlendOptions BlendOp;
        DX12BlendOptions SrcBlendAlpha;
        DX12BlendOptions DestBlendAlpha;
        DX12BlendOptions BlendOpAlpha;
        
        bool LogicOpEnable;
        DX12LogicOperators LogicOp;
        
        DX12ColorWriteEnable RenderTargetWriteMask;

        static property DX12BlendConfig Default
        {
            DX12BlendConfig get()
            {
                return {
                    true,
                    DX12BlendOptions::SrcAlpha,
                    DX12BlendOptions::InvSrcAlpha,
                    DX12BlendOptions::OpAdd,
                    DX12BlendOptions::One,
                    DX12BlendOptions::Zero,
                    DX12BlendOptions::OpAdd,
                    false,
                    DX12LogicOperators::Noop,
                    DX12ColorWriteEnable::All
                };
            }
        }
    };
}
