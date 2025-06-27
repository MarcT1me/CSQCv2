#pragma once

#include <d3d12.h>

namespace MirageAPI::DirectX
{
    public enum class DX12ShaderType
    {
        Vertex,
        Pixel,
        Geometry,
        Compute,
        Domain,
        Hull
    };
    
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
