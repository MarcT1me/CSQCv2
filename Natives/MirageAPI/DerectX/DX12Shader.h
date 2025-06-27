#pragma once

#include <d3d12.h>
#include "DX12Enums.h"

namespace MirageAPI::DirectX
{
    public ref class DX12Shader
    {
        array<System::Byte>^ m_bytecode;
        DX12ShaderType m_type;
    
    public:
        DX12Shader(array<System::Byte>^ bytecode, DX12ShaderType type);
        
        property array<System::Byte>^ Bytecode { 
            array<System::Byte>^ get() { return m_bytecode; } 
        }
        property DX12ShaderType Type { 
            DX12ShaderType get() { return m_type; } 
        }
        
        D3D12_SHADER_BYTECODE GetNativeBytecode();
    };
}
